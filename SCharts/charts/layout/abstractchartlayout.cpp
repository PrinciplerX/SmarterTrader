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

        // 布局和背景之间有一层margins（contentMargins），从rect除去该部分。
        QRectF contentGeometry = calculateBackgroundGeometry(rect, background);
        // 背景和内容的边界之间是一层margins（m_margins），从contentGeometry中除去该部分。
        contentGeometry = calculateContentGeometry(contentGeometry);

        // 从contentGeometry中除去标题区域。
        if (title && title->isVisible())
            contentGeometry = calculateTitleGeometry(contentGeometry, title);

        // 从contentGeometry中除去图例区域。
        if (legend->isAttachedToChart() && legend->isVisible())
            contentGeometry = calculateLegendGeometry(contentGeometry, legend);

        // 从contentGeometry中除去坐标轴区域，该部分由不同类型的图表自定义。
        contentGeometry = calculateAxisGeometry(contentGeometry, axes);

        // 剩余的内容区域为ChartPresenter管理区域。
        m_presenter->setGeometry(contentGeometry);
        if (m_presenter->chart()->chartType() == QChart::ChartTypeCartesian)
            static_cast<QGraphicsRectItem *>(m_presenter->plotAreaElement())->setRect(contentGeometry);
        else
            static_cast<QGraphicsEllipseItem *>(m_presenter->plotAreaElement())->setRect(contentGeometry);
    }

    QGraphicsLayout::setGeometry(rect);
}

/*!
 * \brief 根据给定的\a geometry 计算内容部分所占区域。
 * \param geometry 内容区域 + m_margins（等同于背景区域）
 * \return 内容区域。
 * \sa calculateBackgroundGeometry()
 */
QRectF AbstractChartLayout::calculateContentGeometry(const QRectF &geometry) const
{
    return geometry.adjusted(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());
}

/*!
 * \brief 累加内容部分占用的最小区域。
 * \return 累加结果。
 */
QRectF AbstractChartLayout::calculateContentMinimum(const QRectF &minimum) const
{
    return  minimum.adjusted(0, 0, m_margins.left() + m_margins.right(), m_margins.top() + m_margins.bottom());
}

/*!
 * \brief 根据给定的布局\a geometry 计算并更新背景部分的区域。背景区域为\a geometry 除去空白部分（margins）的剩余部分。
 * \return 背景区域（内容区域 + m_margins）。
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
 * \brief 累加背景部分占用的最小区域。
 * \return 累加结果。
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
 * \brief 根据给定的布局\a geometry 计算并更新图例（legend）部分的区域。
 * \return 将图例所占区域从\a geometry扣除后所得区域。
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
 * \brief 计算图例部分占用的最小区域。
 * \return 将图例所占空间添加到\a minimum 中后所得区域。
 */
QRectF AbstractChartLayout::calculateLegendMinimum(const QRectF &geometry, QLegend *legend) const
{
    QSizeF minSize = legend->effectiveSizeHint(Qt::MinimumSize, QSizeF(-1, -1));
    return geometry.adjusted(0, 0, minSize.width(), minSize.height());
}

/*!
 * \brief 根据给定的内容区域\a geometry 计算并更新标题部分的所占区域。
 * \return 将标题部分区域从\a geometry 扣除后所得区域。
 * \sa calculateContentGeometry()
 */
QRectF AbstractChartLayout::calculateTitleGeometry(const QRectF &geometry, ChartTitle *title) const
{
    title->setGeometry(geometry);
    if (title->text().isEmpty()) {
        return geometry;
    } else {
        // 通过QPoint四舍五入至整像素以避免某些情况下位于边缘的单像素点被剪切。
        QPointF center((geometry.center() - title->boundingRect().center()).toPoint());

        title->setPos(center.x(), title->pos().y());
        return geometry.adjusted(0, title->boundingRect().height() + 1, 0, 0);
    }
}

/*!
 * \brief 计算标题部分占用的最小区域。
 * \return 将标题所占空间添加到\a minimum 中后所得区域。
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
