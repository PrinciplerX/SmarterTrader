#include "tickmodel.h"
TickModel::TickModel()
{
}

int TickModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return rawData.size();
}

/*!
 * \brief 共4列数据：时间，成交价，成交手数，成交笔数
 */
int TickModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 4;
}

QVariant TickModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole){
        switch (index.column()) {
        case 0: /*时间*/  return rawData.at(index.row()).time.left(5);
        case 1: /*价格*/  return rawData.at(index.row()).price;
        case 2: /*手数*/  return rawData.at(index.row()).volume;
        case 3: /*笔数*/  return rawData.at(index.row()).type;
        default:         return QVariant();
        }
    }
    return QVariant();
}

QVariant TickModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
        case 0:return QString("时刻");
        case 1:return QString("成交价");
        case 2:return QString("成交量");
        case 3:return QString("类型");
        }
    }
    return QVariant();
}

void TickModel::setData(const QVector<TickData> &data){
    rawData = data;
}

