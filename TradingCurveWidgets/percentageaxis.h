#ifndef PERCENTAGEAXIS_H
#define PERCENTAGEAXIS_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "titledplotaxis.h"
#include "percentagescaledraw.h"

namespace TradingCurveWidgets{

/*!
 * \brief 基于TraceScaleWidget，用百分比显示刻度值。百分比基于一个固定的相对值，可以手动设置。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT PercentageAxis : public TitledPlotAxis
{
    Q_OBJECT
public:
    explicit PercentageAxis(double baseValue, AbstractAxis::AxisType align, AbstractItemPlot *parent);

    void setBaseValue(double value);

signals:

public slots:

private:
    PercentageScaleDraw* scaleDraw;
};

}

#endif // PERCENTAGEAXIS_H
