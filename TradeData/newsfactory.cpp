#include "newsfactory.h"
#include <QTextCodec>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

const QString NewsFactory::UT_LATEST_NEWS = QString("http://roll.news.sina.com.cn/interface/rollnews_ch_out_interface.php?col=%1&ch=03&&num=%2");

NewsFactory::NewsFactory(QObject *parent) : QObject(parent)
{
    connect(&downloader, SIGNAL(downloadComplete(bool,int,QByteArray)), this, SLOT(onReplyFinished(bool,int,QByteArray)));
}

void NewsFactory::requestLatestNews(NewsReferenceItem *item, int category, int num)
{
    QUrl url(UT_LATEST_NEWS.arg(QString::number(category), QString::number(num)));
    replyMap.insert(downloader.download(url), item);
}

bool NewsFactory::parseLatestNews(const QByteArray &reply, NewsReferenceItem *item)
{
    QVector<NewsReference> output;
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    QString input = codec->toUnicode(reply);

    QRegularExpression title(QString("path : \\[{title : \\\"(.*?)\\\", id : \\\"(.*?)\\\",.*?}\\]"));
    QRegularExpressionMatch m = title.match(input);
    if(m.hasMatch()){
        item->setCategory(QString("%1:%2").arg(m.captured(2), m.captured(1)));
    }

    QRegularExpression re(QString("{channel : {title : \\\"(.*?)\\\",.*?},title : \\\"(.*?)\\\",url : \\\"(.*?)\\\",.*?,time : (.*?)}"));
    QRegularExpressionMatchIterator iterator = re.globalMatch(input);

    while(iterator.hasNext()){
        QRegularExpressionMatch match = iterator.next();
        NewsReference item;
        item.category = match.captured(1);
        item.title = match.captured(2);
        item.url = match.captured(3);
        item.datetime = QDateTime::fromTime_t(match.captured(4).toUInt());
        output.append(item);
    }
    item->setData(output);
    return true;
}

void NewsFactory::onReplyFinished(bool isSucceed, int index, QByteArray data)
{
    QMap<int, NewsReferenceItem*>::iterator i = replyMap.find(index);
    if(i != replyMap.end()){
        if(isSucceed){
            parseLatestNews(data, i.value());
        }
        replyMap.remove(index);
    }
}
