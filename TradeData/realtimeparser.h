#ifndef REALTIMEPARSER_H
#define REALTIMEPARSER_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "dataproviderinfo.h"
#include "QByteArray"

class TRADEDATASHARED_EXPORT RealTimeParser
{
public:
    /*!
     * \brief �������ݽӿڷ��ص�ʵʱ���ݡ�
     * \param reply ���ݽӿڷ������ݣ�����Ҫ�󷵻ص�ʵʱ���ݵ�����ʽ�����ݴ洢������target���ı���˳������ͬ�ġ�
     * \param provider �����ṩ�̡�
     * \param target ���ݴ洢������
     * \return
     */
    static bool parseReply(const QByteArray& reply, DataProviderInfo::DataProvider provider,
                           QMap<QString, RealTimeDataItem*>* target);
    static bool parseSinaReply(const QByteArray& reply, QMap<QString, RealTimeDataItem *> *target);
    static bool parseSinaReply(const QString& reply, QuoteData* target);

    static QString formatDoubleString(const QString &num, char format, int precision);
};

#endif // REALTIMEPARSER_H
