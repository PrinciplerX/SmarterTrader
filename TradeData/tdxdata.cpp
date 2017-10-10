/*****************************************************************************
 *
 * Copyright (C) 2017 SuperTrader - All Rights Reserved
 *
 * This file is part of the TradeData module of SuperTrader.
 *
 * Author:       Yang Yichao
 * CreationDate: 2017-04-27
 *
 ****************************************************************************/

#include "tdxdata.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

int columnIndex(const QString& column)
{
    if (column == "代码") return 0;
    if (column == "名称") return 1;
    if (column == "涨幅%%") return 2;
    if (column == "现价") return 3;
    if (column == "涨跌") return 4;
    if (column == "买价") return 5;
    if (column == "卖价") return 6;
    if (column == "总量") return 7;
    if (column == "现量") return 8;
    if (column == "涨速%%") return 9;
    if (column == "换手%%") return 10;
    if (column == "今开") return 11;
    if (column == "最高") return 12;
    if (column == "最低") return 13;
    if (column == "昨收") return 14;
    if (column == "市盈(动)") return 15;
    if (column == "总金额") return 16;
    if (column == "量比") return 17;
    if (column == "细分行业") return 18;
    if (column == "地区") return 19;
    if (column == "振幅%%") return 20;
    if (column == "均价") return 21;
    if (column == "内盘") return 22;
    if (column == "外盘") return 23;
    if (column == "内外比") return 24;
    if (column == "买量") return 25;
    if (column == "卖量") return 26;
    if (column == "未匹配量") return 27;
    if (column == "流通股(亿)") return 28;
    if (column == "流通市值") return 29;
    if (column == "AB股总市值") return 30;
    if (column == "强弱度%%") return 31;
    if (column == "活跃度") return 32;
    if (column == "笔均量") return 33;
    if (column == "笔换手") return 34;
    if (column == "连涨天") return 35;
    if (column == "3日涨幅%%") return 36;
    if (column == "贝塔系数") return 37;
    if (column == "市盈(TTM)") return 38;
    if (column == "市盈(静)") return 39;
    if (column == "开盘%%") return 40;
    if (column == "最高%%") return 41;
    if (column == "最低%%") return 42;
    if (column == "均涨幅%%") return 43;
    if (column == "实体涨幅%%") return 44;
    if (column == "回头波%%") return 45;
    if (column == "攻击波%%") return 46;
    if (column == "财务更新") return 47;
    if (column == "上市日期") return 48;
    if (column == "总股本(亿)") return 49;
    if (column == "B股(或A)") return 50;
    if (column == "H股(亿)") return 51;
    if (column == "总资产(万)") return 52;
    if (column == "净资产") return 53;
    if (column == "少数股权") return 54;
    if (column == "资产负债率%%") return 55;
    if (column == "流动资产") return 56;
    if (column == "固定资产") return 57;
    if (column == "无形资产") return 58;
    if (column == "流动负债") return 59;
    if (column == "公积金") return 60;
    if (column == "存货") return 61;
    if (column == "应收帐款") return 62;
    if (column == "营业收入") return 63;
    if (column == "营业成本") return 64;
    if (column == "营业利润") return 65;
    if (column == "投资收益") return 66;
    if (column == "利润总额") return 67;
    if (column == "税后利润") return 68;
    if (column == "净利润") return 69;
    if (column == "未分利润") return 70;
    if (column == "经营现金流") return 71;
    if (column == "总现金流") return 72;
    if (column == "股东人数") return 73;
    if (column == "人均持股") return 74;
    if (column == "人均市值") return 75;
    if (column == "利润同比%%") return 76;
    if (column == "收入同比%%") return 77;
    if (column == "市净率") return 78;
    if (column == "市现率") return 79;
    if (column == "市销率") return 80;
    if (column == "每股收益") return 81;
    if (column == "每股净资") return 82;
    if (column == "调整后净资") return 83;
    if (column == "每股公积") return 84;
    if (column == "每股未分配") return 85;
    if (column == "权益比") return 86;
    if (column == "净益率%%") return 87;
    if (column == "毛利率%%") return 88;
    if (column == "营业利润率%%") return 89;
    if (column == "净利润率%%") return 90;
    if (column == "交易代码") return 91;
    if (column == "自选日") return 92;
    if (column == "自选价") return 93;
    if (column == "自选收益%%") return 94;
    qDebug() << "[TdxData] column(" << column << ") not found.";
    return -1;
}

