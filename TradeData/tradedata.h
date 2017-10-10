#ifndef TRADEDATA_H
#define TRADEDATA_H

#include "tradedata_global.h"

#include <QString>
#include <QDateTime>
#include <QTime>

/*!
 * \brief 订单档位，用于构成五档，十档等数据。
 */
struct TRADEDATASHARED_EXPORT OrederRecord{
    QString price;
    QString volume;
};

/*!
 * \brief 实时股票报价数据格式，该数据一般只用于显示，不会储存。所以，只需要字符串格式数据即可。
 */
struct TRADEDATASHARED_EXPORT QuoteData{
    /*! Unique Id */
    QString symbol;
    /*! 代码 */
    QString code;
    /*! 名称 */
    QString name;
    /*! 日期 */
    QString date;
    /*! 时刻 */
    QString time;
    /*! 现价 */
    QString current;
    /*! 涨幅 */
    QString changeRate;
    /*! 涨跌 */
    QString change;
    /*! 开盘价 */
    QString open;
    /*! 最高价 */
    QString high;
    /*! 最低价 */
    QString low;
    /*! 上一交易日的收盘价*/
    QString lastClose;
    /*! 成交量 */
    QString volume;
    /*! 成交额 */
    QString turnover;
    /*! 五档（买进档位） */
    OrederRecord ask1;
    OrederRecord ask2;
    OrederRecord ask3;
    OrederRecord ask4;
    OrederRecord ask5;
    /*! 五档（卖出档位）*/
    OrederRecord bid1;
    OrederRecord bid2;
    OrederRecord bid3;
    OrederRecord bid4;
    OrederRecord bid5;
    /*! 换手率 */
    QString turnoverRate;
    /*! 行业 */
    QString industry;
    /*! 总市值（Total Market Capitalization） */
    QString totalMktCap;
    /*! 流通市值（Negotiable Market Capitalization） */
    QString negotiableCap;
    /*! 市盈率（Price/Earnings Ratio） */
    QString pe;
    /*! 市净率（Price/Book Value） */
    QString pb;
};

/*!
 * \brief 单个时刻K线历史数据格式，历史周期任意。可以是股票和期货品种。
 */
struct TRADEDATASHARED_EXPORT TradeData{
    /*! 开盘 */
    double open;
    /*! 最高 */
    double high;
    /*! 最低 */
    double low;
    /*! 收盘 */
    double close;
    /*! 成交量 */
    long volume;
    /*! 时刻 */
    QString date;
};

/*!
 * \brief 单条Tick数据格式。
 */
struct TRADEDATASHARED_EXPORT TickData{
    /*! 时刻，精确到秒，格式为“hh:mm:ss”*/
    QString time;
    /*! 价格 */
    double price;
    /*! 变化量 */
    double change;
    /*! 成交量 */
    int volume;
    /*! 成交额 */
    int amount;
    /*! 买卖盘类型 */
    QString type;
};

/*!
 * \brief 历史数据周期。
 */
enum DataPeriod {
    //! 月周期
    MONTH = 0,
    //! 周周期
    WEEK = 1,
    //! 日周期
    DAY = 2,
    //! 60分钟周期
    MINUTES_60 = 3,
    //! 30分钟周期
    MINUTES_30 = 4,
    //! 15分钟周期
    MINUTES_15 = 5,
    //! 5分钟周期
    MINUTES_5 = 6,
    //! 1分钟周期
    MINUTES_1 = 7,
    //! 不同类型的周期总数
    PERIOD_TYPE_COUNT = 8
};

/*!
 * \brief 表示一个连续时间段。
 */
struct TRADEDATASHARED_EXPORT ContinuouTimeInterval{
    /*! 时刻，精确到分钟。格式为“mm:ss” */
    QTime startTime;
    /*! 时刻，精确到分钟。格式为“mm:ss” */
    QTime endTime;

    bool contains(const QTime& time) const {
        return (startTime <= time && endTime >= time);
    }

