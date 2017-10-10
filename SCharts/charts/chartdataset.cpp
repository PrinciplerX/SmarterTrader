#include <chartdataset_p.h>
#include <chartpresenter_p.h>
#include <QChart>
#include <qchart_p.h>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <qvalueaxis_p.h>
#include <QCategoryAxis>
#include <qabstractseries_p.h>
#include <QAbstractBarSeries>
#include <QStackedBarSeries>
#include <QPercentBarSeries>
#include <QPieSeries>
#include <chartitem_p.h>
#include <xydomain_p.h>
#include <xypolardomain_p.h>
#include <xlogydomain_p.h>
#include <logxydomain_p.h>
#include <logxlogydomain_p.h>
#include <xlogypolardomain_p.h>
#include <logxypolardomain_p.h>
#include <logxlogypolardomain_p.h>
#include <glxyseriesdata_p.h>

#ifndef QT_QREAL_IS_FLOAT
#include <QDateTimeAxis>
#endif

SCHARTS_BEGIN_NAMESPACE

ChartDataSet::ChartDataSet(QChart *chart)
    : QObject(chart),
      m_chart(chart),
      m_glXYSeriesDataManager(new GLXYSeriesDataManager(this))
{

}

ChartDataSet::~ChartDataSet()
{
    deleteAllSeries();
    deleteAllAxes();
}

/*! \brief 该函数将序列对象添加到图表数据集合（ChartDataSet）中，并且从调用者接管对象的所有权。*/
void ChartDataSet::addSeries(QAbstractSeries *series)
{
    if (m_seriesList.contains(series)) {
        qWarning() << QObject::tr("Can not add series. Series already on the chart.");
        return;
    }

    // 忽视添加到极坐标图中不支持的序列类型
    if (m_chart && m_chart->chartType() == QChart::ChartTypePolar) {
        if (!(series->type() == QAbstractSeries::SeriesTypeArea
            || series->type() == QAbstractSeries::SeriesTypeLine
            || series->type() == QAbstractSeries::SeriesTypeScatter
            || series->type() == QAbstractSeries::SeriesTypeSpline)) {
            qWarning() << QObject::tr("Can not add series. Series type is not supported by a polar chart.");
            return;
        }
        // 禁用添加到极坐标图中序列的OpenGL加速功能
        series->setUseOpenGL(false);
        series->d_ptr->setDomain(new XYPolarDomain());
        // 为上边（upper）与下边（lower）的子序列设置正确的domain。
        if (series->type() == QAbstractSeries::SeriesTypeArea) {
            foreach (QObject *child, series->children()) {
                if (qobject_cast<QAbstractSeries *>(child)) {
                    QAbstractSeries *childSeries = qobject_cast<QAbstractSeries *>(child);
                    childSeries->d_ptr->setDomain(new XYPolarDomain());
                }
            }
        }
    } else {
        series->d_ptr->setDomain(new XYDomain());
    }

    series->d_ptr->initializeDomain();
    m_seriesList.append(series);

    series->setParent(this); // 取得所有权
    series->d_ptr->m_chart = m_chart;

    emit seriesAdded(series);
}

/*!
 * 该函数将坐标轴添加到图表数据集合（ChartDataSet）中，并且从调用者接管坐标轴的所有权。
 * \note 添加新的坐标轴并不会删除该位置上原来的坐标轴对象，同一坐标图的同一方向上可以放多个坐标轴。
 */
void ChartDataSet::addAxis(QAbstractAxis *axis, Qt::Alignment aligment)
{
    if (m_axisList.contains(axis)) {
        qWarning() << QObject::tr("Can not add axis. Axis already on the chart.");
        return;
    }

    axis->d_ptr->setAlignment(aligment);

    if (!axis->alignment()) {
        qWarning() << QObject::tr("No alignment specified !");
        return;
    };

    AbstractDomain *newDomain;
    if (m_chart && m_chart->chartType() == QChart::ChartTypePolar)
        newDomain = new XYPolarDomain();
    else
        newDomain = new XYDomain();

    QSharedPointer<AbstractDomain> domain(newDomain);
    axis->d_ptr->initializeDomain(domain.data());

    axis->setParent(this);
    axis->d_ptr->m_chart = m_chart;
    m_axisList.append(axis);

    emit axisAdded(axis);
}

