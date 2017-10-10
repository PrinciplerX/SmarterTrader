#include "tickmodel.h"
TickModel::TickModel()
{
}

int TickModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return rawData.size();
}

/*!
 * \brief ��4�����ݣ�ʱ�䣬�ɽ��ۣ��ɽ��������ɽ�����
 */
int TickModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 4;
}

QVariant TickModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole){
        switch (index.column()) {
        case 0: /*ʱ��*/  return rawData.at(index.row()).time.left(5);
        case 1: /*�۸�*/  return rawData.at(index.row()).price;
        case 2: /*����*/  return rawData.at(index.row()).volume;
        case 3: /*����*/  return rawData.at(index.row()).type;
        default:         return QVariant();
        }
    }
    return QVariant();
}

QVariant TickModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
        case 0:return QString("ʱ��");
        case 1:return QString("�ɽ���");
        case 2:return QString("�ɽ���");
        case 3:return QString("����");
        }
    }
    return QVariant();
}

void TickModel::setData(const QVector<TickData> &data){
    rawData = data;
}

