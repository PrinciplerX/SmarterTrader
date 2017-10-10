#ifndef BILLBOARDFACTORY_H
#define BILLBOARDFACTORY_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "downloader.h"

struct TRADEDATASHARED_EXPORT BillboardItem
{
    QString symbol;         /*!< ���� */
    QString name;           /*!< ���� */
    QString changeRate;     /*!< �����ǵ��� */
    QString amount;         /*!< ������ɽ���(��) */
    QString buyAmount;      /*!< �����(��) */
    QString buyratio;       /*!< ����ռ�ܳɽ����� */
    QString sellAmount;     /*!< ������(��) */
    QString sellratio;      /*!< ����ռ�ܳɽ����� */
    QString reason;         /*!< �ϰ�ԭ�� */
    QString date;           /*!< ���� */
};

class TRADEDATASHARED_EXPORT AbstractBillboardData : public LoadableVector<BillboardItem>
{
    Q_OBJECT
public:
    AbstractBillboardData(QObject* parent = nullptr) : LoadableVector<BillboardItem>(parent) {}
    virtual QString startDate() const = 0;
    virtual QString endDate() const = 0;
};

class TRADEDATASHARED_EXPORT BillboardDataItem : public AbstractBillboardData
{
    Q_OBJECT
public:
    BillboardDataItem(const QString& startDate, const QString& endDate, QObject* parent = nullptr):
        AbstractBillboardData(parent),
        mStartDate(startDate),
        mEndDate(endDate)
    {

    }

    virtual int size() const override { return mData.size(); }
    virtual const BillboardItem& at(int index) const override { return mData.at(index); }
    virtual const QVector<BillboardItem>& data() const override { return mData; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ��������
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setData(const QVector<BillboardItem>& data)
    {
        mData = data;
        emit dataLoaded();
    }

    void append(const BillboardItem& item)
    {
        mData.append(item);
        emit dataAppended();
    }

    virtual QString startDate() const override { return mStartDate; }
    virtual QString endDate() const override { return mEndDate; }
private:
    QString mStartDate;
    QString mEndDate;
    QVector<BillboardItem> mData;
};

class TRADEDATASHARED_EXPORT BillboardDataProxy : public AbstractBillboardData
{
    Q_OBJECT
public:
    BillboardDataProxy(BillboardDataItem* parent = nullptr) : AbstractBillboardData(parent)
    {
        mData = parent;
        connect(mData, SIGNAL(dataLoaded()), this, SIGNAL(dataLoaded()));
        connect(mData, SIGNAL(dataAppended()), this, SIGNAL(dataAppended()));
        connect(mData, SIGNAL(dataUpdated()), this, SIGNAL(dataUpdated()));
    }

    virtual ~BillboardDataProxy() {}

    virtual int size() const override { return mData->size(); }
    virtual const BillboardItem& at(int index) const override { return mData->at(index); }
    virtual const QVector<BillboardItem>& data() const override { return mData->data(); }

    virtual QString startDate() const override { return mData->startDate(); }
    virtual QString endDate() const override { return mData->endDate(); }

private:
    BillboardDataItem* mData;
};

class TRADEDATASHARED_EXPORT BillboardFactory : public QObject
{
    Q_OBJECT
public:
    explicit BillboardFactory(QObject *parent = 0);

    void request(BillboardDataItem* item);

    static bool parseBillboard(const QByteArray& reply, BillboardDataItem *item);

protected slots:
    void onReplyFinished(bool isSucceed, int index, QByteArray data);

private:
    QMap<int, BillboardDataItem*> replyMap;
    QVector<BillboardItem> mBillboardData;
    static const QString UT_EASTMONEY_BILLBOARD;
    Downloader downloader;
};

#endif // BILLBOARDFACTORY_H
