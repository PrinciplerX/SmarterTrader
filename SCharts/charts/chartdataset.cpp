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

/*! \brief �ú��������ж�����ӵ�ͼ�����ݼ��ϣ�ChartDataSet���У����Ҵӵ����߽ӹܶ��������Ȩ��*/
void ChartDataSet::addSeries(QAbstractSeries *series)
{
    if (m_seriesList.contains(series)) {
        qWarning() << QObject::tr("Can not add series. Series already on the chart.");
        return;
    }

    // ������ӵ�������ͼ�в�֧�ֵ���������
    if (m_chart && m_chart->chartType() == QChart::ChartTypePolar) {
        if (!(series->type() == QAbstractSeries::SeriesTypeArea
            || series->type() == QAbstractSeries::SeriesTypeLine
            || series->type() == QAbstractSeries::SeriesTypeScatter
            || series->type() == QAbstractSeries::SeriesTypeSpline)) {
            qWarning() << QObject::tr("Can not add series. Series type is not supported by a polar chart.");
            return;
        }
        // ������ӵ�������ͼ�����е�OpenGL���ٹ���
        series->setUseOpenGL(false);
        series->d_ptr->setDomain(new XYPolarDomain());
        // Ϊ�ϱߣ�upper�����±ߣ�lower����������������ȷ��domain��
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

    series->setParent(this); // ȡ������Ȩ
    series->d_ptr->m_chart = m_chart;

    emit seriesAdded(series);
}

/*!
 * �ú�������������ӵ�ͼ�����ݼ��ϣ�ChartDataSet���У����Ҵӵ����߽ӹ������������Ȩ��
 * \note ����µ������Ტ����ɾ����λ����ԭ�������������ͬһ����ͼ��ͬһ�����Ͽ��ԷŶ�������ᡣ
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

/*! \brief �ú��������ж����ͼ�����ݼ��ϣ�ChartDataSet�����Ƴ���������������Ȩ�黹�������ߡ�*/
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

    // ��domain��ԭΪĬ�����á�
    series->d_ptr->setDomain(new XYDomain());
    series->setParent(0);
    series->d_ptr->m_chart = 0;

    // ���� QXYSeries ���͵����У��� QLineSeries �� QScatterSeries��֧�� OpenGL ���١�
    QXYSeries *xySeries = qobject_cast<QXYSeries *>(series);
    if (xySeries)
        m_glXYSeriesDataManager->removeSeries(xySeries);
}

/*
 * �ú���������������ͼ�����ݼ��ϣ�ChartDataSet�����Ƴ�����������������Ȩ�黹�������ߡ�����������������н���ȡ��
 * ������
 */
void ChartDataSet::removeAxis(QAbstractAxis *axis)
{
    if (! m_axisList.contains(axis)) {
        qWarning() << QObject::tr("Can not remove axis. Axis not found on the chart.");
        return;
    }

    QList<QAbstractSeries*> series =  axis->d_ptr->m_series;

    // ȡ�������ж���֮��Ĺ�����
    foreach(QAbstractSeries* s, series) {
      detachAxis(s,axis);
    }

    emit axisRemoved(axis);
    m_axisList.removeAll(axis);

    axis->setParent(0);
    axis->d_ptr->m_chart = 0;
}

/*! \brief �ú���������������������ϣ�����ɹ��򷵻�false��*/
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

    // ���к�������Ӧ�þ�����ͬ��domain���ͣ����domain���Ͳ�ͬ������Ҫ�����������ж����domain��
    if (domain->type() != type) {
        AbstractDomain *old = domain;
        domain = createDomain(type);
        domain->setRange(old->minX(), old->maxX(), old->minY(), old->maxY());
        // ��domain��С����Ϊԭ��domain�Ĵ�С����Ϊ�ڼ���λ�ã�geometry���ı�֮ǰ�������ø�����ʾ��update����
        domain->setSize(old->size());
    }

    // ��Ҫ�����е�domain�������������ϡ�
    if (!domain)
        return false;
    if (!domain->attachAxis(axis))
        return false;

    // ��ʱ����domain������ź�
    QList<AbstractDomain *> blockedDomains;
    domain->blockRangeSignals(true);
    blockedDomains << domain;

    if (domain != series->d_ptr->domain()) {
        // Ҫ�滻�����ж����domain��������Ҫ������ԭ��domain������������������֮��Ĺ�����ϵ�滻����
        foreach (QAbstractAxis *axis, series->d_ptr->m_axes) {
            series->d_ptr->domain()->detachAxis(axis);
            domain->attachAxis(axis);
            // ��ʱ�������滻������ϵ�����������������������ж����domain����ֹ���Ƿ����źŸ������������
            foreach (QAbstractSeries *otherSeries, axis->d_ptr->m_series) {
                if (otherSeries != series && otherSeries->d_ptr->domain()) {
                    if (!otherSeries->d_ptr->domain()->rangeSignalsBlocked()) {
                        otherSeries->d_ptr->domain()->blockRangeSignals(true);
                        blockedDomains << otherSeries->d_ptr->domain();
                    }
                }
            }
        }
        // �Ѿ��ѹ�����ϵ������ϣ����ڿ����滻��domain����
        series->d_ptr->setDomain(domain);
        series->d_ptr->initializeDomain();
    }

    // ���±˴˵Ĺ����б�
    series->d_ptr->m_axes<<axis;
    axis->d_ptr->m_series<<series;

    series->d_ptr->initializeAxes();
    axis->d_ptr->initializeDomain(domain);
    connect(axis, &QAbstractAxis::reverseChanged, this, &ChartDataSet::reverseChanged);
    // �ָ���ʱ�������źŵ�domain������źŽ��չ���
    foreach (AbstractDomain *blockedDomain, blockedDomains)
        blockedDomain->blockRangeSignals(false);

    return true;
}

