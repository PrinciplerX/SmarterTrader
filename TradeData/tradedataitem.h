#ifndef TRADEDATAITEM_H
#define TRADEDATAITEM_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "realtimedatainterface.h"
#include "techniqueindicator.h"

/*!
 * \brief 实际存储历史数据和实时数据的类型。
 * \sa TradeDataItemProxy
 */
class TRADEDATASHARED_EXPORT TradeDataItem : public AbstractHistoryData, public RealTimeDataInterface
{
    Q_OBJECT
public:
    TradeDataItem(const HistoryDataIndex& dataIndex, const QString& itemName, QObject* parent = nullptr);
    TradeDataItem(const QString& symbol, DataPeriod period, const QString& itemName, QObject* parent = nullptr);

    virtual ~TradeDataItem();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 更新交易数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void setData(const TradeDataSeries& data) override;
    virtual void append(const TradeData& item) override;
    virtual void update(const TradeData& item) override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 基本信息
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int size() const override;
    virtual const TradeData& at(int index) const override;
    virtual const QVector<TradeData>& data() const override;

    virtual const QString& getSymbol() const override;
    virtual const QString& getName() const override;
    virtual DataPeriod getPeriod() const override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 关联技术指标
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool attachIndicator(TechniqueIndicator* indicator) override;
    virtual void removeIndicator(TechniqueIndicator* indicator) override;
    virtual TechniqueIndicator* findIndicator(const QString& name) override;
    virtual QVector<TechniqueIndicator*>& indicators() override;

private:
    Q_DISABLE_COPY(TradeDataItem)
    HistoryDataIndex index;
    /*! 名称。*/
    QString name;
    /*! 历史数据内容。*/
    TradeDataSeries mData;
    QVector<TechniqueIndicator*> indicatorVector;
};

#endif // TRADEDATAITEM_H
