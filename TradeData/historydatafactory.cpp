#include "historydatafactory.h"
#include "historydataparser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

HistoryDataFactory::HistoryDataFactory(QObject *parent) : QObject(parent)
{
    connect(&downloader, SIGNAL(downloadComplete(bool,int,QByteArray)), this, SLOT(onReplyFinished(bool,int,QByteArray)));
}

void HistoryDataFactory::request(TradeDataItem *item)
{
    if(!item) return;

    QUrl url;
    const DataProviderInfo& info = DataProviderInfo::getInstance();
    if(info.getDataUrl(DataProviderInfo::IFENG, item->getPeriod(), item->getSymbol(), url)){
        replyMap.insert(downloader.download(url), item);
    }
}

void HistoryDataFactory::onReplyFinished(bool isSucceed, int index, QByteArray data)
{
    QMap<int, TradeDataItem*>::iterator i = replyMap.find(index);
    if(i != replyMap.end()){
        if(isSucceed){
            TradeDataSeries series;
            if(HistoryDataParser::parseIfengReply(data, series))
                i.value()->setData(series);
        }
        replyMap.remove(index);
    }
}
