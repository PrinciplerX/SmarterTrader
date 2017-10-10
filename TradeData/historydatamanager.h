#ifndef HISTORYDATAMANAGER_H
#define HISTORYDATAMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "tradedata_global.h"
#include "tradedata.h"
#include "tradedataitem.h"
#include "tradedataitemproxy.h"
#include "historydatafactory.h"

/*!
 * \brief ������Ӧ�ó��������ڼ�����Ʊ����ʷ���ݣ��������ݵĻ�ȡ��������ͷš�
 *
 * ��ȡ���ݵ�;�������кܶ��֣������硢���ݿ⡢�Լ��ض���ʽ���ļ��ȡ�������Ϊһ�����ݷ����������е���ʷ��������Ӧ
 * �������ﴦ�������ȡ���ݵ�;�����ڵ����߶����ǲ��ɼ��ġ�
 *
 * ��ʷ���ݲ���ҪƵ������ ������˵��Ӧ�ó��������ڼ��ȡһ�ξͿ����ˡ��ڳ�������ʱ��ʹ��tick������������ʷ���ݡ���
 * ���������̶ȼ������������
 *
 * ��֧�ֶ������ݵ����������ַ�ʽ��ͬ��������첽���󡣿��Բ�ѯ�������Ƿ���ڣ���������ڿ��԰����ȡ��
 *
 * \todo ����Ӧ�ò�ȡһЩ���ݼ��ز��ԣ�������������ڼ�ļ����ٶȡ�
 * \date 2016-05-16
 */
class TRADEDATASHARED_EXPORT HistoryDataManager : public QObject
{
    Q_OBJECT
public:
    virtual ~HistoryDataManager();

    /*!
     * \todo Ӧ�÷������ݴ���
     */
    AbstractHistoryData* getHistoryData(const HistoryDataIndex& index);
    AbstractHistoryData* getHistoryData(const QString& symbol, DataPeriod period);

    AbstractHistoryData* fromTickData(const AbstractTickData* tickData, DailyTradeHour tradeHour);

    static HistoryDataManager& instance();

public slots:
    void update(const HistoryDataIndex& index, const TradeData& data);

protected:
    explicit HistoryDataManager(QObject *parent = 0);
    Q_DISABLE_COPY(HistoryDataManager)

private:
    //! ����-����ӳ���������ÿ������Ԫ�ض�Ӧһ�����ڡ�
    QVector<QMap<QString, TradeDataItem*>> data;
    HistoryDataFactory dataFactory;
    static HistoryDataManager* mInstance;
};

#endif // HISTORYDATAMANAGER_H
