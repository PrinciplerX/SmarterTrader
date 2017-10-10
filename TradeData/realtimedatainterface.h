#ifndef REALTIMEDATAINTERFACE_H
#define REALTIMEDATAINTERFACE_H

#include "tradedata_global.h"
#include "tradedata.h"

/*!
 * \brief 实时更新数据类型的接口。
 */
class TRADEDATASHARED_EXPORT RealTimeDataInterface
{
public:
    virtual ~RealTimeDataInterface() {}

    /*!
     * \brief setData 设置历史数据，计算指标数值序列。
     * \param data 历史数据。
     */
    virtual void setData(const TradeDataSeries& data) = 0;

    /*!
     * \brief 增加一个周期的数据，该周期为指标当前表示涵盖周期范围之外的下一周期。
     * \param data 新周期的交易数据内容。
     */
    virtual void append(const TradeData& data) = 0;

    /*!
     * \brief 更新指标当前涵盖范围内的最新数据，重新计算指标的最新值。
     *
     * 常常会出现这样一种需求：当最新的一个周期尚未结束时，其最高价、最低价、收盘价以及成交量都在不停变化。在该周期内
     * 的价格发生变化时，技术指标需要根据最新数据重复计算最新指标数值。当这个周期结束时，OHLVC数据全部确定下来，指标
     * 数值也不再是动态变化的，成为了一个固定的值（此时可以使用append函数添加下一周期的交易数据，计算新周期的指标）。
     *
     * \param data 最新周期的交易数据内容。
     */
    virtual void update(const TradeData& data) = 0;
};

#endif // REALTIMEDATAINTERFACE_H
