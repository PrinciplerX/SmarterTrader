#include "favoritelistmodel.h"
#include <QTextCodec>
#include <QPushButton>
#include <QAbstractItemModel>
#include <realtimedatamanager.h>
#include <QDebug>

FavoriteListModel::FavoriteListModel(QObject *parent) : QAbstractTableModel(parent)
{
    headers.resize(15);
    headers[0] = tr("����");
    headers[1] = tr("����");
    headers[2] = tr("�Ƿ�%");
    headers[3] = tr("�ּ�");
    headers[4] = tr("�ǵ�");
    headers[5] = tr("����%");
    headers[6] = tr("����");
    headers[7] = tr("��ҵ");
    headers[8] = tr("ί��%");
    headers[9] = tr("����");
    headers[10] = tr("����");
    headers[11] = tr("����ֵ(��)");
    headers[12] = tr("��ͨ��ֵ(��)");
    headers[13] = tr("��ӯ��");
    headers[14] = tr("�о���");

    dataVector.resize(11);

    dataVector[0] = RealTimeDataManager::instance().getRealTimeData(QString("002195"));
    dataVector[1] = RealTimeDataManager::instance().getRealTimeData(QString("002792"));
    dataVector[2] = RealTimeDataManager::instance().getRealTimeData(QString("000029"));
    dataVector[3] = RealTimeDataManager::instance().getRealTimeData(QString("600446"));
    dataVector[4] = RealTimeDataManager::instance().getRealTimeData(QString("000709"));
    dataVector[5] = RealTimeDataManager::instance().getRealTimeData(QString("600503"));
    dataVector[6] = RealTimeDataManager::instance().getRealTimeData(QString("sh000001"));
    dataVector[7] = RealTimeDataManager::instance().getRealTimeData(QString("002312"));
    dataVector[8] = RealTimeDataManager::instance().getRealTimeData(QString("300251"));
    dataVector[9] = RealTimeDataManager::instance().getRealTimeData(QString("601099"));
    dataVector[10] = RealTimeDataManager::instance().getRealTimeData(QString("603866"));

    connect((QObject*)&RealTimeDataManager::instance(), SIGNAL(dataUpdated()), this, SLOT(refreshData()));
}

FavoriteListModel::~FavoriteListModel(){
}

QModelIndex FavoriteListModel::index(int row,int column,const QModelIndex& parent) const{
    if(parent.isValid() && parent.column()!=0){
        return QModelIndex();
    }

    if(row>=0&&column>=0&&row<dataVector.size()&&column<headers.size())
        return QAbstractItemModel::createIndex(row, column, (void*)&dataVector.at(row));
    else
        return QModelIndex();
}

void FavoriteListModel::refreshData(){
    emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

AbstractRealTimeData *FavoriteListModel::getItem(const QModelIndex &index) const{
    return *static_cast<AbstractRealTimeData**>(index.internalPointer());
}

QString FavoriteListModel::getSymbol(int row) const {
    return dataVector[row]->getQuoteData()->symbol;
}

int FavoriteListModel::rowCount(const QModelIndex &parent) const{
    return dataVector.size();
}

int FavoriteListModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return headers.size();
}

