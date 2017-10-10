#ifndef HISTORYDATAFACTORY_H
#define HISTORYDATAFACTORY_H

#include <QObject>
#include <QJsonArray>
#include <QUrl>
#include "tradedata_global.h"
#include "tradedata.h"
#include "dataproviderinfo.h"
#include "downloader.h"
#include "tradedataitem.h"

/*!
 * \brief 历史数据下载器。一个应用程序拥有一个历史数据下载器就足够了。
 */
class TRADEDATASHARED_EXPORT HistoryDataFactory : public QObject
{
    Q_OBJECT
public:
    explicit HistoryDataFactory(QObject *parent = 0);

    /*!
     * \brief 下载历史数据。
     * \param item 用于指定要下载的数据，并用于接收数据。
     */
    void request(TradeDataItem* item);

protected slots:
    /*!
     * \brief 用于解析并存储QNetworkReply对象接收到的数据。数据保存后，将由数据存储对象TradeDataItem来
     * 通知应用程序数据已更新的消息（通过TradeDataItem的信号。）。
     * \warning 这个槽函数只能和QNetworkReply对象绑定。
     */
    void onReplyFinished(bool isSucceed, int index, QByteArray data);

private:
    QMap<int, TradeDataItem*> replyMap;

    Downloader downloader;
};

#endif // HISTORYDATAFACTORY_H
