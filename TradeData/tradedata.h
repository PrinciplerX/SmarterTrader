#ifndef TRADEDATA_H
#define TRADEDATA_H

#include "tradedata_global.h"

#include <QString>
#include <QDateTime>
#include <QTime>

/*!
 * \brief ������λ�����ڹ����嵵��ʮ�������ݡ�
 */
struct TRADEDATASHARED_EXPORT OrederRecord{
    QString price;
    QString volume;
};

/*!
 * \brief ʵʱ��Ʊ�������ݸ�ʽ��������һ��ֻ������ʾ�����ᴢ�档���ԣ�ֻ��Ҫ�ַ�����ʽ���ݼ��ɡ�
 */
struct TRADEDATASHARED_EXPORT QuoteData{
    /*! Unique Id */
    QString symbol;
    /*! ���� */
    QString code;
    /*! ���� */
    QString name;
    /*! ���� */
    QString date;
    /*! ʱ�� */
    QString time;
    /*! �ּ� */
    QString current;
    /*! �Ƿ� */
    QString changeRate;
    /*! �ǵ� */
    QString change;
    /*! ���̼� */
    QString open;
    /*! ��߼� */
    QString high;
    /*! ��ͼ� */
    QString low;
    /*! ��һ�����յ����̼�*/
    QString lastClose;
    /*! �ɽ��� */
    QString volume;
    /*! �ɽ��� */
    QString turnover;
    /*! �嵵�������λ�� */
    OrederRecord ask1;
    OrederRecord ask2;
    OrederRecord ask3;
    OrederRecord ask4;
    OrederRecord ask5;
    /*! �嵵��������λ��*/
    OrederRecord bid1;
    OrederRecord bid2;
    OrederRecord bid3;
    OrederRecord bid4;
    OrederRecord bid5;
    /*! ������ */
    QString turnoverRate;
    /*! ��ҵ */
    QString industry;
    /*! ����ֵ��Total Market Capitalization�� */
    QString totalMktCap;
    /*! ��ͨ��ֵ��Negotiable Market Capitalization�� */
    QString negotiableCap;
    /*! ��ӯ�ʣ�Price/Earnings Ratio�� */
    QString pe;
    /*! �о��ʣ�Price/Book Value�� */
    QString pb;
};

/*!
 * \brief ����ʱ��K����ʷ���ݸ�ʽ����ʷ�������⡣�����ǹ�Ʊ���ڻ�Ʒ�֡�
 */
struct TRADEDATASHARED_EXPORT TradeData{
    /*! ���� */
    double open;
    /*! ��� */
    double high;
    /*! ��� */
    double low;
    /*! ���� */
    double close;
    /*! �ɽ��� */
    long volume;
    /*! ʱ�� */
    QString date;
};

/*!
 * \brief ����Tick���ݸ�ʽ��
 */
struct TRADEDATASHARED_EXPORT TickData{
    /*! ʱ�̣���ȷ���룬��ʽΪ��hh:mm:ss��*/
    QString time;
    /*! �۸� */
    double price;
    /*! �仯�� */
    double change;
    /*! �ɽ��� */
    int volume;
    /*! �ɽ��� */
    int amount;
    /*! ���������� */
    QString type;
};

/*!
 * \brief ��ʷ�������ڡ�
 */
enum DataPeriod {
    //! ������
    MONTH = 0,
    //! ������
    WEEK = 1,
    //! ������
    DAY = 2,
    //! 60��������
    MINUTES_60 = 3,
    //! 30��������
    MINUTES_30 = 4,
    //! 15��������
    MINUTES_15 = 5,
    //! 5��������
    MINUTES_5 = 6,
    //! 1��������
    MINUTES_1 = 7,
    //! ��ͬ���͵���������
    PERIOD_TYPE_COUNT = 8
};

/*!
 * \brief ��ʾһ������ʱ��Ρ�
 */
struct TRADEDATASHARED_EXPORT ContinuouTimeInterval{
    /*! ʱ�̣���ȷ�����ӡ���ʽΪ��mm:ss�� */
    QTime startTime;
    /*! ʱ�̣���ȷ�����ӡ���ʽΪ��mm:ss�� */
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
 * \brief ÿ��Ľ���ʱ�Σ������ɶ�������Ľ���ʱ�Σ�ContinuouTimeInterval����ɡ�
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

/*! \warning ��������Դ�Ĵ洢˳����Σ����������ǰѾ����ݴ洢��ǰ������ţ��������ݴ洢�ں󣨸���ţ���*/
typedef QVector<TradeData> TradeDataSeries;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// �ۺ���������
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief һ����ʷ���ݵ�������ʹ�ø�����Ψһ��ȷ����һ����ʷ���ݡ�
 */
struct TRADEDATASHARED_EXPORT HistoryDataIndex{
    /*! ���롣*/
    QString symbol;
    /*! �������ڡ�*/
    DataPeriod period;
};

/*!
 * \brief ��ʾһ��ʱ����ڵ���ʷ���ݡ�
 * \todo ���������Ƿ����
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
     * \brief �����ݱ����롣
     */
    void dataLoaded();
    /*!
     * \brief ������������¡�
     */
    void dataUpdated();
    /*!
     * \brief ������һ�����ڵ����ݡ�
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
    // ��������
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
    /*! \brief �ڷ����ź�֮ǰ�����ദ���Ѹ������ݵĻ��ᡣ*/
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
 * \brief ��ȡһ����ʷ���ݵĽӿڡ�
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

    /*! \return ����true��ʾ��ӳɹ���false��ʾ�߱���ͬ���ͺͲ�����ָ���Ѿ����ڣ����߲�����Ч��*/
    virtual bool attachIndicator(TechniqueIndicator* indicator) = 0;
    /*! \brief �Ƴ�������ָ������Ͳ�������ͬ��ָ�꣨��һ��Ϊͬһ���󣩡�*/
    virtual void removeIndicator(TechniqueIndicator* indicator) = 0;
    virtual QVector<TechniqueIndicator*>& indicators() = 0;

    /*!
     * \brief ��ȡ����ָ�����ָ�롣
     * \param name ָ�����ơ�
     * \return ����ҵ�ָ����ָ�꣬����ָ�����ָ�룻���򷵻�nullptr��
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
     * \brief �����ݱ����롣
     */
    void dataLoaded();
    /*!
     * \brief ������һ�����ڵ����ݡ�
     */
    void dataAppended();
};

#endif // TRADEDATA_H