/*! \brief 该函数将序列对象从图表数据集合（ChartDataSet）中移除，并把序列所有权归还到调用者。*/
void ChartDataSet::removeSeries(QAbstractSeries *series)
{
    if (! m_seriesList.contains(series)) {
        qWarning() << QObject::tr("Can not remove series. Series not found on the chart.");
        return;
    }

    QList<QAbstractAxis*> axes = series->d_ptr->m_axes;

    foreach(QAbstractAxis* axis, axes) {
        detachAxis(series,axis);
    }

    emit seriesRemoved(series);
    m_seriesList.removeAll(series);

    // 将domain还原为默认设置。
    series->d_ptr->setDomain(new XYDomain());
    series->setParent(0);
    series->d_ptr->m_chart = 0;

    // 仅有 QXYSeries 类型的序列（如 QLineSeries 和 QScatterSeries）支持 OpenGL 加速。
    QXYSeries *xySeries = qobject_cast<QXYSeries *>(series);
    if (xySeries)
        m_glXYSeriesDataManager->removeSeries(xySeries);
}

/*
 * 该函数将坐标轴对象从图表数据集合（ChartDataSet）中移除，并把坐标轴所有权归还到调用者。和坐标轴关联的序列将被取消
 * 关联。
 */
void ChartDataSet::removeAxis(QAbstractAxis *axis)
{
    if (! m_axisList.contains(axis)) {
        qWarning() << QObject::tr("Can not remove axis. Axis not found on the chart.");
        return;
    }

    QList<QAbstractSeries*> series =  axis->d_ptr->m_series;

    // 取消与序列对象之间的关联。
    foreach(QAbstractSeries* s, series) {
      detachAxis(s,axis);
    }

    emit axisRemoved(axis);
    m_axisList.removeAll(axis);

    axis->setParent(0);
    axis->d_ptr->m_chart = 0;
}

