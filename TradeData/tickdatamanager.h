#ifndef TICKDATAMANAGER_H
#define TICKDATAMANAGER_H

#include <QObject>
#include "tradedata.h"
#include "tickdatafactory.h"

/*!
 * \brief �ֱ����ݹ�������
 *
 * �ֱ���������ײ㣬��ȫ��Ľ������ݡ�ͨ���ֱ����ݿ��Լ���õ���ͬ���ڵ���ʷ���ݡ����ֱ����ݸ���ʱ��Ӧ��
 * ��������������ͬ���ڵ���ʷ�����У�ͨ�����HistoryDataManagerʵ�֣���\todo ���ʵ����һ�㣿
 *
 * ���ڻ�ȡĳ��һ��ķ�ʱ���ƣ���ͨ���ֱ����ݼ���õ��ĵ���1�������ݣ���Ӧ��TickDataManager�л�ȡ������Ӧ
 * �ô�HistoryDataManager�л�ȡ��������������1�������ݣ���Ӧ�ô�HistoryDataManager�л�ȡ��
 */
class TRADEDATASHARED_EXPORT TickDataManager : public QObject
{
    Q_OBJECT
public:
    AbstractTickData* getTickData(const QString& symbol, const QString& date);
    AbstractHistoryData* getRealTimeTradeData(const QString& symbol, const QString& date);

    static TickDataManager& instance();

protected:
    explicit TickDataManager(QObject *parent = 0);
    Q_DISABLE_COPY(TickDataManager)

private:
    //! QMap<����,QMap<����,����>>��
    QMap<QString, QMap<QString, TickDataItem*>> mData;
    TickDataFactory dataFactory;
    static TickDataManager* mInstance;
};

#endif // TICKDATAMANAGER_H
