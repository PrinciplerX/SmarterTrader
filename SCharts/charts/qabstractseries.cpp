#include "../qabstractseries.h"
#include <qabstractseries_p.h>
#include <chartdataset_p.h>
#include <QChart>
#include <qchart_p.h>
#include <chartitem_p.h>
#include <xydomain_p.h>
#include <xlogydomain_p.h>
#include <logxydomain_p.h>
#include <logxlogydomain_p.h>

SCHARTS_BEGIN_NAMESPACE

/*!
 * \class QAbstractSeries
 * \inmodule Qt Charts
 * \brief Base class for all Qt Chart series.
 *
 * ͨ��ʹ���ɸ����������ض��������Ͷ�����ֱ��ʹ�øû��ࡣ
 * \sa QXYSeries, QLineSeries, QSplineSeries, QScatterSeries, QAreaSeries, QAbstractBarSeries, QStackedBarSeries,
 * QPercentBarSeries, QPieSeries
 */

/*!
 * \qmltype AbstractSeries
 * \instantiates QAbstractSeries
 * \inqmlmodule QtCharts
 *
 * \brief ����Qt Chart���ж����ͨ�û��ࡣ
 *
 * AbstractSeries���������еĻ��ࡣ
 * ���಻�����û�ʵ������
 */

/*!
 * \enum QAbstractSeries::SeriesType
 *
 * ���ж�������͡�
 *
 * \value SeriesTypeLine
 * \value SeriesTypeArea
 * \value SeriesTypeBar
 * \value SeriesTypeStackedBar
 * \value SeriesTypePercentBar
 * \value SeriesTypePie
 * \value SeriesTypeScatter
 * \value SeriesTypeSpline
 * \value SeriesTypeHorizontalBar
 * \value SeriesTypeHorizontalStackedBar
 * \value SeriesTypeHorizontalPercentBar
 * \value SeriesTypeBoxPlot
 */

/*!
 * \property QAbstractSeries::type
 * ���ж�������͡�
 */
/*!
 * \qmlproperty ChartView.SeriesType AbstractSeries::type
 * ���ж�������͡�
 */

/*!
 * \property QAbstractSeries::name
 * \brief ���е����ơ������ƻ���ʾ�����е�ͼ���ϣ�����֧��html��ʽ��
 */
/*!
 * \qmlproperty string AbstractSeries::name
 * ���е����ơ������ƻ���ʾ�����е�ͼ���ϣ�����֧��html��ʽ��
 */

/*!
 * \fn void QAbstractSeries::nameChanged()
 * ���������Ƹı�ʱ���������źš�
 */
/*!
 * \qmlsignal AbstractSeries::onNameChanged()
 * ���������Ƹı�ʱ���������źš�
 */

/*!
 * \property QAbstractSeries::visible
 * \brief �����ж����Ƿ�ɼ���Ĭ��Ϊtrue��
 */
/*!
 * \qmlproperty bool AbstractSeries::visible
 * �����ж����Ƿ�ɼ���Ĭ��Ϊtrue��
 */

/*!
 * \fn void QAbstractSeries::visibleChanged()
 * �����пɼ��Ըı�ʱ���������źš�
 */
/*!
 * \qmlsignal AbstractSeries::onVisibleChanged()
 * �����пɼ��Ըı�ʱ���������źš�
 */

/*!
 * \property QAbstractSeries::opacity
 * \brief ���ж���Ĳ�͸���ȡ�
 *
 * Ĭ������²�͸����Ϊ 1.0����Ч����ֵ��Χ�Ǵ� 0.0 ����ȫ͸���� �� 1.0 ����͸������
 */
/*!
 * \qmlproperty real AbstractSeries::opacity
 * ���ж���Ĳ�͸���ȡ�Ĭ������²�͸����Ϊ 1.0��
 * ��Ч����ֵ��Χ�Ǵ� 0.0 ����ȫ͸���� �� 1.0 ����͸������
 */

/*!
 * \fn void QAbstractSeries::opacityChanged()
 * �����ж����͸���ȷ����仯ʱ���������źš�
 */
/*!
 * \qmlsignal AbstractSeries::onOpacityChanged()
 * �����ж����͸���ȷ����仯ʱ���������źš�
 */

