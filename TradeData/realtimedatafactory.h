#ifndef REALTIMEDATAFACTORY_H
#define REALTIMEDATAFACTORY_H

#include <QObject>
#include <QUrl>
#include "tradedata_global.h"
#include "tradedata.h"
#include "realtimedataitem.h"
#include "downloader.h"
#include "dataproviderinfo.h"

class TRADEDATASHARED_EXPORT RealTimeDataFactory : public QObject
{
    Q_OBJECT
public:
    explicit RealTimeDataFactory(QObject *parent = 0);

    void request(QMap<QString, RealTimeDataItem*>* data);

protected slots:
    void onReplyFinished(bool isSucceed, int index, QByteArray data);

signals:
    void replyFinished();

private:
    QMap<QString, RealTimeDataItem*>* target;
    Downloader downloader;
    int index;
};

#endif // REALTIMEDATAFACTORY_H
