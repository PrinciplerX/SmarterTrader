#include "realtimecurveplot.h"
#include <climits>
#include <cfloat>
#include <QPushButton>
#include "abstractplotlayout.h"
#include "realtimecurvescaledraw.h"

namespace TradingCurveWidgets{

RealTimeCurvePlot::RealTimeCurvePlot(double value, QWidget *parent) :
    RealTimePlot(RealTimeItemAxis::OldestLeft, parent),
    centralValue(value),
    percentageScaleDraw(new PercentageScaleDraw(value))
{
    getTraceScaleWidget(yRight)->setScaleDraw(percentageScaleDraw);
    getScaleWidget(yLeft)->setEndBorderDistPolicy(AbstractAxis::Preferred);
    getScaleWidget(yLeft)->setFixedStartBorderDist(0);
    getScaleWidget(yRight)->setEndBorderDistPolicy(AbstractAxis::Preferred);
    getScaleWidget(yRight)->setFixedStartBorderDist(0);

    getScaleWidget(yLeft)->setScaleDraw(new RealTimeCurveScaleDraw(value));

    getScaleWidget(yLeft)->setTickLength(0, 0, 0);
    getScaleWidget(yRight)->setTickLength(0, 0, 0);

    getScaleWidget(xBottom)->scaleDraw()->enableComponent(RectangularAxisDraw::Backbone, false);
    getScaleWidget(xBottom)->scaleDraw()->setSpacing(0);
    getScaleWidget(xBottom)->setContentsMargins(0, 0, 0, 0);
}

void RealTimeCurvePlot::setCentralValue(double value){
    centralValue = value;
    percentageScaleDraw->setBaseValue(value);
    updateAxes();
}

void RealTimeCurvePlot::drawCanvasGrid(QPainter *painter)
{
    RealTimePlot::drawCanvasGrid(painter);
    QPen pen = getGridLinePen();
    setGridLinePen(pen);
    pen.setWidth(3);
    painter->setPen(pen);

    int y = canvasMap(yRight).transform(centralValue);
    painter->drawLine(0, y, getCanvas()->width()-1, y);
}

}
