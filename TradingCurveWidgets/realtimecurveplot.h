#ifndef REALTIMECURVEPLOT_H
#define REALTIMECURVEPLOT_H

#include <QWidget>
#include "realtimeplot.h"
#include "tradingcurvewidgets_global.h"
#include "percentageaxis.h"

namespace TradingCurveWidgets{

/*!
 * \brief 用于显示实时价格曲线，包含数值坐标轴和百分比坐标轴。
 *
 * 自定义坐标轴范围和刻度值，使显示的数据都处在基于某个数值(centralValue)的对称区间范围内。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimeCurvePlot : public RealTimePlot
{
    Q_OBJECT
public:
    /*!
     * \brief 构造函数
     * \param value 中心位置数值
     * \param parent 父控件
     */
    explicit RealTimeCurvePlot(double value = 0.0, QWidget *parent = nullptr);

    /*!
     * \todo 这里没有将纵轴全部设置，而是只设置了percentageAxis。
     */
    void setCentralValue(double value);

    virtual void drawCanvasGrid(QPainter* painter) override;

private:
    double centralValue;
    PercentageScaleDraw* percentageScaleDraw;
};

}

#endif // REALTIMECURVEPLOT_H
