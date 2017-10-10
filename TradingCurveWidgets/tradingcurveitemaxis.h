#ifndef TRADINGCURVEITEMAXIS_H
#define TRADINGCURVEITEMAXIS_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "abstractitemaxis.h"

#define BASIC_SCALE_LEVEL_COUNT 11

namespace TradingCurveWidgets{

class TRADINGCURVEWIDGETSSHARED_EXPORT TradingCurveItemAxis : public AbstractItemAxis
{
    Q_OBJECT
public:
    struct CandleStickSize{
        int shadowWidth;     //上下影线的宽度
        int bodyWidth;       //主体部分宽度
        int space;           //相邻两个柱线间隔长度
    };

    explicit TradingCurveItemAxis(AbstractAxis::AxisType align, AbstractItemPlot *parent);

    int caculateItemPos(int index) const;
    int caculateItemIndex(int x) const;
    int caculatePlotPos(int index) const;
    int caculatePlotIndex(int x) const;

    int getScaleLevel() const;

    void resetIndexRange();
    virtual void updatePlotIndexRange() override;

    /*!
     * \brief 放大图形。以参数指示位置为中心放大图形。
     *
     * 首先，调整currentScaleLevel，获取对应放缩级别所对应的单个柱线所占用的宽度。然后计算在当前的控
     * 件宽度内所能绘制的数据元素数目，接着以参数pos为中心，确定这些数据元素的具体区间。最后，根据所
     * 绘数据的日期范围和最大最小值调整纵横坐标轴显示区间，重新绘制图形。
     *
     * \param pos 数据集合中的序号，作为放大区域的中心位置。
     */
    virtual void zoomIn();

    /*!
     * \brief 缩小图形。以参数指示位置为中心缩小图形。
     * \param pos 数据集合中的序号，作为缩小区域的中心位置
     */
    virtual void zoomOut();

    CandleStickSize currentItemSize();
    virtual int bodyWidth() const override;

    virtual void onSelectedItemChanged() override;
    virtual void onPlotableItemAdded() override;

    virtual void resizeEvent(QResizeEvent *event) override;

    const AbstractItemPlot *getItemPlot() const;
    AbstractItemPlot *getItemPlot();

    /*!
     * \brief indexPaintInterval 相邻序号绘图坐标的差值，仅和scale level有关。
     */
    double indexPaintInterval() const;
    const CandleStickSize& candleStickSize() const;

    virtual void updateAxisMap() override;
    virtual QwtScaleMap caculateCanvasMap(const QWidget *canvas) const override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Controller
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setScaleLevel(int scaleLevel);

signals:
    void scaleLevelChanged();

protected:
     virtual void updateTickLabels() override;
    /*!
     * \brief 给定屏幕上显示数据项下限（最小序号）求绘制数据项范围
     * \param minIndex 屏幕上显示数据的期望最小序号，但并不一定是计算结果中的最小序号。
     * \return 绘制数据项范围
     */
    IndexRange caculatePlotRangeWithMinIndex(int minIndex);
    /*!
     * \brief 给定屏幕上显示数据项上限（最大序号）求绘制数据项范围
     * \param maxIndex 屏幕上显示数据的期望最大序号，但并不一定是计算结果中的最大序号。
     * \return 绘制数据项范围
     */
    IndexRange caculatePlotRangeWithMaxIndex(int maxIndex);
    /*!
     * \brief 给定屏幕上显示数据项中限（中间序号）求绘制数据项范围
     * \param maxIndex 屏幕上显示数据的期望中间序号，但并不一定是计算结果中的中间序号。
     * \return 绘制数据项范围
     */
    IndexRange caculatePlotRangeWithMidIndex(int midIndex);

private:
    /*! 当前放缩级别。可放大到的最大级别为0，随数值增加而图像逐渐缩小。大于10时，柱线之间产生重合 */
    int currentScaleLevel = 6;

    static const CandleStickSize CandleStickSizeStandard[BASIC_SCALE_LEVEL_COUNT];

    AbstractItemPlot* itemPlot;
};

}

#endif // TRADINGCURVEITEMAXIS_H
