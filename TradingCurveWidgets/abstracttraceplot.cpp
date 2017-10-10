#include "abstracttraceplot.h"

AbstractTracePlot::AbstractTracePlot(QWidget *parent) : AbstractPlot(parent)
{

}

AbstractAxis *AbstractTracePlot::getAxisWidget(int axisId){
    return static_cast<AbstractAxis*>(getTraceAxis(axisId));
}

const AbstractAxis *AbstractTracePlot::getAxisWidget(int axisId) const{
    return static_cast<const AbstractAxis*>(getTraceAxis(axisId));
}
