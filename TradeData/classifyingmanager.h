#ifndef CLASSIFYINGMANAGER_H
#define CLASSIFYINGMANAGER_H

#include <QObject>
#include "tradedata_global.h"
#include "tradedata.h"
#include "downloader.h"
#include <QtSql>

class TRADEDATASHARED_EXPORT ClassifyingManager : public QObject
{
    Q_OBJECT
public:
    static ClassifyingManager* instance();
    virtual ~ClassifyingManager();
    QStringList getIndustryList() const;
    QStringList getIndustryItems() const;

public slots:
    void updateIndustryDetail();
    void updateIndustryInfo();
    void updateIndustryDetail(QString industry);

protected slots:
    void onDownloadFinished(bool isSucceed, int index, QByteArray data);

protected:
    explicit ClassifyingManager(QObject *parent = 0);

    void loadIndustryData(const QByteArray rawData);
    void loadIndustryDetailData(const QString &industry, const QByteArray rawData);

private:
    QSqlDatabase database;
    static ClassifyingManager* mInstance;

    int industryInfoDownloadIndex = -1;
    QMap<int, QString> industryDetailDownloadMap;

    //! ¡ìindustry
    static const QString URL_SINA_INDUSTRY_INDEX;
    static const QString URL_SW_INDUSTRY_INDEX;
    static const QString UT_SINA_INDUSTRY_DETAIL;

    Downloader downloader;
};

#endif // CLASSIFYINGMANAGER_H
