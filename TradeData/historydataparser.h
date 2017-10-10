#ifndef HISTORYDATAPARSER_H
#define HISTORYDATAPARSER_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "dataproviderinfo.h"
#include <QByteArray>

/*!
 * \brief ��ĳЩ����Դ��������ӿڻظ���������ʷ���ݣ����߽���ʷ����ת��Ϊ��һ�ָ�ʽ����csv����
 */
class TRADEDATASHARED_EXPORT HistoryDataParser
{
public:
    /*!
     * \brief ����ʷ����ת��Ϊcsv��ʽ��
     * \param series
     */
    static QString toCsv(const TradeDataSeries& series);

    /*!
     * \brief ����������ӿ��������õ��Ļظ�����ͬ�Ľӿ��ṩ�̵Ļظ���ʽ��ͬ�����Խ�����ʽҲ��ͬ��
     * \param reply ������ӿڻ�ȡ��������Ϣ��
     * \param provider �ӿ��ṩ�̡�
     * \param output ���ڽ��ս��������
     * \return �������һ���������򷵻�true�����ִ����޷������򷵻�false��
     */
    static bool parseReply(const QByteArray& reply, DataProviderInfo::DataProvider provider, TradeDataSeries& output);
    static bool parseIfengReply(const QByteArray& reply, TradeDataSeries& output);
    static bool parseYahooReply(const QByteArray& reply, TradeDataSeries& output);
    static bool parseYahooReply(const QString& reply, TradeDataSeries& output);

    static bool parseTdxFile(const QString path, TradeDataSeries& output);

private:
};

inline bool HistoryDataParser::parseYahooReply(const QByteArray &reply, TradeDataSeries &output){
    return parseYahooReply(QString(reply), output);
}

#endif // HISTORYDATAPARSER_H
