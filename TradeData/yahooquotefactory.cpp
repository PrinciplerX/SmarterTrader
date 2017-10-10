#include "yahooquotefactory.h"
#include <QFileInfo>
#include "historydataparser.h"


YahooQuoteFactory::YahooQuoteFactory()
{

}

void YahooQuoteFactory::downloadYahooDailyData(const QString &symbol){
    QString standardSymbol = DataProviderInfo::getInstance().getStandardSymbol(DataProviderInfo::YAHOO, symbol);
    downloadFile(getYahooQuoteUrl(standardSymbol));
}

QString YahooQuoteFactory::getYahooQuoteUrl(const QString &symbol){
    return QString("http://table.finance.yahoo.com/table.csv?s=") + DataProviderInfo::getInstance().getStandardSymbol(DataProviderInfo::YAHOO, symbol);
}

bool YahooQuoteFactory::parseFile(const QString &fileName, TradeDataSeries &output)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    return HistoryDataParser::parseYahooReply(file.readAll(), output);
}

void YahooQuoteFactory::startRequest(QUrl url)
{
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(requestFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(requestReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void YahooQuoteFactory::downloadFile(QUrl url)
{
    QFileInfo fileInfo(url.path());
    fileName = fileInfo.fileName();
    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        delete file;
        file = 0;
        return;
    }

    // schedule the request
    httpRequestAborted = false;
    startRequest(url);
}

void YahooQuoteFactory::cancelDownload()
{
    httpRequestAborted = true;
    reply->abort();
}

void YahooQuoteFactory::requestFinished()
{
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        return;
    }

    file->flush();
    file->close();

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        emit downloadFailed();
        qDebug()<<tr("Download failed: %1.").arg(reply->errorString());
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        url = newUrl;
        reply->deleteLater();
        file->open(QIODevice::WriteOnly);
        file->resize(0);
        startRequest(url);
        return;
    }else{
        qDebug()<<"Download Succeed!";
        emit downloadComplete(QFileInfo(*file).absoluteFilePath());
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = nullptr;
}

void YahooQuoteFactory::requestReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

void YahooQuoteFactory::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;
    qDebug()<<"Read Progress: bytesRead = "<<bytesRead<<",totalBytes = "<<totalBytes;
}

#ifndef QT_NO_SSL
void YahooQuoteFactory::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    qDebug()<<tr("One or more SSL errors has occurred: %1").arg(errorString);
    reply->ignoreSslErrors();
}
#endif
