#ifndef VALUESCALEDRAW_H
#define VALUESCALEDRAW_H

#include "tradingcurvewidgets_global.h"
#include "rectangularaxisdraw.h"

/*!
 * \brief 基于连续数值的直角坐标轴绘制工具。
 *
 * 连续数值表示坐标轴上显示数据是连续的。连续数值坐标轴的刻度值由QwtScaleDiv表示。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT ValueScaleDraw : public RectangularAxisDraw
{
public:
    ValueScaleDraw();
};

#endif // VALUESCALEDRAW_H
