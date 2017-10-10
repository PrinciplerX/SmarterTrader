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
    if (column == "����") return 0;
    if (column == "����") return 1;
    if (column == "�Ƿ�%%") return 2;
    if (column == "�ּ�") return 3;
    if (column == "�ǵ�") return 4;
    if (column == "���") return 5;
    if (column == "����") return 6;
    if (column == "����") return 7;
    if (column == "����") return 8;
    if (column == "����%%") return 9;
    if (column == "����%%") return 10;
    if (column == "��") return 11;
    if (column == "���") return 12;
    if (column == "���") return 13;
    if (column == "����") return 14;
    if (column == "��ӯ(��)") return 15;
    if (column == "�ܽ��") return 16;
    if (column == "����") return 17;
    if (column == "ϸ����ҵ") return 18;
    if (column == "����") return 19;
    if (column == "���%%") return 20;
    if (column == "����") return 21;
    if (column == "����") return 22;
    if (column == "����") return 23;
    if (column == "�����") return 24;
    if (column == "����") return 25;
    if (column == "����") return 26;
    if (column == "δƥ����") return 27;
    if (column == "��ͨ��(��)") return 28;
    if (column == "��ͨ��ֵ") return 29;
    if (column == "AB������ֵ") return 30;
    if (column == "ǿ����%%") return 31;
    if (column == "��Ծ��") return 32;
    if (column == "�ʾ���") return 33;
    if (column == "�ʻ���") return 34;
    if (column == "������") return 35;
    if (column == "3���Ƿ�%%") return 36;
    if (column == "����ϵ��") return 37;
    if (column == "��ӯ(TTM)") return 38;
    if (column == "��ӯ(��)") return 39;
    if (column == "����%%") return 40;
    if (column == "���%%") return 41;
    if (column == "���%%") return 42;
    if (column == "���Ƿ�%%") return 43;
    if (column == "ʵ���Ƿ�%%") return 44;
    if (column == "��ͷ��%%") return 45;
    if (column == "������%%") return 46;
    if (column == "�������") return 47;
    if (column == "��������") return 48;
    if (column == "�ܹɱ�(��)") return 49;
    if (column == "B��(��A)") return 50;
    if (column == "H��(��)") return 51;
    if (column == "���ʲ�(��)") return 52;
    if (column == "���ʲ�") return 53;
    if (column == "������Ȩ") return 54;
    if (column == "�ʲ���ծ��%%") return 55;
    if (column == "�����ʲ�") return 56;
    if (column == "�̶��ʲ�") return 57;
    if (column == "�����ʲ�") return 58;
    if (column == "������ծ") return 59;
    if (column == "������") return 60;
    if (column == "���") return 61;
    if (column == "Ӧ���ʿ�") return 62;
    if (column == "Ӫҵ����") return 63;
    if (column == "Ӫҵ�ɱ�") return 64;
    if (column == "Ӫҵ����") return 65;
    if (column == "Ͷ������") return 66;
    if (column == "�����ܶ�") return 67;
    if (column == "˰������") return 68;
    if (column == "������") return 69;
    if (column == "δ������") return 70;
    if (column == "��Ӫ�ֽ���") return 71;
    if (column == "���ֽ���") return 72;
    if (column == "�ɶ�����") return 73;
    if (column == "�˾��ֹ�") return 74;
    if (column == "�˾���ֵ") return 75;
    if (column == "����ͬ��%%") return 76;
    if (column == "����ͬ��%%") return 77;
    if (column == "�о���") return 78;
    if (column == "������") return 79;
    if (column == "������") return 80;
    if (column == "ÿ������") return 81;
    if (column == "ÿ�ɾ���") return 82;
    if (column == "��������") return 83;
    if (column == "ÿ�ɹ���") return 84;
    if (column == "ÿ��δ����") return 85;
    if (column == "Ȩ���") return 86;
    if (column == "������%%") return 87;
    if (column == "ë����%%") return 88;
    if (column == "Ӫҵ������%%") return 89;
    if (column == "��������%%") return 90;
    if (column == "���״���") return 91;
    if (column == "��ѡ��") return 92;
    if (column == "��ѡ��") return 93;
    if (column == "��ѡ����%%") return 94;
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
    "����\t����\t�Ƿ�%%\t�ּ�\t�ǵ�\t���\t����\t����\t����\t����%%\t����%%\t��\t���\t���\t����\t��ӯ(��)\t"
    "�ܽ��\t����\tϸ����ҵ\t����\t���%%\t����\t����\t����\t�����\t����\t����\tδƥ����\t��ͨ��(��)\t��ͨ��ֵ\t"
    "AB������ֵ\tǿ����%%\t��Ծ��\t�ʾ���\t�ʻ���\t������\t3���Ƿ�%%\t����ϵ��\t��ӯ(TTM)\t��ӯ(��)\t����%%\t���%%\t"
    "���%%\t���Ƿ�%%\tʵ���Ƿ�%%\t��ͷ��%%\t������%%\t�������\t��������\t�ܹɱ�(��)\tB��(��A)\tH��(��)\t���ʲ�(��)\t"
    "���ʲ�\t������Ȩ\t�ʲ���ծ��%%\t�����ʲ�\t�̶��ʲ�\t�����ʲ�\t������ծ\t������\t���\tӦ���ʿ�\tӪҵ����\tӪҵ�ɱ�\t"
    "Ӫҵ����\tͶ������\t�����ܶ�\t˰������\t������\tδ������\t��Ӫ�ֽ���\t���ֽ���\t�ɶ�����\t�˾��ֹ�\t�˾���ֵ\t����ͬ��%%\t"
    "����ͬ��%%\t�о���\t������\t������\tÿ������\tÿ�ɾ���\t��������\tÿ�ɹ���\tÿ��δ����\tȨ���\t������%%\të����%%\t"
    "Ӫҵ������%%\t��������%%\t���״���\t��ѡ��\t��ѡ��\t��ѡ����%%\t\n");

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