QVariant FavoriteListModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::TextAlignmentRole){
        switch (index.column()) {
        case 0: return Qt::AlignCenter;
        case 1: return Qt::AlignLeft;
        default:return Qt::AlignRight;
        }
    }

    AbstractRealTimeData* item = getItem(index);

    if(role == Qt::BackgroundRole){
        if(item->getQuoteData()->changeRate.startsWith('+'))
            return QColor("#FF3030");
        else
            return QColor("#00A800");
    }

    if(role == Qt::ForegroundRole){
        if(item->getQuoteData()->changeRate.startsWith('+'))
            return QColor(Qt::white);
        else
            return QColor(Qt::black);
    }

    if(role != Qt::DisplayRole)
        return QVariant();

    //����������ʾ
    switch (index.column()) {
    case 0: return item->getQuoteData()->symbol;            //����
    case 1: return item->getQuoteData()->name;              //����
    case 2: return item->getQuoteData()->changeRate;        //�Ƿ�
    case 3: return item->getQuoteData()->current;           //�ּ�
    case 4: return item->getQuoteData()->change;            //�ǵ�
    case 5: return item->getQuoteData()->turnoverRate;      //������
    case 11:return item->getQuoteData()->totalMktCap;
    case 12:return item->getQuoteData()->negotiableCap;
    case 13:return item->getQuoteData()->pe;
    case 14:return item->getQuoteData()->pb;
    default: return QString("--");
    }
}
QVariant FavoriteListModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if(section < headers.size())
            return headers.at(section);
        else
            return QString("[No Title]");
    }

    if(role == Qt::ToolTipRole){
        return QString("ToolTip");
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

SnapshotModel::SnapshotModel(const QMap<QString, TdxStock> &data, QObject *parent) : QAbstractTableModel(parent)
{
    for (const TdxStock& item : data) {
        mData.append(item);
    }

    initHeader(mHeaders);
}

SnapshotModel::~SnapshotModel()
{

}

void SnapshotModel::initHeader(QVector<QString> &headers)
{
    headers << "����" << "����" << "�Ƿ�%%" << "�ּ�" << "�ǵ�" << "���" << "����" << "����" << "����"
             << "����%%" << "����%%" << "��" << "���" << "���" << "����" << "��ӯ(��)" << "�ܽ��"
             << "����" << "ϸ����ҵ" << "����" << "���%%" << "����" << "����" << "����" << "�����"
             << "����" << "����" << "δƥ����" << "��ͨ��(��)" << "��ͨ��ֵ" << "AB������ֵ" << "ǿ����%%"
             << "��Ծ��" << "�ʾ���" << "�ʻ���" << "������" << "3���Ƿ�%%" << "����ϵ��" << "��ӯ(TTM)"
             << "��ӯ(��)" << "����%%" << "���%%" << "���%%" << "���Ƿ�%%" << "ʵ���Ƿ�%%" << "��ͷ��%%"
             << "������%%" << "�������" << "��������" << "�ܹɱ�(��)" << "B��(��A)" << "H��(��)" << "���ʲ�(��)"
             << "���ʲ�" << "������Ȩ" << "�ʲ���ծ��%%" << "�����ʲ�" << "�̶��ʲ�" << "�����ʲ�" << "������ծ"
             << "������" << "���" << "Ӧ���ʿ�" << "Ӫҵ����" << "Ӫҵ�ɱ�" << "Ӫҵ����" << "Ͷ������"
             << "�����ܶ�" << "˰������" << "������" << "δ������" << "��Ӫ�ֽ���" << "���ֽ���" << "�ɶ�����"
             << "�˾��ֹ�" << "�˾���ֵ" << "����ͬ��%%" << "����ͬ��%%" << "�о���" << "������" << "������"
             << "ÿ������" << "ÿ�ɾ���" << "��������" << "ÿ�ɹ���" << "ÿ��δ����" << "Ȩ���" << "������%%"
             << "ë����%%" << "Ӫҵ������%%" << "��������%%" << "���״���" << "��ѡ��" << "��ѡ��" << "��ѡ����%%";
}

int SnapshotModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mData.size() - 1;
}

int SnapshotModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mHeaders.size();
}

