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

        // ���ֺͱ���֮����һ��margins��contentMargins������rect��ȥ�ò��֡�
        QRectF contentGeometry = calculateBackgroundGeometry(rect, background);
        // ���������ݵı߽�֮����һ��margins��m_margins������contentGeometry�г�ȥ�ò��֡�
        contentGeometry = calculateContentGeometry(contentGeometry);

        // ��contentGeometry�г�ȥ��������
        if (title && title->isVisible())
            contentGeometry = calculateTitleGeometry(contentGeometry, title);

        contentGeometry = calculateLegendAndAxisGeometry(contentGeometry, legend, axes);

        // ʣ�����������ΪChartPresenter��������
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
    // ���������������ռ��ȵ�ѹ�����ʡ�
    qreal leftSqueezeRatio = 1.0;
    // �ұ�������������ռ��ȵ�ѹ�����ʡ�
    qreal rightSqueezeRatio = 1.0;
    // ÿ���������ƽ����ȣ���ֱ�����᷽���ϵĳߴ��С����
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
    // ����������������ռ��ȵ�ѹ�����ʡ�
    qreal topSqueezeRatio = 1.0;
    // �ײ�������������ռ��ȵ�ѹ�����ʡ�
    qreal bottomSqueezeRatio = 1.0;
    // ÿ���������ƽ����ȣ���ֱ�����᷽���ϵĳߴ��С����
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

    // ÿ��������������������ռ����Ӧ����ȡ�
    qreal minHeight = qMax(minLeft.height(),minRight.height()) + 1;
    qreal minWidth = qMax(minTop.width(),minBottom.width()) + 1;

    // ȷ�����㹻�Ŀռ�������һ�������һ���̶ȱ�ǩ��
    left.setWidth(qMax(labelExtents.width(), left.width()));
    right.setWidth(qMax(labelExtents.width(), right.width()));
    top.setHeight(qMax(labelExtents.height(), top.height()));
    bottom.setHeight(qMax(labelExtents.height(), bottom.height()));

    // ����Ŀ������ӦΪˮƽ����������������ռ��ȵ�һ�롣
    // ����Ŀ������ӦΪ��ֱ����������������ռ��ȵ�һ�롣
    // ����ȷ���������ͼ����
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

    // ����ͬһλ�ÿ����ж�������ᣬ������Ҫ��¼ͬһλ��ÿ���������ƫ����������һ���ۻ�������
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
