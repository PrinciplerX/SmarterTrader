#include "realtimeparser.h"
#include <QTextCodec>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

bool RealTimeParser::parseReply(const QByteArray &reply, DataProviderInfo::DataProvider provider, QMap<QString, RealTimeDataItem *> *target)
{
    switch(provider){
    case DataProviderInfo::SINA:
        return parseSinaReply(reply, target);
    default:
        return false;
    }
}

bool RealTimeParser::parseSinaReply(const QByteArray &reply, QMap<QString, RealTimeDataItem *> *target)
{
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    QStringList list = codec->toUnicode(reply).split('\n');

    QMap<QString, RealTimeDataItem *>::const_iterator i = target->begin();
    static QuoteData temp;

    for(QString& item : list){
        parseSinaReply(item, &temp);
        i.value()->update(&temp);
        i++;

        if(i == target->end())
            break;
    }
    return true;
}

bool RealTimeParser::parseSinaReply(const QString &reply, QuoteData *target)
{
    QRegularExpression re(QString("^var hq_str_(.*)=\\\"(.*)\\\";"));
    QRegularExpressionMatch match = re.match(reply);

    if(!match.hasMatch()) return false;

    QString symbol = match.captured(1);
    QString content = match.captured(2);

    if(symbol.isEmpty() || content.isEmpty()) return false;

    target->symbol = symbol;
    QStringList parsedData = content.split(',');
    if (parsedData.size() <= 32){
        return false;
    }

    target->name = parsedData.at(0);
    target->open = formatDoubleString(parsedData.at(1), 'g', 5);
    target->lastClose = formatDoubleString(parsedData.at(2), 'g', 5);
    target->current = formatDoubleString(parsedData.at(3), 'g', 5);
    target->high = formatDoubleString(parsedData.at(4), 'g', 5);
    target->low = formatDoubleString(parsedData.at(5), 'g', 5);

    target->volume = QString::number(parsedData.at(8).toDouble() / 1000000.0, 'f', 2) + QString("万");
    target->turnover = QString::number(parsedData.at(9).toDouble() / 100000000.0, 'f', 2) + QString("亿");

    // 五档数据
    target->ask1.volume = parsedData.at(10);
    target->ask1.price = parsedData.at(11);
    target->ask2.volume = parsedData.at(12);
    target->ask2.price = parsedData.at(13);
    target->ask3.volume = parsedData.at(14);
    target->ask3.price = parsedData.at(15);
    target->ask4.volume = parsedData.at(16);
    target->ask4.price = parsedData.at(17);
    target->ask5.volume = parsedData.at(18);
    target->ask5.price = parsedData.at(19);

    target->bid1.volume = parsedData.at(20);
    target->bid1.price = parsedData.at(21);
    target->bid2.volume = parsedData.at(22);
    target->bid2.price = parsedData.at(23);
    target->bid3.volume = parsedData.at(24);
    target->bid3.price = parsedData.at(25);
    target->bid4.volume = parsedData.at(26);
    target->bid4.price = parsedData.at(27);
    target->bid5.volume = parsedData.at(28);
    target->bid5.price = parsedData.at(29);

    target->date = parsedData.at(30);
    target->time = parsedData.at(31);
    target->turnoverRate = QString("----");

    double lastClosePrice = target->lastClose.toDouble();
    double currentPrice = target->current.toDouble();

    QString crt(currentPrice >= lastClosePrice ? "+%1%" : "%1%");
    target->change = QString::number(currentPrice - lastClosePrice, 'f', 2);
    target->changeRate = crt.arg(QString::number(100.0*(currentPrice / lastClosePrice - 1.0), 'f', 2));

    return true;
}

QString RealTimeParser::formatDoubleString(const QString &num, char format, int precision){
    /*
        bool result;
        double d=num.toDouble(&result);
        if(!result)
        return num;

        return QString::number(d,format,precision);
        */
    Q_UNUSED(format)
    Q_UNUSED(precision)
    return num;
}
