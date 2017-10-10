#include "fpanellayout.h"
#include "fchartpresenter.h"
#include <ChartTitle>
#include <QDebug>
#include <QtAlgorithms>
#include <ChartAxisElement>
#include <ChartBackground>

SCHARTS_BEGIN_NAMESPACE

static const qreal maxAxisPortion = 0.4;
static const qreal golden_ratio = 0.4;

FPanelLayout::FPanelLayout(FChartPresenter *presenter):
    CartesianChartLayout(presenter)
{

}

void FPanelLayout::setGeometry(const QRectF &rect)
{
    if (!rect.isValid())
        return;

    if (presenter()->chart()->isVisible() && presenter()->presenters().size() != 0) {
        QVector<ChartPresenter*> presenters = presenter()->presenters();

        ChartPresenter* itemPresenter = presenter();
        ChartTitle *title = m_presenter->titleElement();
        QList<ChartAxisElement *> axes = itemPresenter->axisItems();
        QLegend *legend = itemPresenter->legend();
        ChartBackground *background = itemPresenter->backgroundElement();

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

        // 分配每个QChart的显示区域。
        QVector<qreal> ratios;
        QVector<ChartPresenter*> visibleCharts;
        for (int i = 0; i < presenters.size(); i++) {
            if (presenters[i]->chart()->isVisible()) {
                visibleCharts.append(presenters[i]);
                ratios.append(1.0);
            }
        }
        calculateItemChartGeometry(contentGeometry, visibleCharts, ratios);

        // 剩余的内容区域为ChartPresenter管理区域。
        itemPresenter->setGeometry(contentGeometry);
        static_cast<QGraphicsRectItem *>(itemPresenter->plotAreaElement())->setRect(contentGeometry);
    }

    QGraphicsLayout::setGeometry(rect);
}

FChartPresenter *FPanelLayout::presenter()
{
    return (FChartPresenter*)m_presenter;
}

