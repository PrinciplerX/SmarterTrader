#ifndef TICKMODEL_H
#define TICKMODEL_H

#include <QAbstractTableModel>
#include "tradedata.h"
#include "tradedata_global.h"

class TRADEDATASHARED_EXPORT TickModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TickModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const override;

    void setData(const QVector<TickData>& data);

private:
    QVector<TickData> rawData;
};

#endif // TICKMODEL_H