/*! \brief 该函数将坐标轴关联到序列上，如果成功则返回false。*/
bool ChartDataSet::attachAxis(QAbstractSeries *series,QAbstractAxis *axis)
{
    Q_ASSERT(axis);

    if (!series)
        return false;

    QList<QAbstractSeries *> attachedSeriesList = axis->d_ptr->m_series;
    QList<QAbstractAxis *> attachedAxisList = series->d_ptr->m_axes;

    if (!m_seriesList.contains(series)) {
        qWarning() << QObject::tr("Can not find series on the chart.");
        return false;
    }

    if (axis && !m_axisList.contains(axis)) {
        qWarning() << QObject::tr("Can not find axis on the chart.");
        return false;
    }

    if (attachedAxisList.contains(axis)) {
        qWarning() << QObject::tr("Axis already attached to series.");
        return false;
    }

    if (attachedSeriesList.contains(series)) {
        qWarning() << QObject::tr("Axis already attached to series.");
        return false;
    }

    AbstractDomain *domain = series->d_ptr->domain();
    AbstractDomain::DomainType type = selectDomain(attachedAxisList<<axis);

    if (type == AbstractDomain::UndefinedDomain) return false;

    // 序列和坐标轴应该具有相同的domain类型，如果domain类型不同，则需要重新设置序列对象的domain。
    if (domain->type() != type) {
        AbstractDomain *old = domain;
        domain = createDomain(type);
        domain->setRange(old->minX(), old->maxX(), old->minY(), old->maxY());
        // 将domain大小设置为原有domain的大小，因为在几何位置（geometry）改变之前，不会获得更新显示（update）。
        domain->setSize(old->size());
    }

    // 需要将序列的domain关联到坐标轴上。
    if (!domain)
        return false;
    if (!domain->attachAxis(axis))
        return false;

    // 暂时阻塞domain对象的信号
    QList<AbstractDomain *> blockedDomains;
    domain->blockRangeSignals(true);
    blockedDomains << domain;

    if (domain != series->d_ptr->domain()) {
        // 要替换掉序列对象的domain，首先需要将序列原有domain和序列所关联坐标轴之间的关联关系替换掉。
        foreach (QAbstractAxis *axis, series->d_ptr->m_axes) {
            series->d_ptr->domain()->detachAxis(axis);
            domain->attachAxis(axis);
            // 暂时阻塞被替换关联关系的坐标轴对象关联的其他序列对象的domain，防止他们发送信号给该坐标轴对象。
            foreach (QAbstractSeries *otherSeries, axis->d_ptr->m_series) {
                if (otherSeries != series && otherSeries->d_ptr->domain()) {
                    if (!otherSeries->d_ptr->domain()->rangeSignalsBlocked()) {
                        otherSeries->d_ptr->domain()->blockRangeSignals(true);
                        blockedDomains << otherSeries->d_ptr->domain();
                    }
                }
            }
        }
        // 已经把关联关系更新完毕，现在可以替换掉domain对象。
        series->d_ptr->setDomain(domain);
        series->d_ptr->initializeDomain();
    }

    // 更新彼此的关联列表
    series->d_ptr->m_axes<<axis;
    axis->d_ptr->m_series<<series;

    series->d_ptr->initializeAxes();
    axis->d_ptr->initializeDomain(domain);
    connect(axis, &QAbstractAxis::reverseChanged, this, &ChartDataSet::reverseChanged);
    // 恢复暂时被阻塞信号的domain对象的信号接收功能
    foreach (AbstractDomain *blockedDomain, blockedDomains)
        blockedDomain->blockRangeSignals(false);

    return true;
}

/*! \brief 该函数将坐标轴和序列对象解除关联，如果成功则返回true。*/
bool ChartDataSet::detachAxis(QAbstractSeries* series,QAbstractAxis *axis)
{
    Q_ASSERT(series);
    Q_ASSERT(axis);

    QList<QAbstractSeries* > attachedSeriesList = axis->d_ptr->m_series;
    QList<QAbstractAxis* > attachedAxisList = series->d_ptr->m_axes;
    AbstractDomain* domain =  series->d_ptr->domain();

    if (!m_seriesList.contains(series)) {
        qWarning() << QObject::tr("Can not find series on the chart.");
        return false;
    }

    if (axis && !m_axisList.contains(axis)) {
        qWarning() << QObject::tr("Can not find axis on the chart.");
        return false;
    }

    if (!attachedAxisList.contains(axis)) {
        qWarning() << QObject::tr("Axis not attached to series.");
        return false;
    }

    Q_ASSERT(axis->d_ptr->m_series.contains(series));

    // 主要是解除序列的domain对象和坐标轴之间的关联
    domain->detachAxis(axis);

    series->d_ptr->m_axes.removeAll(axis);
    axis->d_ptr->m_series.removeAll(series);
    disconnect(axis, &QAbstractAxis::reverseChanged, this, &ChartDataSet::reverseChanged);
    return true;
}

void ChartDataSet::createDefaultAxes()
{
    if (m_seriesList.isEmpty())
        return;

    QAbstractAxis::AxisTypes typeX(0);
    QAbstractAxis::AxisTypes typeY(0);

    // 移除可能存在的坐标轴
    deleteAllAxes();

    Q_ASSERT(m_axisList.isEmpty());

    // 根据当前的序列类型选择所需的x坐标轴和y坐标轴的类型
    foreach(QAbstractSeries* s, m_seriesList) {
        typeX |= s->d_ptr->defaultAxisType(Qt::Horizontal);
        typeY |= s->d_ptr->defaultAxisType(Qt::Vertical);
    }

    createAxes(typeX, Qt::Horizontal);
    createAxes(typeY, Qt::Vertical);
}