/*!
 * \property QAbstractSeries::useOpenGL
 * \brief ָ�����л����Ƿ�ʹ��OpenGL���١�
 *
 * OpenGL�������н�֧�� QLineSeries �� QScatterSeries������ QAreaSeries �߽����е����������޷�ʹ��OpenGL���١���
 * ͼ����������κ�ʹ��OpenGL���Ƶ�����ʱ��������ͼ���������Ķ��˴���һ��͸���� QOpenGLWidget �������ٵ����в���
 * �����ڵײ�� QGraphicsView �����ǻ����ڴ����� QOpenGLWidget �ϡ�
 *
 * ʹ��OpenGL�������л��ƴ�������������ȡ���ڵײ�Ӳ�������Ǵ���������Ч�������ԡ����磬��һ̨��׼���������ϣ���
 * ��ĳ����������OpenGL����ͨ���ܹ��ڲ�����֡��������¶���Ⱦ���ٱ��ĵ�����
 * ͼ���С��֡����Ӱ���С��
 *
 * OpenGL�����л��Ƶļ�����Ҫ��������Ҫ���ٻ��ƴ����������������Ϊ�����Ч�������Ż���������δ�����ٵ����ж���ȱ
 * ����������ԣ�
 *
 * \list
 * \li �������в�֧�����ж�����
 * \li �������в�֧�ֿ���ݡ�
 * \li �������в�֧�ֵ��ǩ��Point labels����
 * \li �������н�����ӻ��ʷ��ͱ�ǣ�marker����״��
 *     ��֧��ʵ���߶κ���ͨ��ɢ�㡣
 *     ɢ�������Բ�λ��߾��Σ�ȡ���ڵײ��ͼ��Ӳ����������
 * \li ������ͼ��֧�ּ������С�
 * \li �������в�֧������¼���
 * \li ��ʹ�ü�������ʱ���Ƽ�����ͼ����Ӱ����͸����ͼ������ɫ����Ϊ����ܻ���������֡�ʡ�
 * \endlist
 *
 * ��Щ���ӵ�������Ҫ�ǻ��ڼ���������ͼ�����Ķ����ؼ����Ƶ���ʵ��
 *
 * \list
 * \li �������ͼ���������κΰ����˼������е�ͼ��Ԫ��I���������н�������ЩԪ��֮�ϻ��ơ�
 * \li Ϊ��ʹ QOpenGLWidget ����͸����������λ�ڶѵ��������ؼ��Ķ����� ����ζ������ʹ�ü�������ʱ������������������
 *     ��ͼ��Ŀռ䡣
 * \li �� graphics scene �ж�������� graphics view ʱ����֧�ּ������С�
 * \li ��ͼ��û�г������� graphics view �����з�Ĭ�ϵĳߴ��λ�ã�geometry��ʱ����֧�ּ������С����磬 ʹ�ù�������
 *     ����ͼ���߸� graphics view ��ӱ任��transformation�����¼������л��Ƶ����ͼ��Ĵ���λ���ϡ�
 * \endlist
 *
 * Ĭ��ֵΪ \c{false}.
*/
/*!
 * \qmlproperty bool AbstractSeries::useOpenGL
 * ָ�����л����Ƿ�ʹ��OpenGL���١�
 *
 * OpenGL�������н�֧�� QLineSeries �� QScatterSeries������ QAreaSeries �߽����е����������޷�ʹ��OpenGL���١���
 * ͼ����������κ�ʹ��OpenGL���Ƶ�����ʱ��������ͼ���������Ķ��˴���һ��͸���� QOpenGLWidget �������ٵ����в���
 * �����ڵײ�� QGraphicsView �����ǻ����ڴ����� QOpenGLWidget �ϡ�
 *
 * ʹ��OpenGL�������л��ƴ�������������ȡ���ڵײ�Ӳ�������Ǵ���������Ч�������ԡ����磬��һ̨��׼���������ϣ���
 * ��ĳ����������OpenGL����ͨ���ܹ��ڲ�����֡��������¶���Ⱦ���ٱ��ĵ�����
 * ͼ���С��֡����Ӱ���С��
 *
 * OpenGL�����л��Ƶļ�����Ҫ��������Ҫ���ٻ��ƴ����������������Ϊ�����Ч�������Ż���������δ�����ٵ����ж���ȱ
 * ����������ԣ�
 *
 * \list
 * \li �������в�֧�����ж�����
 * \li �������в�֧�ֿ���ݡ�
 * \li �������в�֧�ֵ��ǩ��Point labels����
 * \li �������н�����ӻ��ʷ��ͱ�ǣ�marker����״��
 *     ��֧��ʵ���߶κ���ͨ��ɢ�㡣
 *     ɢ�������Բ�λ��߾��Σ�ȡ���ڵײ��ͼ��Ӳ����������
 * \li ������ͼ��֧�ּ������С�
 * \li �������в�֧������¼���
 * \li ��ʹ�ü�������ʱ���Ƽ�����ͼ����Ӱ����͸����ͼ������ɫ����Ϊ����ܻ���������֡�ʡ�
 * \endlist
 *
 * Ĭ��ֵΪ \c{false}.
 */

