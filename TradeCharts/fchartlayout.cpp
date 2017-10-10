#include "fchartlayout.h"
#include <ChartPresenter>
#include <ChartTitle>
#include <ChartAxisElement>
#include <QDebug>
#include <QtMath>

static const qreal maxAxisPortion = 0.4;
static const qreal golden_ratio = 0.4;

FChartLayout::FChartLayout(ChartPresenter *presenter):
    CartesianChartLayout(presenter)
{

}

void FChartLayout::setGeometry(const QRectF &rect)
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

        contentGeometry = calculateLegendAndAxisGeometry(contentGeometry, legend, axes);

        // 剩余的内容区域为ChartPresenter管理区域。
        m_presenter->setGeometry(contentGeometry);
        if (m_presenter->chart()->chartType() == QChart::ChartTypeCartesian)
            static_cast<QGraphicsRectItem *>(m_presenter->plotAreaElement())->setRect(contentGeometry);
        else
            static_cast<QGraphicsEllipseItem *>(m_presenter->plotAreaElement())->setRect(contentGeometry);
    }

    QGraphicsLayout::setGeometry(rect);
}

QRectF FChartLayout::calculateLegendAndAxisGeometry(const QRectF &geometry, QLegend *legend, const QList<ChartAxisElement *> &axes) const
{
    QSizeF size = legend->effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, -1));
    QRectF legendRect;

    QSizeF left(0,0);
    QSizeF minLeft(0,0);
    QSizeF right(0,0);
    QSizeF minRight(0,0);
    QSizeF bottom(0,0);
    QSizeF minBottom(0,0);
    QSizeF top(0,0);
    QSizeF minTop(0,0);
    QSizeF labelExtents(0,0);
    int leftCount = 0;
    int rightCount = 0;
    int topCount = 0;
    int bottomCount = 0;

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
           labelExtents.setHeight(qMax(size.height(), labelExtents.height()));
           leftCount++;
           break;
        case Qt::AlignRight:
            right.setWidth(right.width()+size.width());
            right.setHeight(qMax(right.height(),size.height()));
            minRight.setWidth(minRight.width()+minSize.width());
            minRight.setHeight(qMax(minRight.height(),minSize.height()));
            labelExtents.setHeight(qMax(size.height(), labelExtents.height()));
            rightCount++;
            break;
        case Qt::AlignTop:
            top.setWidth(qMax(top.width(),size.width()));
            top.setHeight(top.height()+size.height());
            minTop.setWidth(qMax(minTop.width(),minSize.width()));
            minTop.setHeight(minTop.height()+minSize.height());
            labelExtents.setWidth(qMax(size.width(), labelExtents.width()));
            topCount++;
            break;
        case Qt::AlignBottom:
            bottom.setWidth(qMax(bottom.width(), size.width()));
            bottom.setHeight(bottom.height() + size.height());
            minBottom.setWidth(qMax(minBottom.width(),minSize.width()));
            minBottom.setHeight(minBottom.height() + minSize.height());
            labelExtents.setWidth(qMax(size.width(), labelExtents.width()));
            bottomCount++;
            break;
        default:
            qWarning()<<"Axis is without alignment !";
            break;
        }
    }

    qreal totalVerticalAxes = leftCount + rightCount;
    // 左边所有坐标轴所占宽度的压缩比率。
    qreal leftSqueezeRatio = 1.0;
    // 右边所有坐标轴所占宽度的压缩比率。
    qreal rightSqueezeRatio = 1.0;
    // 每个坐标轴的平均宽度（垂直坐标轴方向上的尺寸大小）。
    qreal vratio = 0;

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
    qreal topSqueezeRatio = 1.0;
    // 底部所有坐标轴所占宽度的压缩比率。
    qreal bottomSqueezeRatio = 1.0;
    // 每个坐标轴的平均宽度（垂直坐标轴方向上的尺寸大小）。
    qreal hratio = 0;

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
    qreal minHeight = qMax(minLeft.height(),minRight.height()) + 1;
    qreal minWidth = qMax(minTop.width(),minBottom.width()) + 1;

    // 确保有足够的空间留给第一个和最后一个刻度标签。
    left.setWidth(qMax(labelExtents.width(), left.width()));
    right.setWidth(qMax(labelExtents.width(), right.width()));
    top.setHeight(qMax(labelExtents.height(), top.height()));
    bottom.setHeight(qMax(labelExtents.height(), bottom.height()));

    // 纵轴的宽度至少应为水平方向所有坐标轴所占宽度的一半。
    // 横轴的宽度至少应为垂直方向所有坐标轴所占宽度的一半。
    // 最终确定的中央绘图区域。
    QRectF chartWithLegendRect = geometry.adjusted(qMax(left.width(),minWidth/2), qMax(top.height(), minHeight/2),
                                                   -qMax(right.width(),minWidth/2),-qMax(bottom.height(),minHeight/2));
    QRectF chartRect = chartWithLegendRect;
    if (legend->isVisible()) {
        switch (legend->alignment()) {
        case Qt::AlignTop: {
            legendRect = QRectF(chartWithLegendRect.topLeft(), QSizeF(chartWithLegendRect.width(), size.height()));
            chartRect = chartWithLegendRect.adjusted(0, legendRect.height(), 0, 0);
            break;
        }
        case Qt::AlignBottom: {
            legendRect = QRectF(QPointF(chartWithLegendRect.left(), chartWithLegendRect.bottom() - size.height()),
                                QSizeF(chartWithLegendRect.width(), size.height()));
            chartRect = chartWithLegendRect.adjusted(0, 0, 0, -legendRect.height());
            break;
        }
        case Qt::AlignLeft: {
            qreal width = qMin(size.width(), chartWithLegendRect.width() * golden_ratio);
            legendRect = QRectF(chartWithLegendRect.topLeft(), QSizeF(width, chartWithLegendRect.height()));
            chartRect = chartWithLegendRect.adjusted(width, 0, 0, 0);
            break;
        }
        case Qt::AlignRight: {
            qreal width = qMin(size.width(), chartWithLegendRect.width() * golden_ratio);
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

    legend->setGeometry(legendRect);

    // 由于同一位置可能有多个坐标轴，所以需要记录同一位置每个坐标轴的偏移量（这是一个累积量）。
    qreal leftOffset = 0;
    qreal rightOffset = 0;
    qreal topOffset = 0;
    qreal bottomOffset = 0;

    foreach (ChartAxisElement *axis , axes) {
        if (!axis->isVisible())
            continue;

        QSizeF size = axis->effectiveSizeHint(Qt::PreferredSize);

        switch(axis->axis()->alignment()) {
        case Qt::AlignLeft: {
            qreal width = size.width();
            if (leftSqueezeRatio < 1.0)
                width *= leftSqueezeRatio;
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
            if (rightSqueezeRatio < 1.0)
                width *= rightSqueezeRatio;

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
            if (topSqueezeRatio < 1.0)
                height *= topSqueezeRatio;

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
            if (bottomSqueezeRatio < 1.0)
                height *= bottomSqueezeRatio;

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

    return chartRect;
}
