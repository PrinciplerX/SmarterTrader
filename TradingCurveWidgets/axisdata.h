#ifndef AXISDATA_H
#define AXISDATA_H

#include "tradingcurvewidgets_global.h"
#include "rectangularaxisdraw.h"
#include <qwt_text.h>
#include <qwt_color_map.h>
#include <qwt_interval.h>

/*!
 * \brief 绘制一个坐标轴所使用的所有数据。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AxisData
{
public:
    AxisData();

    virtual ~AxisData();

    /*! 使用指针保存，支持多态特性。*/
    RectangularAxisDraw *scaleDraw = nullptr;

    // 确定坐标轴的最大显示区域。
    int basePos;        /*! 坐标轴绘制区域的位置，横轴对应横坐标，纵轴对应纵坐标。*/
    int scaleLength;    /*! 坐标轴绘制区域的长度，横轴对应横轴方向尺寸，纵轴对应纵轴方向尺寸。*/

    // 绘制时需注意的信息。
    int margin;

    int spacing;
};

#endif // AXISDATA_H
