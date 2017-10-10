#include <cartesianchartlayout_p.h>
#include <chartpresenter_p.h>
#include <chartaxiselement_p.h>
#include <QtCore/QDebug>

SCHARTS_BEGIN_NAMESPACE

//! \todo 图表的同一方向上所有坐标轴的宽度之和所占图表该方向尺寸的最大比例。
static const qreal maxAxisPortion = 0.4;

CartesianChartLayout::CartesianChartLayout(ChartPresenter *presenter)
    : AbstractChartLayout(presenter)
{
}

CartesianChartLayout::~CartesianChartLayout()
{
}

QRectF CartesianChartLayout::calculateAxisGeometry(const QRectF &geometry, const QList<ChartAxisElement *> &axes) const
{
    // 注意：在同一位置上可能叠加了多个坐标轴。定义的坐标轴大小均为同一位置所有坐标轴的累加大小。
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
    QRectF chartRect = geometry.adjusted(qMax(left.width(),minWidth/2), qMax(top.height(), minHeight/2),
                                         -qMax(right.width(),minWidth/2),-qMax(bottom.height(),minHeight/2));

    // 由于同一位置可能有多个坐标轴，所以需要记录同一位置每个坐标轴的偏移量（这是一个累积量）。
    qreal leftOffset = 0;
    qreal rightOffset = 0;
    qreal topOffset = 0;
    qreal bottomOffset = 0;

    foreach (ChartAxisElement *axis , axes) {

        if (!axis->isVisible())
            continue;

        QSizeF size = axis->effectiveSizeHint(Qt::PreferredSize);

        switch (axis->axis()->alignment()){
        case Qt::AlignLeft:{
            qreal width = size.width();
            if (leftSqueezeRatio < 1.0)
                width *= leftSqueezeRatio;
            leftOffset+=width;
            axis->setGeometry(QRect(chartRect.left()-leftOffset, geometry.top(),width, geometry.bottom()),chartRect);
            break;
        }
        case Qt::AlignRight:{
            qreal width = size.width();
            if (rightSqueezeRatio < 1.0)
                width *= rightSqueezeRatio;
            axis->setGeometry(QRect(chartRect.right()+rightOffset,geometry.top(),width,geometry.bottom()),chartRect);
            rightOffset+=width;
            break;
        }
        case Qt::AlignTop: {
            qreal height = size.height();
            if (topSqueezeRatio < 1.0)
                height *= topSqueezeRatio;
            axis->setGeometry(QRect(geometry.left(), chartRect.top() - topOffset - height, geometry.width(), height), chartRect);
            topOffset += height;
            break;
        }
        case Qt::AlignBottom:
            qreal height = size.height();
            if (bottomSqueezeRatio < 1.0)
                height *= bottomSqueezeRatio;
            axis->setGeometry(QRect(geometry.left(), chartRect.bottom() + bottomOffset, geometry.width(), height), chartRect);
            bottomOffset += height;
            break;
        }
    }

    return chartRect;
}

QRectF CartesianChartLayout::calculateAxisMinimum(const QRectF &minimum, const QList<ChartAxisElement *> &axes) const
{
    QSizeF left;
    QSizeF right;
    QSizeF bottom;
    QSizeF top;

    // 累加每个坐标轴的MinimumSizeHint。
    foreach (ChartAxisElement *axis, axes) {
        QSizeF size = axis->effectiveSizeHint(Qt::MinimumSize);

        if (!axis->isVisible())
            continue;

        switch (axis->axis()->alignment()) {
        case Qt::AlignLeft:
            left.setWidth(left.width() + size.width());
            //! \todo 为什么高度至少更新为前面的两倍？
            //! \warning 当第一次使用 left.height() 时，它为-1。
            left.setHeight(qMax(left.height() * 2, size.height()));
            break;
        case Qt::AlignRight:
            //! \todo 为什么高度至少更新为前面的两倍？
            //! \warning 当第一次使用 right.height() 时，它为-1。
            right.setWidth(right.width() + size.width());
            right.setHeight(qMax(right.height() * 2, size.height()));
            break;
        case Qt::AlignTop:
            top.setWidth(qMax(top.width(), size.width()));
            top.setHeight(top.height() + size.height());
            break;
        case Qt::AlignBottom:
            bottom.setWidth(qMax(bottom.width(), size.width()));
            bottom.setHeight(bottom.height() + size.height());
            break;
        }
    }
    return minimum.adjusted(0,
                            0,
                            left.width() + right.width() + qMax(top.width(), bottom.width()),
                            top.height() + bottom.height() + qMax(left.height(), right.height()));
}

SCHARTS_END_NAMESPACE