void ChartDataSet::createAxes(QAbstractAxis::AxisTypes type, Qt::Orientation orientation)
{
    QAbstractAxis *axis = 0;
    // 决定创建哪种坐标轴

    switch (type) {
        case QAbstractAxis::AxisTypeValue:
        axis = new QValueAxis(this);
        break;
        case QAbstractAxis::AxisTypeBarCategory:
        axis = new QBarCategoryAxis(this);
        break;
        case QAbstractAxis::AxisTypeCategory:
        axis = new QCategoryAxis(this);
        break;
#ifndef QT_QREAL_IS_FLOAT
        case QAbstractAxis::AxisTypeDateTime:
        axis = new QDateTimeAxis(this);
        break;
#endif
        default:
        axis = 0;
        break;
    }

    if (axis) {
        // 为所有序列对象创建一个坐标轴
        addAxis(axis,orientation==Qt::Horizontal?Qt::AlignBottom:Qt::AlignLeft);
        qreal min = 0;
        qreal max = 0;
        findMinMaxForSeries(m_seriesList,orientation,min,max);
        foreach(QAbstractSeries *s, m_seriesList) {
            attachAxis(s,axis);
        }
        axis->setRange(min,max);
    } else {
        // 为每一个序列对象创建独立的坐标轴
        foreach(QAbstractSeries *s, m_seriesList) {
            QAbstractAxis *axis = s->d_ptr->createDefaultAxis(orientation);
            if(axis) {
                addAxis(axis,orientation==Qt::Horizontal?Qt::AlignBottom:Qt::AlignLeft);
                attachAxis(s,axis);
            }
        }
    }
}

/*! \brief 计算某一方向上（水平/数值）序列对象的最大/最小值。序列对象的范围由其domain对象指定。*/
void ChartDataSet::findMinMaxForSeries(QList<QAbstractSeries *> series,Qt::Orientations orientation, qreal &min, qreal &max)
{
    Q_ASSERT(!series.isEmpty());

    AbstractDomain *domain = series.first()->d_ptr->domain();
    min = (orientation == Qt::Vertical) ? domain->minY() : domain->minX();
    max = (orientation == Qt::Vertical) ? domain->maxY() : domain->maxX();

    for (int i = 1; i< series.size(); i++) {
        AbstractDomain *domain = series[i]->d_ptr->domain();
        min = qMin((orientation == Qt::Vertical) ? domain->minY() : domain->minX(), min);
        max = qMax((orientation == Qt::Vertical) ? domain->maxY() : domain->maxX(), max);
    }
    if (min == max) {
        min -= 0.5;
        max += 0.5;
    }
}

/*! \brief 删除所有的序列对象。*/
void ChartDataSet::deleteAllSeries()
{
    foreach (QAbstractSeries *s , m_seriesList){
        removeSeries(s);
        s->deleteLater();
    }
    Q_ASSERT(m_seriesList.count() == 0);
}

/*! \brief 删除所有的坐标轴对象。*/
void ChartDataSet::deleteAllAxes()
{
    foreach (QAbstractAxis *a , m_axisList){
        removeAxis(a);
        a->deleteLater();
    }
    Q_ASSERT(m_axisList.count() == 0);
}

/*! \brief 拉进（放大）至指定区域。*/
void ChartDataSet::zoomInDomain(const QRectF &rect)
{
    QList<AbstractDomain*> domains;
    // 阻塞domain信号
    foreach(QAbstractSeries *s, m_seriesList) {
        AbstractDomain* domain = s->d_ptr->domain();
        s->d_ptr->m_domain->blockRangeSignals(true);
        domains<<domain;
    }

    // 将每个domain拉进（放大）至指定区域
    foreach(AbstractDomain *domain, domains)
        domain->zoomIn(rect);

    // 恢复domain重新接收信号
    foreach(AbstractDomain *domain, domains)
        domain->blockRangeSignals(false);
}

