#include "tickdatafactory.h"
#include <QTextCodec>

TickDataFactory::TickDataFactory(QObject *parent) : QObject(parent)
{
    connect(&downloader, SIGNAL(downloadComplete(bool,int,QByteArray)), this, SLOT(onReplyFinished(bool,int,QByteArray)));
}

QVector<TickData> TickDataFactory::parse(const QString &fileData){
    QStringList data = fileData.split('\n');
    QVector<TickData> samples;
    if (data.size() > 1) {
        for (int i = 1; i<data.size(); i++) {
            QStringList dataList = data.at(i).split('\t');

            if (dataList.size()<5) return samples;

            samples.push_front({
                                   dataList.at(0),
                                   dataList.at(1).toDouble(),
                                   dataList.at(2).toDouble(),
                                   dataList.at(3).toInt(),
                                   dataList.at(4).toInt(),
                                   dataList.at(5)
                               });
        }
    }
    return samples;
}

QVector<TickData> TickDataFactory::parseFile(const QString &fileName){
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    return parse(QString(file.readAll()));
}

bool TickDataFactory::parseSinaReply(const QByteArray &reply, QVector<TickData> &output)
{
    QTextCodec* codec = QTextCodec::codecForName("GB18030");
    QString data = codec->toUnicode(reply);
    output = parse(data);
    return true;
}

void TickDataFactory::request(TickDataItem *item)
{
    if(!item) return;

    QUrl url;
    const DataProviderInfo& info = DataProviderInfo::getInstance();
    if(info.getHistoryTickUrl(DataProviderInfo::SINA, item->getSymbol(), item->getDate(), url)){
        replyMap.insert(downloader.download(url), item);
    }else{
        qDebug() << "无法下载分笔数据。";
    }
}

void TickDataFactory::onReplyFinished(bool isSucceed, int index, QByteArray data)
{
    QMap<int, TickDataItem*>::iterator i = replyMap.find(index);
    if(i != replyMap.end()){
        if(isSucceed){
            QVector<TickData> series;
            if(parseSinaReply(data, series))
                i.value()->setData(series);
        }
        replyMap.remove(index);
    }
}

