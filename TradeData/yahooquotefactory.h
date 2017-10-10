#ifndef YAHOOQUOTEFACTORY_H
#define YAHOOQUOTEFACTORY_H

#include "tradedata_global.h"
#include <QString>
#include <QObject>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QEventLoop>
#include <QVector>
#include "tradedata.h"
#include <QDebug>

class TRADEDATASHARED_EXPORT YahooQuoteFactory :public QObject
{
    Q_OBJECT

public:
    YahooQuoteFactory();

    void downloadYahooDailyData(const QString& symbol);
    static QString getYahooQuoteUrl(const QString& symbol);

    static bool parseFile(const QString& fileName, TradeDataSeries& output);

signals:
    void downloadComplete(const QString& fileName);
    void downloadFailed();

protected slots:
    void requestFinished();
    void requestReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);

protected:
    void startRequest(QUrl url);

    void downloadFile(QUrl url);
    void cancelDownload();
    void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
private:
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    QString fileName;
    int httpGetId;
    bool httpRequestAborted;
};

#endif // YAHOOQUOTEFACTORY_H
