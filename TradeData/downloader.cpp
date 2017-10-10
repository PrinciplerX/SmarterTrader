#include "downloader.h"

Downloader::Downloader(QObject *parent) : QObject(parent)
{
    connect(&qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadFinished(QNetworkReply*)));
}

int Downloader::download(QUrl url)
{
    QNetworkReply* reply = privateDownload(QNetworkRequest(url));
    int index = findReply(nullptr);

    if(index < 0){
        replyVector.append(reply);
        return replyVector.size() - 1;
    }

    replyVector[index] = reply;
    return index;
}

QNetworkReply *Downloader::privateDownload(const QNetworkRequest& request)
{
    return qnam.get(request);
}

void Downloader::cancelDownload(int index)
{
    if(replyVector.size() > index && index >= 0){
        if(!replyVector[index]){
            replyVector[index]->abort();
            replyVector[index]->deleteLater();
            replyVector[index] = nullptr;
        }
    }
}

int Downloader::findReply(QNetworkReply *reply) const
{
    return replyVector.indexOf(reply);
}

#ifndef QT_NO_SSL
void Downloader::sslErrors(QNetworkReply * reply, const QList<QSslError> &errors)
{
    QString errorString;
    foreach(const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    qDebug() << tr("One or more SSL errors has occurred: %1").arg(errorString);
    reply->ignoreSslErrors();
}
#endif

void Downloader::requestReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
}

void Downloader::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    Q_UNUSED(bytesRead)
    Q_UNUSED(totalBytes)
}

void Downloader::onDownloadFinished(QNetworkReply *reply)
{
    int index = replyVector.indexOf(reply);
    if(index < 0) return;

    QVariant redirectionTarget = replyVector[index]->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (replyVector[index]->error()) {
        emit downloadComplete(false, index, replyVector[index]->readAll());
        qDebug() << tr("Download failed: %1.").arg(replyVector[index]->errorString());
    }
    else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        url = newUrl;
        replyVector[index]->deleteLater();
        replyVector[index] = privateDownload(QNetworkRequest(url));
        return;
    }
    else{
        emit downloadComplete(true, index, replyVector[index]->readAll());
    }

    replyVector[index]->deleteLater();
    replyVector[index] = nullptr;
}