bool TdxDataFrameReader::readTdxFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QTextCodec* codec = QTextCodec::codecForName("GB18030");
    QStringList contentList = codec->toUnicode(file.readAll()).split('\n');

    QStringList list = contentList.at(0).split('\t');
    int firstIndex = columnIndex(list.at(2));
    int indexCount = list.size() - 2;
    int lastIndex = firstIndex + indexCount - 1;

    for (const QString& line : contentList) {
        list = line.split('\t');
        if (list.size() < 2)
            break;

        QMap<QString, TdxStock>::iterator iterator = mData.find(list.at(0).trimmed());
        if (iterator == mData.end()) {
            TdxStock item;
            item.code = list.at(0).trimmed();
            item.name = list.at(1).trimmed();
            for (int i = firstIndex; i < lastIndex; i++) {
                update(i, list.at(i - firstIndex + 2).trimmed(), item);
            }
            mData.insert(item.code, item);
        } else {
            for (int i = firstIndex; i < lastIndex; i++) {
                update(i, list.at(i - firstIndex + 2).trimmed(), iterator.value());
            }
        }
    }

    file.close();
    return true;
}

void TdxDataFrameReader::update(int index, const QString &value, TdxStock &item)
{
    switch (index) {
    case 0:  item.code = value; break;
    case 1:  item.name = value; break;
    case 2:  item.growthRate = value; break;
    case 3:  item.price = value; break;
    case 4:  item.priceChange = value; break;
    case 5:  item.buyPrice = value; break;
    case 6:  item.sellPrice = value; break;
    case 7:  item.volume = value; break;
    case 8:  item.currVolume = value; break;
    case 9:  item.growthSpeed = value; break;
    case 10: item.turnoverRate = value; break;
    case 11: item.open = value; break;
    case 12: item.high = value; break;
    case 13: item.low = value; break;
    case 14: item.lastClose = value; break;
    case 15: item.dynamicProfits = value; break;
    case 16: item.amount = value; break;
    case 17: item.volumeRatio = value; break;
    case 18: item.industry = value; break;
    case 19: item.region = value; break;
    case 20: item.swingRange = value; break;
    case 21: item.avgPrice = value; break;
    case 22: item.initiativeSellVolume = value; break;
    case 23: item.initiativeBuyVolume = value; break;
    case 24: item.initiativeVolumeRatio = value; break;
    case 25: item.buyVolume = value; break;
    case 26: item.sellVolume = value; break;
    case 27: item.unmatchedVolume = value; break;
    case 28: item.circulationStock = value; break;
    case 29: item.circulationMarketValue = value; break;
    case 30: item.abTotalValue = value; break;
    case 31: item.strehgthRatio = value; break;
    case 32: item.activeRatio = value; break;
    case 33: item.volumePreTick = value; break;
    case 34: item.turnoverPreTick = value; break;
    case 35: item.riseDayCount = value; break;
    case 36: item.growthIn3days = value; break;
    case 37: item.betaCoefficient = value; break;
    case 38: item.ttm = value; break;
    case 39: item.staticProfits = value; break;
    case 40: item.openPrecent = value; break;
    case 41: item.highPrecent = value; break;
    case 42: item.lowPrecent = value; break;
    case 43: item.avgPrecent = value; break;
    case 44: item.entityPrecent = value; break;
    case 45: item.pullbackPrecent = value; break;
    case 46: item.pushPrecent = value; break;
    case 47: item.financeUpdate = value; break;
    case 48: item.listingDate = value; break;
    case 49: item.equityBase = value; break;
    case 50: item.bSharesOrA = value; break;
    case 51: item.hShares = value; break;
    case 52: item.totalAssets = value; break;
    case 53: item.netAssets = value; break;
    case 54: item.minorityInterest = value; break;
    case 55: item.assetLiabilityRatio = value; break;
    case 56: item.currentAssets = value; break;
    case 57: item.plantAssets = value; break;
    case 58: item.intangibleAssets = value; break;
    case 59: item.currentLiabilities = value; break;
    case 60: item.capitalSurplus = value; break;
    case 61: item.inventory = value; break;
    case 62: item.accountReceivable = value; break;
    case 63: item.operatingReceipt = value; break;
    case 64: item.operatingCosts = value; break;
    case 65: item.operatingProfit = value; break;
    case 66: item.incomeFromInvestment = value; break;
    case 67: item.totalProfit = value; break;
    case 68: item.afterTaxProfits = value; break;
    case 69: item.retainedProfits = value; break;
    case 70: item.undividedProfits = value; break;
    case 71: item.operationalCashFlow = value; break;
    case 72: item.totalCashFlow = value; break;
    case 73: item.shareHoldersCount = value; break;
    case 74: item.avgShareHolded = value; break;
    case 75: item.avgMarketValue = value; break;
    case 76: item.profitOnYoyBasis = value; break;
    case 77: item.incomeOnYoyBasis = value; break;
    case 78: item.pbRatio = value; break;
    case 79: item.pcfRatio = value; break;
    case 80: item.priceSalesRatio = value; break;
    case 81: item.earningsPerShare = value; break;
    case 82: item.netAssetPerShare = value; break;
    case 83: item.adjustedNetAssetPerShare = value; break;
    case 84: item.capitalSurplusPerShare = value; break;
    case 85: item.undistributedProfitPreShare = value; break;
    case 86: item.debtEquityRatio = value; break;
    case 87: item.roe = value; break;
    case 88: item.grossProfitRate = value; break;
    case 89: item.operatingProfitRatio = value; break;
    case 90: item.netProfitRate = value; break;
    case 91: item.tradingCode = value; break;
    case 92: item.selfSelectDate = value; break;
    case 93: item.selfSelectPrice = value; break;
    case 94: item.selfSelectEarnings = value; break;
    default:
        qDebug() << "[TdxDataFrameReader] [update] invalid index" << index;
        break;
    }
}