    int size() const {
        return startTime.secsTo(endTime) / 60 + 1;
    }

    int indexOf(const QTime& time) const {
        return startTime.secsTo(time) / 60;
    }

    QTime at(int index) const
    {
        return startTime.addSecs(index * 60);
    }
};

/*!
 * \brief 每天的交易时段，可能由多个连续的交易时段（ContinuouTimeInterval）组成。
 */

class TRADEDATASHARED_EXPORT DailyTradeHour{
public:
    DailyTradeHour(const QVector<ContinuouTimeInterval>& tradeHour = QVector<ContinuouTimeInterval>()){
        mTradeHour = tradeHour;
    }
    virtual ~DailyTradeHour() {}

    int intervalCount() const {
        return mTradeHour.size();
    }

    int size() const
    {
        int totalSize = 0;
        for(int i = 0; i < mTradeHour.size(); i++){
            totalSize += mTradeHour[i].size();
        }
        return totalSize;
    }

    int intervalOf(const QTime& time) const
    {
        for(int i = 0; i < mTradeHour.size(); i++){
            if(mTradeHour[i].contains(time)){
                return i;
            }
        }
        return -1;
    }

    int indexOf(const QTime& time) const
    {
        int previous = 0;
        for(int i = 0; i < mTradeHour.size(); i++){
            if(mTradeHour[i].contains(time)){
                return mTradeHour[i].indexOf(time) + previous;
            }
            previous += mTradeHour[i].size();
        }
        return -1;
    }

    const ContinuouTimeInterval& interval(int index) const { return mTradeHour.at(index); }

    void append(const ContinuouTimeInterval& i) { mTradeHour.append(i); }

protected:
    int intervalOfIndex(int index) const
    {
        int previous = 0;
        for(int i = 0; i < mTradeHour.size(); i++){
            previous += mTradeHour[i].size();
            if(previous >= index){
                return i;
            }
        }
        return -1;
    }

private:
    QVector<ContinuouTimeInterval> mTradeHour;
};

typedef QVector<QString> DateSeries;

/*! \warning 无论数据源的存储顺序如何，该向量总是把旧数据存储在前（低序号），新数据存储在后（高序号）。*/
typedef QVector<TradeData> TradeDataSeries;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 综合数据类型
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief 一条历史数据的索引。使用该索引唯一地确定了一条历史数据。
 */
struct TRADEDATASHARED_EXPORT HistoryDataIndex{
    /*! 代码。*/
    QString symbol;
    /*! 数据周期。*/
    DataPeriod period;
};

/*!
 * \brief 表示一段时间段内的历史数据。
 * \todo 这样定义是否合理？
 */
struct TRADEDATASHARED_EXPORT HistoryDataReference{
    HistoryDataIndex index;
    QString start;
    QString end;
};

class TechniqueIndicator;

class TRADEDATASHARED_EXPORT LoadableObject : public QObject
{
    Q_OBJECT
public:
    LoadableObject(QObject* parent = 0) : QObject(parent) {}

signals:
    /*!
     * \brief 新数据被载入。
     */
    void dataLoaded();
    /*!
     * \brief 数据最新项更新。
     */
    void dataUpdated();
    /*!
     * \brief 新增加一个周期的数据。
     */
    void dataAppended();
};

template <typename T>
class TRADEDATASHARED_EXPORT LoadableVector : public LoadableObject
{
public:
    LoadableVector(QObject* parent = nullptr) : LoadableObject(parent) {}
    virtual ~LoadableVector() {}

    virtual int size() const = 0;
    virtual const T& at(int index) const = 0;
    const T& operator[](int index) const { return at(index); }
    virtual const QVector<T>& data() const = 0;
    bool isEmpty() const { return size() == 0; }
};

template <typename T>
class TRADEDATASHARED_EXPORT LoadableVectorItem : public LoadableVector<T>
{
    // Q_OBJECT
public:
    LoadableVectorItem(QObject* parent = nullptr) : LoadableVector<T>(parent) {}
    virtual ~LoadableVectorItem() {}

