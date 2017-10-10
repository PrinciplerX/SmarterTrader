#ifndef TICKDATAFACTORY_H
#define TICKDATAFACTORY_H

#include <QVector>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QFile>
#include "tradedata.h"
#include "tradedata_global.h"
#include "downloader.h"
#include "dataproviderinfo.h"
#include "tickdataitem.h"

/*!
 * \brief The TickDataFactory class
 */
class TRADEDATASHARED_EXPORT TickDataFactory : public QObject
{
    Q_OBJECT
public:
    explicit TickDataFactory(QObject *parent = 0);
    static QVector<TickData> parse(const QString& fileData);
    static QVector<TickData> parseFile(const QString& fileName);
    static bool parseSinaReply(const QByteArray& reply, QVector<TickData>& output);

    /*!
     * \brief 下载历史数据。
     * \param item 用于指定要下载的数据，并用于接收数据。
     */
    void request(TickDataItem* item);

protected slots:
    /*!
     * \brief 用于解析并存储QNetworkReply对象接收到的数据。数据保存后，将由数据存储对象TradeDataItem来
     * 通知应用程序数据已更新的消息（通过TradeDataItem的信号。）。
     * \warning 这个槽函数只能和QNetworkReply对象绑定。
     */
    void onReplyFinished(bool isSucceed, int index, QByteArray data);

private:
    QMap<int, TickDataItem*> replyMap;

    QVector<QMap<QString, TickDataItem*>> mData;
    Downloader downloader;
};

#endif // TICKDATAFACTORY_H
