#ifndef REALTIMECURVESCALEDRAW_H
#define REALTIMECURVESCALEDRAW_H

#include "rectangularaxisdraw.h"
#include "tradingcurvewidgets_global.h"
#include <QPainter>

namespace TradingCurveWidgets {

/*!
 * \brief 根据中心值（central value）绘制脚标。
 *
 * 默认将大于中心值的脚标绘制成红色，小于中心值的脚标绘制成绿色，等于中心值的绘制为白色。
 *
 * \sa TradingCurveWidgets::RealTimeCurvePlot
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimeCurveScaleDraw : public RectangularAxisDraw
{
public:
    RealTimeCurveScaleDraw(double centralValue);

    virtual void drawLabel(QPainter *painter, double value) const override;

    void setBaseValue(double baseValue);

private:
    double centralValue;
};

}

#endif // REALTIMECURVESCALEDRAW_H
