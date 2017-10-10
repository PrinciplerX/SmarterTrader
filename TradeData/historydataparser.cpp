#include "historydataparser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

/*!
 * \todo ���ʣ�����ݡ�
 */
QString HistoryDataParser::toCsv(const TradeDataSeries &series)
{
    return QString();
}

bool HistoryDataParser::parseReply(const QByteArray &reply, DataProviderInfo::DataProvider provider, TradeDataSeries &output)
{
    switch(provider){
    case DataProviderInfo::IFENG:
        return parseIfengReply(reply, output);
    case DataProviderInfo::YAHOO:
        return parseYahooReply(reply, output);
    default:
        qDebug() << "HistoryDataParser::parseReply: �������޷��������������͡�";
        return false;
    }
}

/*!
 * \todo 15������ֻ�õ�������6�С�
 * \warning ��˲ƾ����ص���ʷ���ݲ�������ǰ����δ������ڵ����ݡ����磬�����24�Ž����ڼ䣨δ���̣�����������
 * �ݣ����ֻ�ܷ���23�ŵ����ݡ�
 */
bool HistoryDataParser::parseIfengReply(const QByteArray &reply, TradeDataSeries &output)
{
    bool result = false;
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(reply, &error);

    // ��˲ƾ��������ݣ�
    // {"record": data }
    // dataΪ�������ͣ�ÿ��Ԫ�صĴ洢����������ʾ��
    // ['date', 'open', 'high', 'close', 'low', 'volume', 'price_change', 'p_change',
    //  'ma5', 'ma10', 'ma20', 'v_ma5', 'v_ma10', 'v_ma20', 'turnover']

    if(error.error == QJsonParseError::NoError){
        if(!(document.isNull() || document.isEmpty())){
            if(document.isObject()){

                QJsonObject object = document.object();
                QJsonValue data = object.value("record");

                QJsonArray entryArray = data.toArray();
                output.resize(entryArray.size());

                for(int index = 0; index < entryArray.size(); index++){
                    const QJsonValue& item = entryArray.at(index);
                    QJsonArray array = item.toArray();

                    output[index].date = array[0].toString();
                    output[index].open = array[1].toString().toDouble();
                    output[index].high = array[2].toString().toDouble();
                    output[index].close = array[3].toString().toDouble();
                    output[index].low = array[4].toString().toDouble();
                    //! ��λΪ�֣�ԭʼ����Ϊdouble���ͣ���long���ͽ����������Ϊ0��
                    output[index].volume = array[5].toString().toDouble();
                }
                result = true;
            }
        }
    }

    return result;
}

/*!
 * \todo 7������ֻ�õ�������6�У���Adj Close�������ԡ�
 */
bool HistoryDataParser::parseYahooReply(const QString &reply, TradeDataSeries &output)
{
    QStringList data = reply.split('\n');
    if(data.size() > 1){
        for(int i=1; i<data.size(); i++){
            QStringList dataList=data.at(i).split(',');

            if(dataList.size()<5) return false;

            // ȥ��������Ϊ0�����ݣ�������ͣ���ڼ�û�н��׵ĵ����ݼ�¼��
            if(dataList.at(5).toLong()!=0)
                output.push_front({dataList.at(1).toDouble(),
                                   dataList.at(2).toDouble(),
                                   dataList.at(3).toDouble(),
                                   dataList.at(4).toDouble(),
                                   dataList.at(5).toLong(),
                                   dataList.at(0)});
        }
    }
    return true;
}

bool HistoryDataParser::parseTdxFile(const QString path, TradeDataSeries& output)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "can't open input file " << path;
        return false;
    }

    QTextStream in(&file);
    in.readLine(); // description text
    in.readLine(); // column titles

    while (true) {
        QStringList dataList = in.readLine().trimmed().split('\t');
        if (dataList.size() < 6) {
            // final text: "������Դ:ͨ����"
            break;
        }
        output.push_back({dataList.at(1).toDouble(),
                          dataList.at(2).toDouble(),
                          dataList.at(3).toDouble(),
                          dataList.at(4).toDouble(),
                          dataList.at(5).toLong(),
                          dataList.at(0).trimmed()});
    }

    file.close();
    return true;
}
