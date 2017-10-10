#ifndef PERCENTAGESCALEDRAW_H
#define PERCENTAGESCALEDRAW_H

#include "rectangularaxisdraw.h"
#include "tradingcurvewidgets_global.h"

namespace TradingCurveWidgets {

class TRADINGCURVEWIDGETSSHARED_EXPORT PercentageScaleDraw : public RectangularAxisDraw
{
public:
    PercentageScaleDraw(double value = 50.0);

    /*!
     * \brief 将数值转化为百分比。百分比的计算基于baseValue。
     * \param value 要转化的数值。
     * \return 百分比格式的字符串。
     */
    virtual QwtText label(double value) const override;

    virtual void drawLabel(QPainter *painter, double value) const override;

    void setBaseValue(double value);
    double baseValue() const;

private:
    //使用该值计算其他值的变化百分比
    double mBaseValue;
};

}

#endif // PERCENTAGESCALEDRAW_H
