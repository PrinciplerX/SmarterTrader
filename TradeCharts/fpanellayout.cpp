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

        // ����ÿ��QChart����ʾ����
        QVector<qreal> ratios;
        QVector<ChartPresenter*> visibleCharts;
        for (int i = 0; i < presenters.size(); i++) {
            if (presenters[i]->chart()->isVisible()) {
                visibleCharts.append(presenters[i]);
                ratios.append(1.0);
            }
        }
        calculateItemChartGeometry(contentGeometry, visibleCharts, ratios);

        // ʣ�����������ΪChartPresenter��������
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

    // ����������������ռ��ȵ�ѹ�����ʡ�
    QVector<qreal> leftSqueezeRatios(presenters.size(), 1.0);
    // �ײ�������������ռ��ȵ�ѹ�����ʡ�
    QVector<qreal> rightSqueezeRatios(presenters.size(), 1.0);
    // ÿ���������ƽ����ȣ���ֱ�����᷽���ϵĳߴ��С����
    QVector<qreal> vratios(presenters.size(), 0.0);

    QVector<qreal> topSqueezeRatios(presenters.size(), 1.0);
    // �ײ�������������ռ��ȵ�ѹ�����ʡ�
    QVector<qreal> bottomSqueezeRatios(presenters.size(), 1.0);
    // ÿ���������ƽ����ȣ���ֱ�����᷽���ϵĳߴ��С����
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
        // ���������������ռ��ȵ�ѹ�����ʡ�
        qreal& leftSqueezeRatio = leftSqueezeRatios[i];
        // �ұ�������������ռ��ȵ�ѹ�����ʡ�
        qreal& rightSqueezeRatio = rightSqueezeRatios[i];
        // ÿ���������ƽ����ȣ���ֱ�����᷽���ϵĳߴ��С����
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
        // ����������������ռ��ȵ�ѹ�����ʡ�
        qreal& topSqueezeRatio = topSqueezeRatios[i];
        // �ײ�������������ռ��ȵ�ѹ�����ʡ�
        qreal& bottomSqueezeRatio = bottomSqueezeRatios[i];
        // ÿ���������ƽ����ȣ���ֱ�����᷽���ϵĳߴ��С����
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

        // ÿ��������������������ռ����Ӧ����ȡ�
        minHeights[i] = qMax(minLeft.height(),minRight.height()) + 1;
        minWidths[i] = qMax(minTop.width(),minBottom.width()) + 1;

        // ȷ�����㹻�Ŀռ�������һ�������һ���̶ȱ�ǩ��ͬʱ��Ϊ�˾����ܵ�ѹ���м�ͼ��Ŀհ�����ֻ����һ�������
        // һ�� QChart �����̶ȱ�ǩ��λ�á�
        //! \todo �Ƿ�Ӧ��ֻѹ����legend�ɼ����ҽ���ֱ�������ڵ�����������������������legend���ɼ���Ӧ��������
        //! ������
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

        // ������������ʾ����
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

        // ʣ�����������ΪChartPresenter��������
        presenters[i]->setGeometry(chartRect);
        static_cast<QGraphicsRectItem *>(presenters[i]->plotAreaElement())->setRect(chartRect);

        topLeft.setY(topLeft.y() + ratios[i] / total * geometry.height());
    }
}

SCHARTS_END_NAMESPACE
