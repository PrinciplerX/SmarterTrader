#ifndef REALTIMEITEMAXIS_H
#define REALTIMEITEMAXIS_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "abstractitemaxis.h"

namespace TradingCurveWidgets{

/*!
 * \brief 总是在运行时显示固定数目的PlotIndex。
 *
 * 刻度脚标计算策略：保存数据序号和刻度脚标的一个字典（map），需要时动态查找。
 * 游标文字计算策略：以向量组形式保存所有指示文字（vector），查询时直接返回。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimeItemAxis : public AbstractItemAxis
{
    Q_OBJECT
public:
    /*!
     * \brief 数据更新的方向。
     *
     * 这里不管真实数据如何组织，总是假设：数据源序号为0的数据为最旧的数据，数据源序号最大的数据为旧数据。
     * 数据序号 = 基序号 + 绘图序号。
     */
    enum UpdateDirection{
        //! 更新旧数据向右移动，表示新数据总是在左边：右边绘图序号为0，左边绘图序号最大（绘图序号增加方向和x坐标增长方向相反）。
        NewestLeft,

        //! 更新旧数据向左移动，表示新数据总是在右边：右边绘图序号最大，左边绘图序号为0（绘图序号增加方向和x坐标增长方向相同）。
        NewestRight,

        //! 更新时不会移动旧数据，而且旧数据被固定在最左边（序号增加方向和x坐标增长方向相同）。
        OldestLeft,

        //! 更新时不会移动旧数据，而且就数据被固定在最右边（序号增加方向和x坐标增长方向相反）。
        OldestRight
    };

    explicit RealTimeItemAxis(AbstractAxis::AxisType align = AbstractAxis::LeftScale,
                              UpdateDirection direction = NewestLeft,
                              QWidget *parent = nullptr);

    void setPlotableIndexCount(int count);
    int getPlotableIndexCount() const;

    DELETE_LATER //virtual int caculateItemPos(int index) const;
    DELETE_LATER //virtual int caculateItemIndex(int x) const;
    DELETE_LATER //virtual int caculatePlotIndex(int x) const;
    DELETE_LATER //virtual int caculatePlotPos(int index) const;

    virtual QwtScaleMap caculateCanvasMap(const QWidget *canvas) const override;

    virtual void updateAxisMap();

    virtual void onSelectedItemChanged() override;
    virtual void onPlotableItemAdded() override;

    virtual void updatePlotIndexRange() override;
    virtual int bodyWidth() const override;

protected:
    virtual void updateTickLabels() override;

private:
    /*! \brief canvas上显示的数据项数。*/
    int plotableIndexCount = 257;

    UpdateDirection updateDirection;

    // 数据序号 - 日期
    QMap<int, QString> majorTickMap;
    QVector<QString> tracerLabelVector;
};

}

#endif // REALTIMEITEMAXIS_H