bool TdxDataFrameReader::writeStdFile(const QString &path) const
{
    QFile file(path);

    if (!file.exists()) {
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }

    if (!file.open(QFile::WriteOnly)) {
        return false;
    }

    QTextStream out(&file);

    out << QString(
    "代码\t名称\t涨幅%%\t现价\t涨跌\t买价\t卖价\t总量\t现量\t涨速%%\t换手%%\t今开\t最高\t最低\t昨收\t市盈(动)\t"
    "总金额\t量比\t细分行业\t地区\t振幅%%\t均价\t内盘\t外盘\t内外比\t买量\t卖量\t未匹配量\t流通股(亿)\t流通市值\t"
    "AB股总市值\t强弱度%%\t活跃度\t笔均量\t笔换手\t连涨天\t3日涨幅%%\t贝塔系数\t市盈(TTM)\t市盈(静)\t开盘%%\t最高%%\t"
    "最低%%\t均涨幅%%\t实体涨幅%%\t回头波%%\t攻击波%%\t财务更新\t上市日期\t总股本(亿)\tB股(或A)\tH股(亿)\t总资产(万)\t"
    "净资产\t少数股权\t资产负债率%%\t流动资产\t固定资产\t无形资产\t流动负债\t公积金\t存货\t应收帐款\t营业收入\t营业成本\t"
    "营业利润\t投资收益\t利润总额\t税后利润\t净利润\t未分利润\t经营现金流\t总现金流\t股东人数\t人均持股\t人均市值\t利润同比%%\t"
    "收入同比%%\t市净率\t市现率\t市销率\t每股收益\t每股净资\t调整后净资\t每股公积\t每股未分配\t权益比\t净益率%%\t毛利率%%\t"
    "营业利润率%%\t净利润率%%\t交易代码\t自选日\t自选价\t自选收益%%\t\n");

    for (const TdxStock& item : mData ) {
        out << item.code << '\t';
        out << item.name << '\t';
        out << item.growthRate << '\t';
        out << item.price << '\t';
        out << item.priceChange << '\t';
        out << item.buyPrice << '\t';
        out << item.sellPrice << '\t';
        out << item.volume << '\t';
        out << item.currVolume << '\t';
        out << item.growthSpeed << '\t';
        out << item.turnoverRate << '\t';
        out << item.open << '\t';
        out << item.high << '\t';
        out << item.low << '\t';
        out << item.lastClose << '\t';
        out << item.dynamicProfits << '\t';
        out << item.amount << '\t';
        out << item.volumeRatio << '\t';
        out << item.industry << '\t';
        out << item.region << '\t';
        out << item.swingRange << '\t';
        out << item.avgPrice << '\t';
        out << item.initiativeSellVolume << '\t';
        out << item.initiativeBuyVolume << '\t';
        out << item.initiativeVolumeRatio << '\t';
        out << item.buyVolume << '\t';
        out << item.sellVolume << '\t';
        out << item.unmatchedVolume << '\t';
        out << item.circulationStock << '\t';
        out << item.circulationMarketValue << '\t';
        out << item.abTotalValue << '\t';
        out << item.strehgthRatio << '\t';
        out << item.activeRatio << '\t';
        out << item.volumePreTick << '\t';
        out << item.turnoverPreTick << '\t';
        out << item.riseDayCount << '\t';
        out << item.growthIn3days << '\t';
        out << item.betaCoefficient << '\t';
        out << item.ttm << '\t';
        out << item.staticProfits << '\t';
        out << item.openPrecent << '\t';
        out << item.highPrecent << '\t';
        out << item.lowPrecent << '\t';
        out << item.avgPrecent << '\t';
        out << item.entityPrecent << '\t';
        out << item.pullbackPrecent << '\t';
        out << item.pushPrecent << '\t';
        out << item.financeUpdate << '\t';
        out << item.listingDate << '\t';
        out << item.equityBase << '\t';
        out << item.bSharesOrA << '\t';
        out << item.hShares << '\t';
        out << item.totalAssets << '\t';
        out << item.netAssets << '\t';
        out << item.minorityInterest << '\t';
        out << item.assetLiabilityRatio << '\t';
        out << item.currentAssets << '\t';
        out << item.plantAssets << '\t';
        out << item.intangibleAssets << '\t';
        out << item.currentLiabilities << '\t';
        out << item.capitalSurplus << '\t';
        out << item.inventory << '\t';
        out << item.accountReceivable << '\t';
        out << item.operatingReceipt << '\t';
        out << item.operatingCosts << '\t';
        out << item.operatingProfit << '\t';
        out << item.incomeFromInvestment << '\t';
        out << item.totalProfit << '\t';
        out << item.afterTaxProfits << '\t';
        out << item.retainedProfits << '\t';
        out << item.undividedProfits << '\t';
        out << item.operationalCashFlow << '\t';
        out << item.totalCashFlow << '\t';
        out << item.shareHoldersCount << '\t';
        out << item.avgShareHolded << '\t';
        out << item.avgMarketValue << '\t';
        out << item.profitOnYoyBasis << '\t';
        out << item.incomeOnYoyBasis << '\t';
        out << item.pbRatio << '\t';
        out << item.pcfRatio << '\t';
        out << item.priceSalesRatio << '\t';
        out << item.earningsPerShare << '\t';
        out << item.netAssetPerShare << '\t';
        out << item.adjustedNetAssetPerShare << '\t';
        out << item.capitalSurplusPerShare << '\t';
        out << item.undistributedProfitPreShare << '\t';
        out << item.debtEquityRatio << '\t';
        out << item.roe << '\t';
        out << item.grossProfitRate << '\t';
        out << item.operatingProfitRatio << '\t';
        out << item.netProfitRate << '\t';
        out << item.tradingCode << '\t';
        out << item.selfSelectDate << '\t';
        out << item.selfSelectPrice << '\t';
        out << item.selfSelectEarnings << '\t';
        out << "\n";
    }

    file.close();
    return true;
}

const QMap<QString, TdxStock> &TdxDataFrameReader::data() const
{
    return mData;
}