/*! \brief �ú���������������ж���������������ɹ��򷵻�true��*/
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

    // ��Ҫ�ǽ�����е�domain�����������֮��Ĺ���
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

    // �Ƴ����ܴ��ڵ�������
    deleteAllAxes();

    Q_ASSERT(m_axisList.isEmpty());

    // ���ݵ�ǰ����������ѡ�������x�������y�����������
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
    // ������������������

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
        // Ϊ�������ж��󴴽�һ��������
        addAxis(axis,orientation==Qt::Horizontal?Qt::AlignBottom:Qt::AlignLeft);
        qreal min = 0;
        qreal max = 0;
        findMinMaxForSeries(m_seriesList,orientation,min,max);
        foreach(QAbstractSeries *s, m_seriesList) {
            attachAxis(s,axis);
        }
        axis->setRange(min,max);
    } else {
        // Ϊÿһ�����ж��󴴽�������������
        foreach(QAbstractSeries *s, m_seriesList) {
            QAbstractAxis *axis = s->d_ptr->createDefaultAxis(orientation);
            if(axis) {
                addAxis(axis,orientation==Qt::Horizontal?Qt::AlignBottom:Qt::AlignLeft);
                attachAxis(s,axis);
            }
        }
    }
}

/*! \brief ����ĳһ�����ϣ�ˮƽ/��ֵ�����ж�������/��Сֵ�����ж���ķ�Χ����domain����ָ����*/
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

/*! \brief ɾ�����е����ж���*/
void ChartDataSet::deleteAllSeries()
{
    foreach (QAbstractSeries *s , m_seriesList){
        removeSeries(s);
        s->deleteLater();
    }
    Q_ASSERT(m_seriesList.count() == 0);
}

/*! \brief ɾ�����е����������*/
void ChartDataSet::deleteAllAxes()
{
    foreach (QAbstractAxis *a , m_axisList){
        removeAxis(a);
        a->deleteLater();
    }
    Q_ASSERT(m_axisList.count() == 0);
}

/*! \brief �������Ŵ���ָ������*/
void ChartDataSet::zoomInDomain(const QRectF &rect)
{
    QList<AbstractDomain*> domains;
    // ����domain�ź�
    foreach(QAbstractSeries *s, m_seriesList) {
        AbstractDomain* domain = s->d_ptr->domain();
        s->d_ptr->m_domain->blockRangeSignals(true);
        domains<<domain;
    }

    // ��ÿ��domain�������Ŵ���ָ������
    foreach(AbstractDomain *domain, domains)
        domain->zoomIn(rect);

    // �ָ�domain���½����ź�
    foreach(AbstractDomain *domain, domains)
        domain->blockRangeSignals(false);
}

/*! \brief Զ�루��С����ĳһ����*/
void ChartDataSet::zoomOutDomain(const QRectF &rect)
{
    QList<AbstractDomain*> domains;
    // ����domain�ź�
    foreach(QAbstractSeries *s, m_seriesList) {
        AbstractDomain* domain = s->d_ptr->domain();
        s->d_ptr->m_domain->blockRangeSignals(true);
        domains<<domain;
    }

    // ��ÿ��domainԶ�루��С����ָ������
    foreach(AbstractDomain *domain, domains)
        domain->zoomOut(rect);

    // �ָ�domain���½����ź�
    foreach(AbstractDomain *domain, domains)
        domain->blockRangeSignals(false);
}

/*! \brief ���÷�������*/
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

/*! \return �Ƿ����˷�����*/
bool ChartDataSet::isZoomedDomain()
{
    foreach (QAbstractSeries *s, m_seriesList) {
        if (s->d_ptr->domain()->isZoomed())
            return true;
    }
    return false;
}

/*! \brief ��ָ��λ�ƹ�������*/
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

/*! \brief �����ж���������λӳ��Ϊ��ֵ��*/
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

/*! \brief �����ж������ֵλӳ��Ϊ��λ��*/
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

/*! \return �洢���������б�*/
QList<QAbstractAxis *> ChartDataSet::axes() const
{
   return m_axisList;
}

/*! \return �洢�����ж����б�*/
QList<QAbstractSeries *> ChartDataSet::series() const
{
    return m_seriesList;
}

/*! \brief ��������������ѡ����ʵ���domain���͡�*/
AbstractDomain::DomainType ChartDataSet::selectDomain(QList<QAbstractAxis *> axes)
{
    enum Type {
        Undefined = 0,
        LogType = 0x1,
        ValueType = 0x2
    };

    int horizontal(Undefined);
    int vertical(Undefined);

    // ���δ����ͼ�������ٶ�Ϊֱ������ͼ��
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
/*! \brief ����ָ����domain����\a type �����Ӧ��domain����*/
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
