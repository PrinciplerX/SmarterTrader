#ifndef TITLEDPLOTAXIS_H
#define TITLEDPLOTAXIS_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "tracescalewidget.h"
#include "abstractitemplot.h"

namespace TradingCurveWidgets{

/*!
 * \brief 配备TitledPlotCanvas的TitledPlot中的坐标轴控件。
 *
 * 默认情况下，坐标轴的坐标轴线和画布对齐。但是对于包含一个标题控件的画布，不能使用border dist来衡量这个部分，否则会出现Plot布局问题。
 * 该坐标轴的border dist总是为0。而将坐标轴的一部分来衡量title widget的部分。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT TitledPlotAxis : public TraceScaleWidget
{
    Q_OBJECT
public:
    explicit TitledPlotAxis(AbstractAxis::AxisType align, AbstractItemPlot *parent = nullptr);

    static AbstractPlot::Axis axisId(AbstractAxis::AxisType alignment);

    virtual void updateScaleDiv() override;
    virtual void updateAxisMap();
    virtual QwtScaleDiv caculateScaleDiv() const;
    virtual QwtScaleMap caculateCanvasMap(const QWidget* canvas) const override;

    void setTitleHeight(int height);

private:
    AbstractItemPlot* plot;

    //纵轴上两个major tick间的最大宽度。当纵轴长度变化时，会重新计算刻度值scaleDiv，调整
    //major tick数目，使major tick之间的宽度始终不超过这个值。
    int maxMajorTickSpace = 60;

    // 记录当前坐标轴被刻度分成的区间数。
    mutable int majorIntervalCount;

    int titleHeight = 15;
};

}
#endif // TITLEDPLOTAXIS_H
