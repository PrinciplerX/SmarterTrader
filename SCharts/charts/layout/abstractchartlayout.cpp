#include <abstractchartlayout_p.h>
#include <chartpresenter_p.h>
#include <qlegend_p.h>
#include <chartaxiselement_p.h>
#include <charttitle_p.h>
#include <chartbackground_p.h>
#include <QtCore/QDebug>

SCHARTS_BEGIN_NAMESPACE

static const qreal golden_ratio = 0.4;

AbstractChartLayout::AbstractChartLayout(ChartPresenter *presenter)
    : m_presenter(presenter),
      m_margins(20, 20, 20, 20),
      m_minChartRect(0, 0, 20, 20)
{
}

AbstractChartLayout::~AbstractChartLayout()
{
}

void AbstractChartLayout::setGeometry(const QRectF &rect)
{
    if (!rect.isValid())
        return;

    if (m_presenter->chart()->isVisible()) {
        QList<ChartAxisElement *> axes = m_presenter->axisItems();
        ChartTitle *title = m_presenter->titleElement();
        QLegend *legend = m_presenter->legend();
        ChartBackground *background = m_presenter->backgroundElement();

        // ���ֺͱ���֮����һ��margins��contentMargins������rect��ȥ�ò��֡�
        QRectF contentGeometry = calculateBackgroundGeometry(rect, background);
        // ���������ݵı߽�֮����һ��margins��m_margins������contentGeometry�г�ȥ�ò��֡�
        contentGeometry = calculateContentGeometry(contentGeometry);

        // ��contentGeometry�г�ȥ��������
        if (title && title->isVisible())
            contentGeometry = calculateTitleGeometry(contentGeometry, title);

        // ��contentGeometry�г�ȥͼ������
        if (legend->isAttachedToChart() && legend->isVisible())
            contentGeometry = calculateLegendGeometry(contentGeometry, legend);

        // ��contentGeometry�г�ȥ���������򣬸ò����ɲ�ͬ���͵�ͼ���Զ��塣
        contentGeometry = calculateAxisGeometry(contentGeometry, axes);

        // ʣ�����������ΪChartPresenter��������
        m_presenter->setGeometry(contentGeometry);
        if (m_presenter->chart()->chartType() == QChart::ChartTypeCartesian)
            static_cast<QGraphicsRectItem *>(m_presenter->plotAreaElement())->setRect(contentGeometry);
        else
            static_cast<QGraphicsEllipseItem *>(m_presenter->plotAreaElement())->setRect(contentGeometry);
    }

    QGraphicsLayout::setGeometry(rect);
}

/*!
 * \brief ���ݸ�����\a geometry �������ݲ�����ռ����
 * \param geometry �������� + m_margins����ͬ�ڱ�������
 * \return ��������
 * \sa calculateBackgroundGeometry()
 */
QRectF AbstractChartLayout::calculateContentGeometry(const QRectF &geometry) const
{
    return geometry.adjusted(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());
}

/*!
 * \brief �ۼ����ݲ���ռ�õ���С����
 * \return �ۼӽ����
 */
QRectF AbstractChartLayout::calculateContentMinimum(const QRectF &minimum) const
{
    return  minimum.adjusted(0, 0, m_margins.left() + m_margins.right(), m_margins.top() + m_margins.bottom());
}

/*!
 * \brief ���ݸ����Ĳ���\a geometry ���㲢���±������ֵ����򡣱�������Ϊ\a geometry ��ȥ�հײ��֣�margins����ʣ�ಿ�֡�
 * \return ���������������� + m_margins����
 */
QRectF AbstractChartLayout::calculateBackgroundGeometry(const QRectF &geometry, ChartBackground *background) const
{
    qreal left;
    qreal top;
    qreal right;
    qreal bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRectF backgroundGeometry = geometry.adjusted(left, top, -right, -bottom);
    if (background)
        background->setRect(backgroundGeometry);
    return backgroundGeometry;
}

/*!
 * \brief �ۼӱ�������ռ�õ���С����
 * \return �ۼӽ����
 */
QRectF AbstractChartLayout::calculateBackgroundMinimum(const QRectF &minimum) const
{
    qreal left;
    qreal top;
    qreal right;
    qreal bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    return minimum.adjusted(0, 0, left + right, top + bottom);
}