    virtual int size() const override { return mData.size(); }
    virtual const T& at(int index) const override { return mData.at(index); }
    virtual const QVector<T>& data() const override { return mData; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 更新数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setData(const QVector<T>& data)
    {
        mData = data;
        onDataLoaded();
        emit this->dataLoaded();
    }

    void append(const T& item)
    {
        mData.append(item);
        onDataAppended();
        emit this->dataAppended();
    }

    void update(const T& item)
    {
        mData[mData.size() - 1] = item;
        onDataUpdated();
        emit this->dataUpdated();
    }

protected:
    /*! \brief 在发送信号之前给子类处理已更新数据的机会。*/
    virtual void onDataLoaded() {}
    virtual void onDataAppended() {}
    virtual void onDataUpdated() {}

private:
    QVector<T> mData;
};

template <typename T>
class TRADEDATASHARED_EXPORT LoadableVectorProxy : public LoadableVector<T>
{
    //Q_OBJECT
public:
    LoadableVectorProxy(LoadableVectorItem<T>* parent = nullptr) : LoadableVector<T>(parent)
    {
        mData = parent;
        connect(mData, SIGNAL(dataLoaded()), this, SIGNAL(dataLoaded()));
        connect(mData, SIGNAL(dataAppended()), this, SIGNAL(dataAppended()));
        connect(mData, SIGNAL(dataUpdated()), this, SIGNAL(dataUpdated()));
    }

    virtual ~LoadableVectorProxy() {}

    virtual int size() const override { return mData->size(); }
    virtual const T& at(int index) const override { return mData->at(index); }
    virtual const QVector<T>& data() const override { return mData->data(); }

private:
    LoadableVectorItem<T>* mData;
};

/*!
 * \brief 读取一条历史数据的接口。
 */
class TRADEDATASHARED_EXPORT AbstractHistoryData : public LoadableVector<TradeData>
{
    Q_OBJECT
public:
    AbstractHistoryData(QObject* parent = nullptr) : LoadableVector<TradeData>(parent) {}
    virtual ~AbstractHistoryData() {}

    virtual const QString& getSymbol() const = 0;
    virtual const QString& getName() const = 0;
    virtual DataPeriod getPeriod() const = 0;

    /*! \return 返回true表示添加成功，false表示具备相同类型和参数的指标已经存在，或者参数无效。*/
    virtual bool attachIndicator(TechniqueIndicator* indicator) = 0;
    /*! \brief 移除和输入指标的类型参数均相同的指标（不一定为同一对象）。*/
    virtual void removeIndicator(TechniqueIndicator* indicator) = 0;
    virtual QVector<TechniqueIndicator*>& indicators() = 0;

    /*!
     * \brief 获取技术指标对象指针。
     * \param name 指标名称。
     * \return 如果找到指定的指标，返回指标对象指针；否则返回nullptr。
     */
    virtual TechniqueIndicator* findIndicator(const QString& name) = 0;
};

class TRADEDATASHARED_EXPORT AbstractRealTimeData : public QObject
{
    Q_OBJECT
public:
    AbstractRealTimeData(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~AbstractRealTimeData() {}

    virtual const QuoteData* getQuoteData() const = 0;

signals:
    void dataUpdated();
};

class TRADEDATASHARED_EXPORT AbstractTickData : public QObject
{
    Q_OBJECT
public:
    AbstractTickData(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~AbstractTickData() {}

    virtual int size() const = 0;
    virtual const TickData& at(int index) const = 0;
    const TickData& operator[](int index) const { return at(index); }
    virtual const QVector<TickData>& data() const = 0;

    virtual const QString& getSymbol() const = 0;
    virtual const QString& getDate() const = 0;

signals:
    /*!
     * \brief 新数据被载入。
     */
    void dataLoaded();
    /*!
     * \brief 新增加一个周期的数据。
     */
    void dataAppended();
};

#endif // TRADEDATA_H
