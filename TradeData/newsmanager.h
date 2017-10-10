#ifndef NEWSMANAGER_H
#define NEWSMANAGER_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "newsfactory.h"

class TRADEDATASHARED_EXPORT NewsManager : public QObject
{
    Q_OBJECT
public:
    static NewsManager& instance();

    AbstractNewsReference *latestNews();
    AbstractNewsReference *customNews(int catogory, int num);

public slots:
    void updateLatest(int num);

signals:
    void latestNewsUpdated();

private:
    explicit NewsManager(QObject* parent = nullptr);

    Q_DISABLE_COPY(NewsManager)

    NewsReferenceItem* mLatestNews;
    NewsFactory factory;
    static NewsManager* mInstance;
};

#endif // NEWSMANAGER_H