/*!
 * \fn void QAbstractSeries::useOpenGLChanged()
 * �� useOpenGL ����ֵ�����仯ʱ���������źš�
 */
/*!
 * \qmlsignal AbstractSeries::onUseOpenGLChanged()
 * �� useOpenGL ����ֵ�����仯ʱ���������źš�
 */

/*!
 * \internal
 * \brief ����һ�� QAbstractSeries ������Ϊ\a parent ���Ӷ���
 */
QAbstractSeries::QAbstractSeries(QAbstractSeriesPrivate &d, QObject *parent) :
    QObject(parent),
    d_ptr(&d)
{
}

/*!
 * \brief ͼ�����е�������������
 */
QAbstractSeries::~QAbstractSeries()
{
    if (d_ptr->m_chart)
        qFatal("Series still bound to a chart when destroyed!");
}

void QAbstractSeries::setName(const QString &name)
{
    if (name != d_ptr->m_name) {
        d_ptr->m_name = name;
        emit nameChanged();
    }
}

QString QAbstractSeries::name() const
{
    return d_ptr->m_name;
}

void QAbstractSeries::setVisible(bool visible)
{
    if (visible != d_ptr->m_visible) {
        d_ptr->m_visible = visible;
        emit visibleChanged();
    }
}

bool QAbstractSeries::isVisible() const
{
    return d_ptr->m_visible;
}

qreal QAbstractSeries::opacity() const
{
    return d_ptr->m_opacity;
}

void QAbstractSeries::setOpacity(qreal opacity)
{
    if (opacity != d_ptr->m_opacity) {
        d_ptr->m_opacity = opacity;
        emit opacityChanged();
    }
}

void QAbstractSeries::setUseOpenGL(bool enable)
{
#ifdef QT_NO_OPENGL
    Q_UNUSED(enable)
#else
    bool polarChart = d_ptr->m_chart && d_ptr->m_chart->chartType() == QChart::ChartTypePolar;
    bool supportedSeries = (type() == SeriesTypeLine || type() == SeriesTypeScatter);
    if ((!enable || !d_ptr->m_blockOpenGL)
            && supportedSeries
            && enable != d_ptr->m_useOpenGL
            && (!enable || !polarChart)) {
        d_ptr->m_useOpenGL = enable;
        emit useOpenGLChanged();
    }
#endif
}

bool QAbstractSeries::useOpenGL() const
{
    return d_ptr->m_useOpenGL;
}

/*!
 * \brief ��������������ͼ�����
 *
 * �����м��뵽ͼ����ʱ�Զ����ã������д�ͼ�����Ƴ�ʱ�Զ�ȡ�����á�
 */
QChart *QAbstractSeries::chart() const
{
    return d_ptr->m_chart;
}

/*!
 * \brief �����еĿɼ�����Ϊtrue��
 * \sa setVisible(), isVisible()
 */
void QAbstractSeries::show()
{
    setVisible(true);
}

/*!
 * \brief �����еĿɼ�����Ϊtrue��
 * \sa setVisible(), isVisible()
 */
void QAbstractSeries::hide()
{
    setVisible(false);
}

/*!
 * ��������\a axis �����������С�
 * \return �������������ɹ�������true�����򷵻�false��
 * \note If multiple axes of same orientation are attached to same series,they will have same min/max ranges.
 * \sa QChart::addAxis(), QChart::createDefaultAxes()
 */
bool QAbstractSeries::attachAxis(QAbstractAxis* axis)
{
    if(d_ptr->m_chart) {
        return d_ptr->m_chart->d_ptr->m_dataset->attachAxis(this, axis);
    } else {
        qWarning()<<"Series not in the chart. Please addSeries to chart first.";
        return false;
    }
}