/*! \brief 远离（缩小）至某一区域。*/
void ChartDataSet::zoomOutDomain(const QRectF &rect)
{
    QList<AbstractDomain*> domains;
    // 阻塞domain信号
    foreach(QAbstractSeries *s, m_seriesList) {
        AbstractDomain* domain = s->d_ptr->domain();
        s->d_ptr->m_domain->blockRangeSignals(true);
        domains<<domain;
    }

    // 将每个domain远离（缩小）至指定区域
    foreach(AbstractDomain *domain, domains)
        domain->zoomOut(rect);

    // 恢复domain重新接收信号
    foreach(AbstractDomain *domain, domains)
        domain->blockRangeSignals(false);
}

/*! \brief 重置放缩区域。*/
void ChartDataSet::zoomResetDomain()
{
    QList<AbstractDomain*> domains;
    foreach (QAbstractSeries *s, m_seriesList) {
        AbstractDomain *domain = s->d_ptr->domain();
        s->d_ptr->m_domain->blockRangeSignals(true);
        domains << domain;
    }

    foreach (AbstractDomain *domain, domains)
        domain->zoomReset();

    foreach (AbstractDomain *domain, domains)
        domain->blockRangeSignals(false);
}

/*! \return 是否发生了放缩。*/
bool ChartDataSet::isZoomedDomain()
{
    foreach (QAbstractSeries *s, m_seriesList) {
        if (s->d_ptr->domain()->isZoomed())
            return true;
    }
    return false;
}

/*! \brief 按指定位移滚动区域。*/
void ChartDataSet::scrollDomain(qreal dx, qreal dy)
{
    QList<AbstractDomain*> domains;
    foreach(QAbstractSeries *s, m_seriesList) {
        AbstractDomain* domain = s->d_ptr->domain();
        s->d_ptr->m_domain->blockRangeSignals(true);
        domains<<domain;
    }

    foreach(AbstractDomain *domain, domains)
        domain->move(dx, dy);

    foreach(AbstractDomain *domain, domains)
        domain->blockRangeSignals(false);
}

/*! \brief 将序列对象的坐标点位映射为数值。*/
QPointF ChartDataSet::mapToValue(const QPointF &position, QAbstractSeries *series)
{
    QPointF point;
    if (series == 0 && !m_seriesList.isEmpty())
        series = m_seriesList.first();

    if (series && series->type() == QAbstractSeries::SeriesTypePie)
        return point;

    if (series && m_seriesList.contains(series))
        point = series->d_ptr->m_domain->calculateDomainPoint(position - m_chart->plotArea().topLeft());
    return point;
}

/*! \brief 将序列对象的数值位映射为点位。*/
QPointF ChartDataSet::mapToPosition(const QPointF &value, QAbstractSeries *series)
{
    QPointF point = m_chart->plotArea().topLeft();
    if (series == 0 && !m_seriesList.isEmpty())
        series = m_seriesList.first();

    if (series && series->type() == QAbstractSeries::SeriesTypePie)
        return QPoint(0, 0);

    bool ok;
    if (series && m_seriesList.contains(series))
        point += series->d_ptr->m_domain->calculateGeometryPoint(value, ok);
    return point;
}

/*! \return 存储的坐标轴列表。*/
QList<QAbstractAxis *> ChartDataSet::axes() const
{
   return m_axisList;
}

/*! \return 存储的序列对象列表。*/
QList<QAbstractSeries *> ChartDataSet::series() const
{
    return m_seriesList;
}