void FPanelLayout::calculateItemChartGeometry(const QRectF &geometry, const QVector<ChartPresenter*> &presenters, const QVector<qreal> &ratios)
{
    Q_ASSERT(ratios.size() == presenters.size());

    if (presenters.size() == 0) return;

    qreal total = 0.0;
    for (int i = 0; i < ratios.size(); i++) {
        total += ratios[i];
    }

    QVector<QSizeF> lefts(presenters.size(), QSizeF(0, 0));
    QVector<QSizeF> minLefts(presenters.size(), QSizeF(0, 0));
    QVector<QSizeF> rights(presenters.size(), QSizeF(0, 0));
    QVector<QSizeF> minRights(presenters.size(), QSizeF(0, 0));
    QVector<QSizeF> bottoms(presenters.size(), QSizeF(0, 0));
    QVector<QSizeF> minBottoms(presenters.size(), QSizeF(0, 0));
    QVector<QSizeF> tops(presenters.size(), QSizeF(0, 0));
    QVector<QSizeF> minTops(presenters.size(), QSizeF(0, 0));
    QVector<QSizeF> labelExtents(presenters.size(), QSizeF(0, 0));

    QVector<int> leftCounts(presenters.size(), 0);
    QVector<int> rightCounts(presenters.size(), 0);
    QVector<int> topCounts(presenters.size(), 0);
    QVector<int> bottomCounts(presenters.size(), 0);

    QVector<QSizeF> legendSizes(presenters.size());
    QVector<QRectF> legendRects(presenters.size());

    QVector<qreal> minHeights(presenters.size());
    QVector<qreal> minWidths(presenters.size());

    // 顶部所有坐标轴所占宽度的压缩比率。
    QVector<qreal> leftSqueezeRatios(presenters.size(), 1.0);
    // 底部所有坐标轴所占宽度的压缩比率。
    QVector<qreal> rightSqueezeRatios(presenters.size(), 1.0);
    // 每个坐标轴的平均宽度（垂直坐标轴方向上的尺寸大小）。
    QVector<qreal> vratios(presenters.size(), 0.0);

    QVector<qreal> topSqueezeRatios(presenters.size(), 1.0);
    // 底部所有坐标轴所占宽度的压缩比率。
    QVector<qreal> bottomSqueezeRatios(presenters.size(), 1.0);
    // 每个坐标轴的平均宽度（垂直坐标轴方向上的尺寸大小）。
    QVector<qreal> hratios(presenters.size(), 0.0);

    for (int i = 0; i < presenters.size(); i++) {
        QList<ChartAxisElement *> axes = presenters[i]->axisItems();
        QLegend *legend = presenters[i]->legend();

        legendSizes[i] = legend->effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, -1));

        QSizeF& legendSize = legendSizes[i];
        QRectF& legendRect = legendRects[i];

        QSizeF& left = lefts[i];
        QSizeF& minLeft = minLefts[i];
        QSizeF& right = rights[i];
        QSizeF& minRight = minRights[i];
        QSizeF& bottom = bottoms[i];
        QSizeF& minBottom = minBottoms[i];
        QSizeF& top = tops[i];
        QSizeF& minTop = minTops[i];
        QSizeF& labelExtent = labelExtents[i];
        int& leftCount = leftCounts[i];
        int& rightCount = rightCounts[i];
        int& topCount = topCounts[i];
        int& bottomCount = bottomCounts[i];

        foreach (ChartAxisElement *axis , axes) {
            if (!axis->isVisible())
                continue;

            QSizeF size = axis->effectiveSizeHint(Qt::PreferredSize);
            //this is used to get single thick font size
            QSizeF minSize = axis->effectiveSizeHint(Qt::MinimumSize);

            switch (axis->axis()->alignment()) {
            case Qt::AlignLeft:
               left.setWidth(left.width()+size.width());
               left.setHeight(qMax(left.height(),size.height()));
               minLeft.setWidth(minLeft.width()+minSize.width());
               minLeft.setHeight(qMax(minLeft.height(),minSize.height()));
               labelExtent.setHeight(qMax(size.height(), labelExtent.height()));
               leftCount++;
               break;
            case Qt::AlignRight:
                right.setWidth(right.width()+size.width());
                right.setHeight(qMax(right.height(),size.height()));
                minRight.setWidth(minRight.width()+minSize.width());
                minRight.setHeight(qMax(minRight.height(),minSize.height()));
                labelExtent.setHeight(qMax(size.height(), labelExtent.height()));
                rightCount++;
                break;
            case Qt::AlignTop:
                top.setWidth(qMax(top.width(),size.width()));
                top.setHeight(top.height()+size.height());
                minTop.setWidth(qMax(minTop.width(),minSize.width()));
                minTop.setHeight(minTop.height()+minSize.height());
                labelExtent.setWidth(qMax(size.width(), labelExtent.width()));
                topCount++;
                break;
            case Qt::AlignBottom:
                bottom.setWidth(qMax(bottom.width(), size.width()));
                bottom.setHeight(bottom.height() + size.height());
                minBottom.setWidth(qMax(minBottom.width(),minSize.width()));
                minBottom.setHeight(minBottom.height() + minSize.height());
                labelExtent.setWidth(qMax(size.width(), labelExtent.width()));
                bottomCount++;
                break;
            default:
                qWarning()<<"Axis is without alignment !";
                break;
            }
        }

        qreal totalVerticalAxes = leftCount + rightCount;
        // 左边所有坐标轴所占宽度的压缩比率。
        qreal& leftSqueezeRatio = leftSqueezeRatios[i];
        // 右边所有坐标轴所占宽度的压缩比率。
        qreal& rightSqueezeRatio = rightSqueezeRatios[i];
        // 每个坐标轴的平均宽度（垂直坐标轴方向上的尺寸大小）。
        qreal& vratio = vratios[i];

        if (totalVerticalAxes > 0)
            vratio = (maxAxisPortion * geometry.width()) / totalVerticalAxes;

        if (leftCount > 0) {
            int maxWidth = vratio * leftCount;
            if (left.width() > maxWidth) {
                leftSqueezeRatio = maxWidth / left.width();
                left.setWidth(maxWidth);
            }
        }
        if (rightCount > 0) {
            int maxWidth = vratio * rightCount;
            if (right.width() > maxWidth) {
                rightSqueezeRatio = maxWidth / right.width();
                right.setWidth(maxWidth);
            }
        }

        qreal totalHorizontalAxes = topCount + bottomCount;
        // 顶部所有坐标轴所占宽度的压缩比率。
        qreal& topSqueezeRatio = topSqueezeRatios[i];
        // 底部所有坐标轴所占宽度的压缩比率。
        qreal& bottomSqueezeRatio = bottomSqueezeRatios[i];
        // 每个坐标轴的平均宽度（垂直坐标轴方向上的尺寸大小）。
        qreal& hratio = hratios[i];

        if (totalHorizontalAxes > 0)
            hratio = (maxAxisPortion * geometry.height()) / totalHorizontalAxes;

        if (topCount > 0) {
            int maxHeight = hratio * topCount;
            if (top.height() > maxHeight) {
                topSqueezeRatio = maxHeight / top.height();
                top.setHeight(maxHeight);
            }
        }
        if (bottomCount > 0) {
            int maxHeight = hratio * bottomCount;
            if (bottom.height() > maxHeight) {
                bottomSqueezeRatio = maxHeight / bottom.height();
                bottom.setHeight(maxHeight);
            }
        }

        // 每个方向上两边坐标轴所占长度应该相等。
        minHeights[i] = qMax(minLeft.height(),minRight.height()) + 1;
        minWidths[i] = qMax(minTop.width(),minBottom.width()) + 1;

        // 确保有足够的空间留给第一个和最后一个刻度标签。同时，为了尽可能地压缩中间图表的空白区域，只给第一个和最后
        // 一个 QChart 留出刻度标签的位置。
        //! \todo 是否应该只压缩有legend可见并且将垂直方向相邻的两个坐标轴隔开的区域？如果legend不可见，应该留出空
        //! 白区域。
        left.setWidth(qMax(labelExtent.width(), left.width()));
        right.setWidth(qMax(labelExtent.width(), right.width()));
        if (i == 0 && !presenters[0]->legend()->isVisible()) top.setHeight(qMax(labelExtent.height(), top.height()));
        if (i == presenters.size() - 1) bottom.setHeight(qMax(labelExtent.height(), bottom.height()));
    }

    qreal maxLeft = lefts[0].width();
    qreal maxRight = rights[0].width();
    for (int i = 1; i < lefts.size(); i++){
        maxLeft = qMax(lefts[i].width(), maxLeft);
        maxRight = qMax(rights[i].width(), maxRight);
    }

    QVector<QRectF> chartWithLegendRects(presenters.size());
    QVector<QRectF> chartRects(presenters.size());

    QPointF topLeft = geometry.topLeft();
    topLeft.setX(geometry.x() + maxLeft);
    for (int i = 0; i < presenters.size(); i++) {
        QRectF& chartWithLegendRect = chartWithLegendRects[i];
        QRectF& chartRect = chartRects[i];
        QRectF& legendRect = legendRects[i];
        QSizeF& legendSize = legendSizes[i];

        chartWithLegendRect.setRect(topLeft.x(), topLeft.y() + tops[i].height(),
                                    geometry.width() - maxLeft - maxRight,
                                    ratios[i] / total * geometry.height() - tops[i].height() - bottoms[i].height());

        chartRect = chartWithLegendRect;

        if (presenters[i]->legend()->isVisible()) {
            switch (presenters[i]->legend()->alignment()) {
            case Qt::AlignTop: {
                legendRect = QRectF(chartWithLegendRect.topLeft(), QSizeF(chartWithLegendRect.width(), legendSize.height()));
                chartRect = chartWithLegendRect.adjusted(0, legendRect.height(), 0, 0);
                break;
            }
            case Qt::AlignBottom: {
                legendRect = QRectF(QPointF(chartWithLegendRect.left(), chartWithLegendRect.bottom() - legendSize.height()),
                                    QSizeF(chartWithLegendRect.width(), legendSize.height()));
                chartRect = chartWithLegendRect.adjusted(0, 0, 0, -legendRect.height());
                break;
            }
            case Qt::AlignLeft: {
                qreal width = qMin(legendSize.width(), chartWithLegendRect.width() * golden_ratio);
                legendRect = QRectF(chartWithLegendRect.topLeft(), QSizeF(width, chartWithLegendRect.height()));
                chartRect = chartWithLegendRect.adjusted(width, 0, 0, 0);
                break;
            }
            case Qt::AlignRight: {
                qreal width = qMin(legendSize.width(), chartWithLegendRect.width() * golden_ratio);
                legendRect = QRectF(QPointF(chartWithLegendRect.right() - width, chartWithLegendRect.top()),
                                    QSizeF(width, chartWithLegendRect.height()));
                chartRect = chartWithLegendRect.adjusted(0, 0, -width, 0);
                break;
            }
            default: {
                legendRect = QRectF(0, 0, 0, 0);
                chartRect = chartWithLegendRect;
                break;
            }
            }
        } else {
            legendRect = QRectF(0, 0, 0, 0);
        }

        presenters[i]->backgroundElement()->setRect(chartRect);
        presenters[i]->legend()->setGeometry(legendRect);

        // 设置坐标轴显示区域
        qreal leftOffset = 0;
        qreal rightOffset = 0;
        qreal topOffset = 0;
        qreal bottomOffset = 0;

        QList<ChartAxisElement *> axes = presenters[i]->axisItems();
        QLegend* legend = presenters[i]->legend();
        foreach (ChartAxisElement *axis , axes) {
            if (!axis->isVisible())
                continue;

            QSizeF size = axis->effectiveSizeHint(Qt::PreferredSize);

            switch(axis->axis()->alignment()) {
            case Qt::AlignLeft: {
                qreal width = size.width();
                if (leftSqueezeRatios[i] < 1.0)
                    width *= leftSqueezeRatios[i];
                leftOffset+=width;

                switch(legend->alignment()) {
                case Qt::AlignLeft:
                case Qt::AlignRight:
                    axis->setGeometry(QRect(chartWithLegendRect.left()-leftOffset, geometry.top(),width, geometry.bottom()),chartRect);
                    break;
                case Qt::AlignTop:
                    axis->setGeometry(QRect(chartWithLegendRect.left()-leftOffset, geometry.top(),width, geometry.bottom()),chartRect);
                    break;
                case Qt::AlignBottom:
                    axis->setGeometry(QRect(chartWithLegendRect.left()-leftOffset, geometry.top(),width, geometry.bottom()),chartRect);
                    break;
                }
                break;
            }
            case Qt::AlignRight: {
                qreal width = size.width();
                if (rightSqueezeRatios[i] < 1.0)
                    width *= rightSqueezeRatios[i];

                switch(legend->alignment()) {
                case Qt::AlignLeft:
                case Qt::AlignRight:
                    axis->setGeometry(QRect(chartWithLegendRect.right()+rightOffset, geometry.top(),width,geometry.bottom()),chartRect);
                    break;
                case Qt::AlignTop:
                    axis->setGeometry(QRect(chartWithLegendRect.right()+rightOffset, chartRect.top(),width, geometry.bottom()),chartRect);
                    break;
                case Qt::AlignBottom:
                    axis->setGeometry(QRect(chartWithLegendRect.right()+rightOffset, geometry.top(),width, chartRect.bottom()),chartRect);
                    break;
                }
                rightOffset+=width;
                break;
            }
            case Qt::AlignTop: {
                qreal height = size.height();
                if (topSqueezeRatios[i] < 1.0)
                    height *= topSqueezeRatios[i];

                switch(legend->alignment()) {
                case Qt::AlignLeft:
                    axis->setGeometry(QRect(geometry.left(), chartWithLegendRect.top() - topOffset - height, geometry.width(), height), chartRect);
                    break;
                case Qt::AlignRight:
                    axis->setGeometry(QRect(geometry.left(), chartWithLegendRect.top() - topOffset - height, geometry.width(), height), chartRect);
                    break;
                case Qt::AlignTop:
                case Qt::AlignBottom:
                    axis->setGeometry(QRect(geometry.left(), chartWithLegendRect.top() - topOffset - height, geometry.width(), height), chartRect);
                    break;
                }
                topOffset += height;
                break;
            }
            case Qt::AlignBottom: {
                qreal height = size.height();
                if (bottomSqueezeRatios[i] < 1.0)
                    height *= bottomSqueezeRatios[i];

                switch(legend->alignment()) {
                case Qt::AlignLeft:
                    axis->setGeometry(QRect(geometry.left(), chartWithLegendRect.bottom() + bottomOffset, geometry.width(), height), chartRect);
                    break;
                case Qt::AlignRight:
                    axis->setGeometry(QRect(geometry.left(), chartWithLegendRect.bottom() + bottomOffset, geometry.width(), height), chartRect);
                    break;
                case Qt::AlignTop:
                case Qt::AlignBottom:
                    axis->setGeometry(QRect(geometry.left(), chartWithLegendRect.bottom() + bottomOffset, geometry.width(), height), chartRect);
                    break;
                }
                bottomOffset += height;
                break;
            }
            }          
        }

        // 剩余的内容区域为ChartPresenter管理区域。
        presenters[i]->setGeometry(chartRect);
        static_cast<QGraphicsRectItem *>(presenters[i]->plotAreaElement())->setRect(chartRect);

        topLeft.setY(topLeft.y() + ratios[i] / total * geometry.height());
    }
}

SCHARTS_END_NAMESPACE
