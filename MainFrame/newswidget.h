#ifndef NEWSWIDGET_H
#define NEWSWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include "mainframe_global.h"
#include "newssitewidget.h"
#include "newsmanager.h"
#include <QAbstractTableModel>
#include <QTreeView>

class NewsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    NewsModel(AbstractNewsReference* newsData, QObject *parent = nullptr) : QAbstractTableModel(parent)
    {
        mData = newsData;
        connect(mData, SIGNAL(dataLoaded()), this, SLOT(onNewsLoaded()));
    }

    virtual int rowCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent)
        return mData->size();
    }

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return 3;
    }

    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        if(index.row() >= rowCount(QModelIndex())) return QVariant();

        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0) return mData->at(index.row()).category;
            if(index.column() == 1) return mData->at(index.row()).datetime.toString(QString("yyyy-MM-dd HH:mm:ss"));
            if(index.column() == 2) return mData->at(index.row()).title;
        }
        if(role == Qt::ToolTipRole)
        {
            return mData->at(index.row()).url;
        }
        return QVariant();
    }

    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const override
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
            switch(section){
            case 0: return QString("市场");
            case 1: return QString("发布时间");
            case 2: return QString("标题");
            default: return QString("--");
            }
        }
        if(role == Qt::TextAlignmentRole){
            return Qt::AlignLeft;
        }
        return QAbstractItemModel::headerData(section, orientation, role);
    }

    virtual QModelIndex index(int row,int column, const QModelIndex& parent = QModelIndex()) const override
    {
        if(parent.isValid() && parent.column()!=0){
            return QModelIndex();
        }

        if(row>=0&&column>=0&&row<mData->size()&&column<3)
            return QAbstractItemModel::createIndex(row, column, (void*)&mData->at(row));
        else
            return QModelIndex();
    }

protected slots:
    void onNewsLoaded(){
        emit dataChanged(index(0, 0), index(rowCount(QModelIndex()), 0));
    }

private:
    AbstractNewsReference* mData;
};

class NewsWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit NewsWidget(QTabWidget* parent=nullptr);

public slots:
    void onNewsItemDoubleClicked(const QModelIndex &index);

private:
    NewsSiteWidget* newsSiteWidget;
    QTreeView* latestNewsList;
};

#endif // NEWSWIDGET_H