/*! \brief 根据坐标轴类型选择出适当的domain类型。*/
AbstractDomain::DomainType ChartDataSet::selectDomain(QList<QAbstractAxis *> axes)
{
    enum Type {
        Undefined = 0,
        LogType = 0x1,
        ValueType = 0x2
    };

    int horizontal(Undefined);
    int vertical(Undefined);

    // 如果未设置图表对象，则假定为直角坐标图。
    QChart::ChartType chartType(QChart::ChartTypeCartesian);
    if (m_chart)
        chartType = m_chart->chartType();

    foreach (QAbstractAxis *axis, axes)
    {
        switch (axis->type()) {
        case QAbstractAxis::AxisTypeLogValue:
            if (axis->orientation() == Qt::Horizontal)
                horizontal |= LogType;
            if (axis->orientation() == Qt::Vertical)
                vertical |= LogType;
            break;
        case QAbstractAxis::AxisTypeValue:
        case QAbstractAxis::AxisTypeBarCategory:
        case QAbstractAxis::AxisTypeCategory:
        case QAbstractAxis::AxisTypeDateTime:
            if (axis->orientation() == Qt::Horizontal)
                horizontal |= ValueType;
            if (axis->orientation() == Qt::Vertical)
                vertical |= ValueType;
            break;
        default:
            qWarning() << "Undefined type";
            break;
        }
    }

    if (vertical == Undefined)
        vertical = ValueType;
    if (horizontal == Undefined)
        horizontal = ValueType;

    if (vertical == ValueType && horizontal == ValueType) {
        if (chartType == QChart::ChartTypeCartesian)
            return AbstractDomain::XYDomain;
        else if (chartType == QChart::ChartTypePolar)
            return AbstractDomain::XYPolarDomain;
    }

    if (vertical == LogType && horizontal == ValueType) {
        if (chartType == QChart::ChartTypeCartesian)
            return AbstractDomain::XLogYDomain;
        if (chartType == QChart::ChartTypePolar)
            return AbstractDomain::XLogYPolarDomain;
    }

    if (vertical == ValueType && horizontal == LogType) {
        if (chartType == QChart::ChartTypeCartesian)
            return AbstractDomain::LogXYDomain;
        else if (chartType == QChart::ChartTypePolar)
            return AbstractDomain::LogXYPolarDomain;
    }

    if (vertical == LogType && horizontal == LogType) {
        if (chartType == QChart::ChartTypeCartesian)
            return AbstractDomain::LogXLogYDomain;
        else if (chartType == QChart::ChartTypePolar)
            return AbstractDomain::LogXLogYPolarDomain;
    }

    return AbstractDomain::UndefinedDomain;
}

//refactor create factory
/*! \brief 根据指定的domain类型\a type 构造对应的domain对象。*/
AbstractDomain* ChartDataSet::createDomain(AbstractDomain::DomainType type)
{
    switch (type)
    {
    case AbstractDomain::LogXLogYDomain:
        return new LogXLogYDomain();
    case AbstractDomain::XYDomain:
        return new XYDomain();
    case AbstractDomain::XLogYDomain:
        return new XLogYDomain();
    case AbstractDomain::LogXYDomain:
        return new LogXYDomain();
    case AbstractDomain::XYPolarDomain:
        return new XYPolarDomain();
    case AbstractDomain::XLogYPolarDomain:
        return new XLogYPolarDomain();
    case AbstractDomain::LogXYPolarDomain:
        return new LogXYPolarDomain();
    case AbstractDomain::LogXLogYPolarDomain:
        return new LogXLogYPolarDomain();
    default:
        return 0;
    }
}

void ChartDataSet::reverseChanged()
{
    QAbstractAxis *axis = qobject_cast<QAbstractAxis *>(sender());
    if (axis)
        m_glXYSeriesDataManager->handleAxisReverseChanged(axis->d_ptr->m_series);
}

#include "moc_chartdataset_p.cpp"

SCHARTS_END_NAMESPACE
