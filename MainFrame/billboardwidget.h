#ifndef BILLBOARDWIDGET_H
#define BILLBOARDWIDGET_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QTreeView>
#include "billboardmanger.h"

class BillboardModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    BillboardModel(AbstractBillboardData* data, QObject *parent = nullptr) : QAbstractTableModel(parent)
    {
        mData = data;
        connect(mData, SIGNAL(dataLoaded()), this, SLOT(onLoaded()));
    }

    virtual int rowCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent)
        return mData->size();
    }

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return 7;
    }

    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        if(index.row() >= rowCount(QModelIndex())) return QVariant();

        if(role == Qt::DisplayRole)
        {
            switch(index.column())
            {
            case 0: return mData->at(index.row()).symbol;
            case 1: return mData->at(index.row()).name;
            case 2: return mData->at(index.row()).changeRate;
            case 3: return mData->at(index.row()).buyratio;
            case 4: return mData->at(index.row()).sellratio;
            case 5: return mData->at(index.row()).reason;
            case 6: return mData->at(index.row()).date;
            default: return QString("--");
            }
        }
        if(role == Qt::ToolTipRole)
        {
            return mData->at(index.row()).reason;
        }
        return QVariant();
    }

    virtual QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const override
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
            switch(section){
            case 0: return QString("代码");
            case 1: return QString("名称");
            case 2: return QString("当日涨跌幅");
            case 3: return QString("买入率");
            case 4: return QString("卖出率");
            case 5: return QString("上榜原因");
            case 6: return QString("日期");
            default: return QString("--");
            }
        }
        if(role == Qt::TextAlignmentRole){
            return Qt::AlignLeft;
        }
        return QAbstractItemModel::headerData(section, orientation, role);
    }

    virtual QModelIndex index(int row,int column, const QModelIndex& parent=QModelIndex()) const override
    {
        if(parent.isValid() && parent.column()!=0){
            return QModelIndex();
        }

        if(row>=0&&column>=0&&row<mData->size()&&column<columnCount())
            return QAbstractItemModel::createIndex(row, column, (void*)&mData->at(row));
        else
            return QModelIndex();
    }

protected slots:
    void onLoaded(){
        emit dataChanged(index(0, 0), index(rowCount(QModelIndex()), 0));
    }

private:
    AbstractBillboardData* mData;
};

class BillboardWidget : public QTreeView
{
    Q_OBJECT
public:
    explicit BillboardWidget(QWidget *parent = 0);

signals:

public slots:

};

#endif // BILLBOARDWIDGET_H
