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
 * \brief ���Ϊ��������ܻ�������⡣
 */
class TRADEDATASHARED_EXPORT Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

    /*!
     * \brief �������ӣ���ʼ���ء�
     * \param url �������ӡ�
     * \return ���ڱ�ʶ��ǰ��������ı�ʶ��������ʱ��Ҫ���棬������ɺ��ʶ��ʧЧ��
     * \sa downloadComplete()
     */
    int download(QUrl url);

signals:
    /*!
     * \brief �����Ѿ���ɡ�
     * \param ���سɹ���Ϊtrue���м���ִ�����Ϊfalse��
     * \param index ���������ʶ����
     * \param reply �������ݡ�
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
