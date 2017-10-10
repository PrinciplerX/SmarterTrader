#include "newsmanager.h"

NewsManager* NewsManager::mInstance = nullptr;

NewsManager::NewsManager(QObject *parent):
    QObject(parent)
{
    mLatestNews = new NewsReferenceItem();
}

NewsManager &NewsManager::instance()
{
    if(!mInstance)
        mInstance = new NewsManager();
    return *mInstance;
}

AbstractNewsReference *NewsManager::latestNews()
{
    return new NewsReferenceProxy(mLatestNews);
}

AbstractNewsReference *NewsManager::customNews(int catogory, int num)
{
    NewsReferenceItem* item = new NewsReferenceItem();
    factory.requestLatestNews(item, catogory, num);
    return item;
}

void NewsManager::updateLatest(int num)
{
    factory.requestLatestNews(mLatestNews, 43, num);
}
