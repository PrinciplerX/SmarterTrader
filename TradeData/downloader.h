#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
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

/*!
 * \brief 设计为单例类可能会出现问题。
 */
class TRADEDATASHARED_EXPORT Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

    /*!
     * \brief 给定链接，开始下载。
     * \param url 下载链接。
     * \return 用于标识当前下载任务的标识符，下载时需要保存，下载完成后标识符失效。
     * \sa downloadComplete()
     */
    int download(QUrl url);

signals:
    /*!
     * \brief 下载已经完成。
     * \param 下载成功则为true，中间出现错误则为false。
     * \param index 下载任务标识符。
     * \param reply 下载内容。
     * \sa download()
     */
    void downloadComplete(bool isSucceed, int index, const QByteArray& reply);

protected:
    QNetworkReply* privateDownload(const QNetworkRequest &request);

    void cancelDownload(int index);
    int findReply(QNetworkReply *reply) const;

#ifndef QT_NO_SSL
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif

protected slots:
    //void requestFinished();
    void requestReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);

    void onDownloadFinished(QNetworkReply* reply);

private:
    QUrl url;
    QNetworkAccessManager qnam;
    QVector<QNetworkReply*> replyVector;
    int httpGetId;
    bool httpRequestAborted;
};

#endif // DOWNLOADER_H