/*!
 * ��������\a axis �����ж�����롣
 * \return ������������ɹ�������true�����򷵻�false��
 * \sa QChart::removeAxis()
 */
bool QAbstractSeries::detachAxis(QAbstractAxis* axis)
{
    if(d_ptr->m_chart) {
        return d_ptr->m_chart->d_ptr->m_dataset->detachAxis(this, axis);
    }
    else {
        qWarning()<<"Series not in the chart. Please addSeries to chart first.";
        return false;
    }
}

/*!
 * ����һ�������������ж�����������б���Щ������ͨ����һ��x�������һ��y�����ᣬ������ʹ
 * �� QPieSeries ��������δ�����κ������ᡣ
 * \sa attachAxis(), detachAxis()
 */
QList<QAbstractAxis*> QAbstractSeries::attachedAxes()
{
    return d_ptr->m_axes;
}

//===========================================================================================================
// �ڲ����� QAbstractSeriesPrivate �Ĵ��롣
//===========================================================================================================

QAbstractSeriesPrivate::QAbstractSeriesPrivate(QAbstractSeries *q)
    : q_ptr(q),
      m_chart(0),
      m_item(0),
      m_domain(new XYDomain()),
      m_visible(true),
      m_opacity(1.0),
      m_useOpenGL(false),
      m_blockOpenGL(false)
{
}

QAbstractSeriesPrivate::~QAbstractSeriesPrivate()
{
}

void QAbstractSeriesPrivate::setDomain(AbstractDomain* domain)
{
    Q_ASSERT(domain);
    if(m_domain.data()!=domain) {
        if(!m_item.isNull()) QObject::disconnect(m_domain.data(), SIGNAL(updated()), m_item.data(), SLOT(handleDomainUpdated()));
        m_domain.reset(domain);
        if(!m_item.isNull()) {
            QObject::connect(m_domain.data(), SIGNAL(updated()),m_item.data(), SLOT(handleDomainUpdated()));
            m_item->handleDomainUpdated();
        }
    }
}

void QAbstractSeriesPrivate::setPresenter(ChartPresenter *presenter)
{
    m_presenter = presenter;
}

ChartPresenter *QAbstractSeriesPrivate::presenter() const
{
    return m_presenter;
}

void QAbstractSeriesPrivate::initializeGraphics(QGraphicsItem* parent)
{
    Q_ASSERT(!m_item.isNull());
    Q_UNUSED(parent);
    QObject::connect(m_domain.data(), SIGNAL(updated()),m_item.data(), SLOT(handleDomainUpdated()));
}

void QAbstractSeriesPrivate::initializeAnimations(QChart::AnimationOptions options, int duration,
                                                  QEasingCurve &curve)
{
    Q_UNUSED(options);
    Q_UNUSED(duration);
    Q_UNUSED(curve);
}

bool QAbstractSeriesPrivate::reverseXAxis()
{
    bool reverseXAxis = false;
    if (m_axes.size() != 0 && !(m_chart->chartType() == QChart::ChartTypePolar)) {
        int i = 0;
        while (i < m_axes.size()) {
            if (m_axes.at(i)->orientation() == Qt::Horizontal && m_axes.at(i)->isReverse()) {
                reverseXAxis = true;
                break;
            }
            i++;
        }
    }

    return reverseXAxis;
}

bool QAbstractSeriesPrivate::reverseYAxis()
{
    bool reverseYAxis = false;
    if (m_axes.size() != 0 && !(m_chart->chartType() == QChart::ChartTypePolar)) {
        int i = 0;
        while (i < m_axes.size()) {
            if (m_axes.at(i)->orientation() == Qt::Vertical && m_axes.at(i)->isReverse()) {
                reverseYAxis = true;
                break;
            }
            i++;
        }
    }

    return reverseYAxis;
}

// This function can be used to explicitly block OpenGL use from some otherwise supported series,
// such as the line series used as edge series of an area series.
void QAbstractSeriesPrivate::setBlockOpenGL(bool enable)
{
    m_blockOpenGL = enable;
    if (enable)
        q_ptr->setUseOpenGL(false);
}

#include "moc_qabstractseries.cpp"
#include "moc_qabstractseries_p.cpp"

SCHARTS_END_NAMESPACE


