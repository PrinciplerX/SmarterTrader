#ifndef TRADEDATAITEM_H
#define TRADEDATAITEM_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "realtimedatainterface.h"
#include "techniqueindicator.h"

/*!
 * \brief ʵ�ʴ洢��ʷ���ݺ�ʵʱ���ݵ����͡�
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
    // ���½�������
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void setData(const TradeDataSeries& data) override;
    virtual void append(const TradeData& item) override;
    virtual void update(const TradeData& item) override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ������Ϣ
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int size() const override;
    virtual const TradeData& at(int index) const override;
    virtual const QVector<TradeData>& data() const override;

    virtual const QString& getSymbol() const override;
    virtual const QString& getName() const override;
    virtual DataPeriod getPeriod() const override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ��������ָ��
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool attachIndicator(TechniqueIndicator* indicator) override;
    virtual void removeIndicator(TechniqueIndicator* indicator) override;
    virtual TechniqueIndicator* findIndicator(const QString& name) override;
    virtual QVector<TechniqueIndicator*>& indicators() override;

private:
    Q_DISABLE_COPY(TradeDataItem)
    HistoryDataIndex index;
    /*! ���ơ�*/
    QString name;
    /*! ��ʷ�������ݡ�*/
    TradeDataSeries mData;
    QVector<TechniqueIndicator*> indicatorVector;
};

#endif // TRADEDATAITEM_H
