#include "historydataparser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

/*!
 * \todo 完成剩余内容。
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
        qDebug() << "HistoryDataParser::parseReply: 出现了无法解析的数据类型。";
        return false;
    }
}

/*!
 * \todo 15列数据只用到了其中6列。
 * \warning 凤凰财经返回的历史数据不包括当前最新未完成周期的数据。例如，如果在24号交易期间（未收盘）请求日线数
 * 据，最多只能返回23号的数据。
 */
bool HistoryDataParser::parseIfengReply(const QByteArray &reply, TradeDataSeries &output)
{
    bool result = false;
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(reply, &error);

    // 凤凰财经返回数据：
    // {"record": data }
    // data为数组类型，每个元素的存储内容如下所示：
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
                    //! 单位为手，原始数据为double类型，按long类型解析结果总是为0。
                    output[index].volume = array[5].toString().toDouble();
                }
                result = true;
            }
        }
    }

    return result;
}

/*!
 * \todo 7列数据只用到了其中6列，‘Adj Close’被忽略。
 */
bool HistoryDataParser::parseYahooReply(const QString &reply, TradeDataSeries &output)
{
    QStringList data = reply.split('\n');
    if(data.size() > 1){
        for(int i=1; i<data.size(); i++){
            QStringList dataList=data.at(i).split(',');

            if(dataList.size()<5) return false;

            // 去掉交易量为0的数据，这是在停牌期间没有交易的的数据记录。
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
            // final text: "数据来源:通达信"
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
