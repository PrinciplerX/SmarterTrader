#include "realtimedatafactory.h"
#include "realtimeparser.h"

RealTimeDataFactory::RealTimeDataFactory(QObject *parent) : QObject(parent)
{
    connect(&downloader, SIGNAL(downloadComplete(bool,int,QByteArray)), this, SLOT(onReplyFinished(bool,int,QByteArray)));
}

void RealTimeDataFactory::request(QMap<QString, RealTimeDataItem *> *data)
{
    QStringList symbols;
    for(QMap<QString, RealTimeDataItem *>::iterator i = data->begin(); i != data->end(); i++){
        symbols.append(i.key());
    }
    target = data;

    const DataProviderInfo& info = DataProviderInfo::getInstance();
    QUrl url;
    if(info.getRealTimeDataUrl(DataProviderInfo::SINA, symbols, url)){
        // һ���������ݰ��������д������Ϣ��
        index = downloader.download(url);
    }else{
        qDebug() << "��ȡʵʱ���ݽӿ�ʱ���ִ���";
    }
}

void RealTimeDataFactory::onReplyFinished(bool isSucceed, int index, QByteArray data)
{
    if(this->index == index && target){
        if(isSucceed){
            RealTimeParser::parseSinaReply(data, target);
            emit replyFinished();
        }
        index = -1;
    }
}
