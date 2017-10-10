#include "titledplotaxis.h"
#include "abstractitemplot.h"

namespace TradingCurveWidgets{

TitledPlotAxis::TitledPlotAxis(AbstractAxis::AxisType align, AbstractItemPlot *parent) :
    TraceScaleWidget(align, parent),
    plot(parent)
{
    setFixedBorderDist(0, 0);
}

SimplePlot::Axis TitledPlotAxis::axisId(AbstractAxis::AxisType alignment){
    switch(alignment){
    case AbstractAxis::TopScale:
        return AbstractPlot::xTop;
    case AbstractAxis::LeftScale:
        return AbstractPlot::yLeft;
    case AbstractAxis::RightScale:
        return AbstractPlot::yRight;
    case AbstractAxis::BottomScale:
        return AbstractPlot::xBottom;
    default:
        return AbstractPlot::axisCnt;
    }
}

/*!
 * \brief TitledPlotAxis::updateScaleDiv
 */
void TitledPlotAxis::updateScaleDiv(){
    updateAxisMap();
    setScaleDiv(caculateScaleDiv());
}

void TitledPlotAxis::updateAxisMap()
{
    double min,max;
    getCanvasValueRange(min, max);

    int realLength = scaleDraw()->length() - titleHeight;

    double pInterval = ValueAxisHelper::caculateScaleDivRange(realLength, min, max);
    max += pInterval * titleHeight;

    scaleDraw()->scaleMap().setScaleInterval(min, max);
    scaleDraw()->scaleMap().setPaintInterval(scaleDraw()->getPos().y() + scaleDraw()->length(), scaleDraw()->getPos().y());
    emit scaleMapChanged();
}

QwtScaleDiv TitledPlotAxis::caculateScaleDiv() const
{
    double min,max;
    getCanvasValueRange(min, max);

    int realLength = scaleDraw()->length() - titleHeight;

    majorIntervalCount = realLength / maxMajorTickSpace;
    majorIntervalCount = qMax(2, majorIntervalCount);

    //先添加顶部的tick，再将下面的空间等分确认剩余的tick
    QList<double> ticks[QwtScaleDiv::NTickTypes];

    double sInterval = (max - min) / majorIntervalCount;
    ticks[QwtScaleDiv::MajorTick].append(max);

    for (int i = 1; i < majorIntervalCount; i++) {
        ticks[QwtScaleDiv::MajorTick].append(max - sInterval * i);
    }

    return QwtScaleDiv(scaleDraw()->scaleMap().s1(), scaleDraw()->scaleMap().s2(), ticks);
}

QwtScaleMap TitledPlotAxis::caculateCanvasMap(const QWidget *canvas) const
{
    double min,max;
    getCanvasValueRange(min, max);

    if(isAxisEnabled()){
        double p1 = scaleDraw()->scaleMap().transform(min);
        double p2 = scaleDraw()->scaleMap().transform(max);

        QwtScaleMap map = scaleDraw()->scaleMap();
        map.setScaleInterval(min, max);

        // 坐标轴方向上，本地坐标到canvas坐标的偏移差值。
        int offset = 0;

        if(scaleDraw()->orientation() == Qt::Vertical){
            offset = canvas->mapFromGlobal(mapToGlobal(QPoint(0, 0))).y();
        }else{
            offset = canvas->mapFromGlobal(mapToGlobal(QPoint(0, 0))).x();
        }
        map.setPaintInterval(p1 + offset, p2 + offset);

        return map;
    }else{
        // 当坐标轴控件没有启用时，画布边界就是“坐标轴”。
        QwtScaleMap map = scaleDraw()->scaleMap();

        if(scaleDraw()->orientation() == Qt::Vertical){
            ValueAxisHelper::caculateScaleDivRange(canvas->height(), min, max);
            map.setPaintInterval(0.0, canvas->height());
        }else{
            ValueAxisHelper::caculateScaleDivRange(canvas->width(), min, max);
            map.setPaintInterval(0.0, canvas->width());
        }
        map.setScaleInterval(min, max);
        return map;
    }
}

void TitledPlotAxis::setTitleHeight(int height) { height = titleHeight; }

}
