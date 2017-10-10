#ifndef REALTIMEPLOT_H
#define REALTIMEPLOT_H

#include "tradingcurvewidgets_global.h"
#include "abstractitemplot.h"
#include "realtimeitemaxis.h"
#include <QLabel>
#include <QWidget>
#include <QMap>

namespace TradingCurveWidgets {

/*!
 * \brief 用于绘制实时走势图。
 *
 * RealTimePlot总是将所有数据全部绘制到canvas上，并且具有连续（或者分段连续）的时间轴。
 * RealTimePlot以分钟为最小单位，时间轴上任何一点能够映射到某一分钟。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimePlot : public AbstractItemPlot
{
    Q_OBJECT

public:

    explicit RealTimePlot(RealTimeItemAxis::UpdateDirection direction = RealTimeItemAxis::OldestLeft, QWidget* parent = nullptr);

    virtual ~RealTimePlot(){}

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 横坐标相关
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual AbstractItemAxis *getItemAxis() override;
    virtual const AbstractItemAxis *getItemAxis() const override;

    void setPlotableIndexCount(int count);
    int getPlotableIndexCount() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 网格
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void drawCanvasGrid(QPainter* painter);
    bool isVerticalGridVisible();
    void hideVerticalGrid();
    void showVerticalGrid();
    void setVerticalGrid(const QMap<int, QPen>& verticalGrid);

protected:

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 绘图元素（PlotableItem）
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int bodyWidth() const override;

private:

    /*! \brief canvas上显示的数据项数。*/
    int plotableIndexCount;

    bool verticalGridVisible = true;

    QMap<int, QPen> verticalGrid;

    // 基序号，表示坐标图中所显示数据中序号最小的数据在PlotItem中的序号。根据
    // UpdateDirection，该序号可能对应canvas最左边，也可能对应canvas最右边。
    int baseIndex = 0;

    RealTimeItemAxis* axis;
};

}

#endif // REALTIMEPLOT_H
