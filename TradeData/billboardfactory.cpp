#include "billboardfactory.h"
#include <QTextCodec>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

const QString BillboardFactory::UT_EASTMONEY_BILLBOARD = QString("http://data.eastmoney.com/DataCenter_V3/stock2016/TradeDetail/pagesize=200,page=1,sortRule=-1,sortType=,startDate=%1,endDate=%2,gpfw=0,js=vardata_tab_1.html");

BillboardFactory::BillboardFactory(QObject *parent) : QObject(parent)
{
    connect(&downloader, SIGNAL(downloadComplete(bool,int,QByteArray)), this, SLOT(onReplyFinished(bool,int,QByteArray)));
}

void BillboardFactory::request(BillboardDataItem *item)
{
    int index = downloader.download(UT_EASTMONEY_BILLBOARD.arg(item->startDate(), item->endDate()));
    replyMap.insert(index, item);
}

bool BillboardFactory::parseBillboard(const QByteArray &reply, BillboardDataItem *item)
{
    QVector<BillboardItem> output;
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    QString input = codec->toUnicode(reply);

    QRegularExpression itemPattern("{\\\"SCode\\\":\\\"(.*?)\\\","
                                   "\\\"SName\\\":\\\"(.*?)\\\","
                                   "\\\"ClosePrice\\\":\\\"(.*?)\\\","
                                   "\\\"Chgradio\\\":\\\"(.*?)\\\","
                                   "\\\"Dchratio\\\":\\\"(.*?)\\\","
                                   "\\\"JmMoney\\\":\\\"(.*?)\\\","
                                   "\\\"Turnover\\\":\\\"(.*?)\\\","
                                   "\\\"Ntransac\\\":\\\"(.*?)\\\","
                                   "\\\"Ctypedes\\\":\\\"(.*?)\\\","
                                   "\\\"Oldid\\\":\\\"(.*?)\\\","
                                   "\\\"Smoney\\\":\\\"(.*?)\\\","
                                   "\\\"Bmoney\\\":\\\"(.*?)\\\","
                                   "\\\"ZeMoney\\\":\\\"(.*?)\\\","
                                   "\\\"Tdate\\\":\\\"(.*?)\\\","
                                   "\\\"JmRate\\\":\\\"(.*?)\\\","
                                   "\\\"ZeRate\\\":\\\"(.*?)\\\","
                                   "\\\"Ltsz\\\":\\\"(.*?)\\\","
                                   ".*?,"
                                   "\\\"JD\\\":\\\"(.*?)\\\","
                                   "\\\"DP\\\":\\\"(.*?)\\\"}");
    QRegularExpressionMatchIterator iterator = itemPattern.globalMatch(input);

    double amount = 0.0;
    while(iterator.hasNext()){
        QRegularExpressionMatch match = iterator.next();
        BillboardItem item;
        item.symbol = match.captured(1);
        item.name = match.captured(2);
        item.changeRate = match.captured(4);
        item.amount = match.captured(6);
        item.reason = match.captured(9);

        amount = item.amount.toDouble();
        item.buyAmount = match.captured(12);
        item.buyratio = QString::number(item.buyAmount.toDouble() / amount, 'f' , 2);
        item.sellAmount = match.captured(11);
        item.sellratio = QString::number(item.sellAmount.toDouble() / amount, 'f' , 2);

        item.date = match.captured(14);
        output.append(item);
    }

    item->setData(output);
    return true;
}

void BillboardFactory::onReplyFinished(bool isSucceed, int index, QByteArray data)
{
    QMap<int, BillboardDataItem*>::iterator i = replyMap.find(index);
    if(i != replyMap.end()){
        if(isSucceed){
            parseBillboard(data, i.value());
        }
        replyMap.remove(index);
    }
}
