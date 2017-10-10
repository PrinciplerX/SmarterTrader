#ifndef FAVORITELISTMODEL_H
#define FAVORITELISTMODEL_H

#include <QAbstractTableModel>
#include <tradedata.h>
#include <QTimer>
#include <QByteArray>
#include <QGraphicsItem>
#include <realtimedatamanager.h>
#include "tdxdata.h"

class FavoriteListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FavoriteListModel(QObject *parent = 0);
    ~FavoriteListModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const override;

    virtual QModelIndex index(int row,int column, const QModelIndex& parent=QModelIndex()) const override;

    AbstractRealTimeData* getItem(const QModelIndex& index) const;

    QString getSymbol(int row) const;
    QString getSymbol(const QModelIndex& index) const;

public slots:
    /*! \brief 更新列表的数据，可能会比较消耗时间，建议使用一个单独的线程来操作。 */
    void refreshData();

private:
    RealTimeDataManager* manager;
    QVector<QuoteData> stockVector;
    QVector<const AbstractRealTimeData*> dataVector;
    QVector<QString> headers;
    QTimer* refreshTimer;
};

inline QString FavoriteListModel::getSymbol(const QModelIndex &index) const {
    return getSymbol(index.row());
}

class SnapshotModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SnapshotModel(const QMap<QString, TdxStock>& data, QObject *parent = 0);
    ~SnapshotModel();

    static void initHeader(QVector<QString>& headers);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const override;

    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    TdxStock* getItem(const QModelIndex& index) const;

    QString getSymbol(int row) const;
    QString getSymbol(const QModelIndex& index) const;

private:
    QVector<QString> mHeaders;
    QVector<TdxStock> mData;
};


#endif // FAVORITELISTMODEL_H