QVariant SnapshotModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::TextAlignmentRole){
        switch (index.column()) {
        case 0: return Qt::AlignCenter;
        case 1: return Qt::AlignLeft;
        default:return Qt::AlignRight;
        }
    }

    const TdxStock& item = mData.at(index.row());

    if(role == Qt::BackgroundRole){
        if(item.growthRate.startsWith('-'))
            return QColor("#00A800");
        else
            return QColor("#FF3030");
    }

    if(role == Qt::ForegroundRole){
        if(item.growthRate.startsWith('-'))
            return QColor(Qt::black);
        else
            return QColor(Qt::white);
    }

    if(role != Qt::DisplayRole)
        return QVariant();

    //����������ʾ
    switch (index.column()) {
    case 0:  return item.code;
    case 1:  return item.name;
    case 2:  return item.growthRate;
    case 3:  return item.price;
    case 4:  return item.priceChange;
    case 5:  return item.buyPrice;
    case 6:  return item.sellPrice;
    case 7:  return item.volume;
    case 8:  return item.currVolume;
    case 9:  return item.growthSpeed;
    case 10: return item.turnoverRate;
    case 11: return item.open;
    case 12: return item.high;
    case 13: return item.low;
    case 14: return item.lastClose;
    case 15: return item.dynamicProfits;
    case 16: return item.amount;
    case 17: return item.volumeRatio;
    case 18: return item.industry;
    case 19: return item.region;
    case 20: return item.swingRange;
    case 21: return item.avgPrice;
    case 22: return item.initiativeSellVolume;
    case 23: return item.initiativeBuyVolume;
    case 24: return item.initiativeVolumeRatio;
    case 25: return item.buyVolume;
    case 26: return item.sellVolume;
    case 27: return item.unmatchedVolume;
    case 28: return item.circulationStock;
    case 29: return item.circulationMarketValue;
    case 30: return item.abTotalValue;
    case 31: return item.strehgthRatio;
    case 32: return item.activeRatio;
    case 33: return item.volumePreTick;
    case 34: return item.turnoverPreTick;
    case 35: return item.riseDayCount;
    case 36: return item.growthIn3days;
    case 37: return item.betaCoefficient;
    case 38: return item.ttm;
    case 39: return item.staticProfits;
    case 40: return item.openPrecent;
    case 41: return item.highPrecent;
    case 42: return item.lowPrecent;
    case 43: return item.avgPrecent;
    case 44: return item.entityPrecent;
    case 45: return item.pullbackPrecent;
    case 46: return item.pushPrecent;
    case 47: return item.financeUpdate;
    case 48: return item.listingDate;
    case 49: return item.equityBase;
    case 50: return item.bSharesOrA;
    case 51: return item.hShares;
    case 52: return item.totalAssets;
    case 53: return item.netAssets;
    case 54: return item.minorityInterest;
    case 55: return item.assetLiabilityRatio;
    case 56: return item.currentAssets;
    case 57: return item.plantAssets;
    case 58: return item.intangibleAssets;
    case 59: return item.currentLiabilities;
    case 60: return item.capitalSurplus;
    case 61: return item.inventory;
    case 62: return item.accountReceivable;
    case 63: return item.operatingReceipt;
    case 64: return item.operatingCosts;
    case 65: return item.operatingProfit;
    case 66: return item.incomeFromInvestment;
    case 67: return item.totalProfit;
    case 68: return item.afterTaxProfits;
    case 69: return item.retainedProfits;
    case 70: return item.undividedProfits;
    case 71: return item.operationalCashFlow;
    case 72: return item.totalCashFlow;
    case 73: return item.shareHoldersCount;
    case 74: return item.avgShareHolded;
    case 75: return item.avgMarketValue;
    case 76: return item.profitOnYoyBasis;
    case 77: return item.incomeOnYoyBasis;
    case 78: return item.pbRatio;
    case 79: return item.pcfRatio;
    case 80: return item.priceSalesRatio;
    case 81: return item.earningsPerShare;
    case 82: return item.netAssetPerShare;
    case 83: return item.adjustedNetAssetPerShare;
    case 84: return item.capitalSurplusPerShare;
    case 85: return item.undistributedProfitPreShare;
    case 86: return item.debtEquityRatio;
    case 87: return item.roe;
    case 88: return item.grossProfitRate;
    case 89: return item.operatingProfitRatio;
    case 90: return item.netProfitRate;
    case 91: return item.tradingCode;
    case 92: return item.selfSelectDate;
    case 93: return item.selfSelectPrice;
    case 94: return item.selfSelectEarnings;
    default: return QString("--");
    }
}

QVariant SnapshotModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if(section < mHeaders.size())
            return mHeaders.at(section);
        else
            return QString("[No Title]");
    }

    if(role == Qt::ToolTipRole){
        return QString("ToolTip");
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

QModelIndex SnapshotModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0){
        return QModelIndex();
    }

    if(row >= 0 && column >= 0 && row < mData.size() && column < mHeaders.size())
        return QAbstractItemModel::createIndex(row, column, (void*)&mData[row]);
    else
        return QModelIndex();
}

TdxStock *SnapshotModel::getItem(const QModelIndex &index) const { return *static_cast<TdxStock**>(index.internalPointer()); }