/*!
 * \brief ���ݸ����Ĳ���\a geometry ���㲢����ͼ����legend�����ֵ�����
 * \return ��ͼ����ռ�����\a geometry�۳�����������
 */
QRectF AbstractChartLayout::calculateLegendGeometry(const QRectF &geometry, QLegend *legend) const
{
    QSizeF size = legend->effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, -1));
    QRectF legendRect;
    QRectF result;

    switch (legend->alignment()) {
    case Qt::AlignTop: {
        legendRect = QRectF(geometry.topLeft(), QSizeF(geometry.width(), size.height()));
        result = geometry.adjusted(0, legendRect.height(), 0, 0);
        break;
    }
    case Qt::AlignBottom: {
        legendRect = QRectF(QPointF(geometry.left(), geometry.bottom() - size.height()), QSizeF(geometry.width(), size.height()));
        result = geometry.adjusted(0, 0, 0, -legendRect.height());
        break;
    }
    case Qt::AlignLeft: {
        qreal width = qMin(size.width(), geometry.width() * golden_ratio);
        legendRect = QRectF(geometry.topLeft(), QSizeF(width, geometry.height()));
        result = geometry.adjusted(width, 0, 0, 0);
        break;
    }
    case Qt::AlignRight: {
        qreal width = qMin(size.width(), geometry.width() * golden_ratio);
        legendRect = QRectF(QPointF(geometry.right() - width, geometry.top()), QSizeF(width, geometry.height()));
        result = geometry.adjusted(0, 0, -width, 0);
        break;
    }
    default: {
        legendRect = QRectF(0, 0, 0, 0);
        result = geometry;
        break;
    }
    }

    legend->setGeometry(legendRect);

    return result;
}

/*!
 * \brief ����ͼ������ռ�õ���С����
 * \return ��ͼ����ռ�ռ���ӵ�\a minimum �к���������
 */
QRectF AbstractChartLayout::calculateLegendMinimum(const QRectF &geometry, QLegend *legend) const
{
    QSizeF minSize = legend->effectiveSizeHint(Qt::MinimumSize, QSizeF(-1, -1));
    return geometry.adjusted(0, 0, minSize.width(), minSize.height());
}

/*!
 * \brief ���ݸ�������������\a geometry ���㲢���±��ⲿ�ֵ���ռ����
 * \return �����ⲿ�������\a geometry �۳�����������
 * \sa calculateContentGeometry()
 */
QRectF AbstractChartLayout::calculateTitleGeometry(const QRectF &geometry, ChartTitle *title) const
{
    title->setGeometry(geometry);
    if (title->text().isEmpty()) {
        return geometry;
    } else {
        // ͨ��QPoint�����������������Ա���ĳЩ�����λ�ڱ�Ե�ĵ����ص㱻���С�
        QPointF center((geometry.center() - title->boundingRect().center()).toPoint());

        title->setPos(center.x(), title->pos().y());
        return geometry.adjusted(0, title->boundingRect().height() + 1, 0, 0);
    }
}

/*!
 * \brief ������ⲿ��ռ�õ���С����
 * \return ��������ռ�ռ���ӵ�\a minimum �к���������
 */
QRectF AbstractChartLayout::calculateTitleMinimum(const QRectF &minimum, ChartTitle *title) const
{
    QSizeF min = title->sizeHint(Qt::MinimumSize);
    return  minimum.adjusted(0, 0, min.width(), min.height());
}

QSizeF AbstractChartLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);
    if (which == Qt::MinimumSize) {
        QList<ChartAxisElement *> axes = m_presenter->axisItems();
        ChartTitle *title = m_presenter->titleElement();
        QLegend *legend = m_presenter->legend();
        QRectF minimumRect(0, 0, 0, 0);
        minimumRect = calculateBackgroundMinimum(minimumRect);
        minimumRect = calculateContentMinimum(minimumRect);
        minimumRect = calculateTitleMinimum(minimumRect, title);
        minimumRect = calculateLegendMinimum(minimumRect, legend);
        minimumRect = calculateAxisMinimum(minimumRect, axes);
        return  minimumRect.united(m_minChartRect).size().toSize();
    }
    return QSize(-1, -1);
}

void AbstractChartLayout::setMargins(const QMargins &margins)
{
    if (m_margins != margins) {
        m_margins = margins;
        updateGeometry();
    }
}

QMargins AbstractChartLayout::margins() const
{
    return m_margins;
}

SCHARTS_END_NAMESPACE
