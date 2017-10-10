#ifndef NEWSFACTORY_H
#define NEWSFACTORY_H

#include <QObject>
#include "tradedata.h"
#include "tradedata_global.h"
#include "downloader.h"

struct TRADEDATASHARED_EXPORT NewsReference{
    QString category;
    QString title;
    QDateTime datetime;
    QString url;
};

class TRADEDATASHARED_EXPORT AbstractNewsReference : public LoadableVector<NewsReference>
{
    Q_OBJECT
public:
    AbstractNewsReference(QObject* parent = nullptr) : LoadableVector<NewsReference>(parent) {}
    virtual QString category() const = 0;
};

class TRADEDATASHARED_EXPORT NewsReferenceItem : public AbstractNewsReference
{
    Q_OBJECT
public:
    NewsReferenceItem(const QString& category = QString("Unknown"), QObject* parent = nullptr):
        AbstractNewsReference(parent),
        mCategory(category)
    {

    }

    virtual int size() const override { return mData.size(); }
    virtual const NewsReference& at(int index) const override { return mData.at(index); }
    virtual const QVector<NewsReference>& data() const override { return mData; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 更新数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setData(const QVector<NewsReference>& data)
    {
        mData = data;
        emit dataLoaded();
    }

    virtual QString category() const override { return mCategory; }
    void setCategory(const QString& category) { mCategory = category; }

    void append(const NewsReference& item)
    {
        mData.append(item);
        emit dataAppended();
    }

private:
    QString mCategory;
    QVector<NewsReference> mData;
};

class TRADEDATASHARED_EXPORT NewsReferenceProxy : public AbstractNewsReference
{
    Q_OBJECT
public:
    NewsReferenceProxy(NewsReferenceItem* parent = nullptr) : AbstractNewsReference(parent)
    {
        mData = parent;
        connect(mData, SIGNAL(dataLoaded()), this, SIGNAL(dataLoaded()));
        connect(mData, SIGNAL(dataAppended()), this, SIGNAL(dataAppended()));
        connect(mData, SIGNAL(dataUpdated()), this, SIGNAL(dataUpdated()));
    }

    virtual ~NewsReferenceProxy() {}

    virtual int size() const override { return mData->size(); }
    virtual const NewsReference& at(int index) const override { return mData->at(index); }
    virtual const QVector<NewsReference>& data() const override { return mData->data(); }
    virtual QString category() const override { return mData->category(); }

private:
    NewsReferenceItem* mData;
};

class NewsFactory : public QObject
{
    Q_OBJECT
public:
    explicit NewsFactory(QObject *parent = 0);

    void requestLatestNews(NewsReferenceItem *item, int category = 43, int num = 30);

    static bool parseLatestNews(const QByteArray& reply, NewsReferenceItem *item);
signals:

protected slots:
    void onReplyFinished(bool isSucceed, int index, QByteArray data);

private:
    QMap<int, NewsReferenceItem*> replyMap;
    static const QString UT_LATEST_NEWS;
    Downloader downloader;
};

#endif // NEWSFACTORY_H
