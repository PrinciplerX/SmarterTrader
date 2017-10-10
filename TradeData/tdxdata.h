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

#ifndef TDXDATA_H
#define TDXDATA_H

#include <QString>
#include <QVector>
#include <QMap>
#include "tradedata_global.h"

struct TdxStock
{
    QString code = "--";                               //!< ����
    QString name = "--";                               //!< ����
    QString growthRate = "--";                         //!< �Ƿ�%%
    QString price = "--";                              //!< �ּ�
    QString priceChange = "--";                        //!< �ǵ�
    QString buyPrice = "--";                           //!< ���
    QString sellPrice = "--";                          //!< ����
    QString volume = "--";                             //!< ����
    QString currVolume = "--";                         //!< ����
    QString growthSpeed = "--";                        //!< ����%%
    QString turnoverRate = "--";                       //!< ����%%
    QString open = "--";                               //!< ��
    QString high = "--";                               //!< ���
    QString low = "--";                                //!< ���
    QString lastClose = "--";                          //!< ����
    QString dynamicProfits = "--";                     //!< ��ӯ(��)
    QString amount = "--";                             //!< �ܽ��
    QString volumeRatio = "--";                        //!< ����
    QString industry = "--";                           //!< ϸ����ҵ
    QString region = "--";                             //!< ����
    QString swingRange = "--";                         //!< ���%%
    QString avgPrice = "--";                           //!< ����
    QString initiativeSellVolume = "--";               //!< ����
    QString initiativeBuyVolume = "--";                //!< ����
    QString initiativeVolumeRatio = "--";              //!< �����
    QString buyVolume = "--";                          //!< ����
    QString sellVolume = "--";                         //!< ����
    QString unmatchedVolume = "--";                    //!< δƥ����
    QString circulationStock = "--";                   //!< ��ͨ��(��)
    QString circulationMarketValue = "--";             //!< ��ͨ��ֵ
    QString abTotalValue = "--";                       //!< AB������ֵ
    QString strehgthRatio = "--";                      //!< ǿ����%%
    QString activeRatio = "--";                        //!< ��Ծ��
    QString volumePreTick = "--";                      //!< �ʾ���
    QString turnoverPreTick = "--";                    //!< �ʻ���
    QString riseDayCount = "--";                       //!< ������
    QString growthIn3days = "--";                      //!< 3���Ƿ�%%
    QString betaCoefficient = "--";                    //!< ����ϵ��
    QString ttm = "--";                                //!< ��ӯ(TTM)
    QString staticProfits = "--";                      //!< ��ӯ(��)
    QString openPrecent = "--";                        //!< ����%%
    QString highPrecent = "--";                        //!< ���%%
    QString lowPrecent = "--";                         //!< ���%%
    QString avgPrecent = "--";                         //!< ���Ƿ�%%
    QString entityPrecent = "--";                      //!< ʵ���Ƿ�%%
    QString pullbackPrecent = "--";                    //!< ��ͷ��%%
    QString pushPrecent = "--";                        //!< ������%%
    QString financeUpdate = "--";                      //!< �������
    QString listingDate = "--";                        //!< ��������
    QString equityBase = "--";                         //!< �ܹɱ�(��)
    QString bSharesOrA = "--";                         //!< B��(��A)
    QString hShares = "--";                            //!< H��(��)
    QString totalAssets = "--";                        //!< ���ʲ�(��)
    QString netAssets = "--";                          //!< ���ʲ�
    QString minorityInterest = "--";                   //!< ������Ȩ
    QString assetLiabilityRatio = "--";                //!< �ʲ���ծ��%%
    QString currentAssets = "--";                      //!< �����ʲ�
    QString plantAssets = "--";                        //!< �̶��ʲ�
    QString intangibleAssets = "--";                   //!< �����ʲ�
    QString currentLiabilities = "--";                 //!< ������ծ
    QString capitalSurplus = "--";                     //!< ������
    QString inventory = "--";                          //!< ���
    QString accountReceivable = "--";                  //!< Ӧ���ʿ�
    QString operatingReceipt = "--";                   //!< Ӫҵ����
    QString operatingCosts = "--";                     //!< Ӫҵ�ɱ�
    QString operatingProfit = "--";                    //!< Ӫҵ����
    QString incomeFromInvestment = "--";               //!< Ͷ������
    QString totalProfit = "--";                        //!< �����ܶ�
    QString afterTaxProfits = "--";                    //!< ˰������
    QString retainedProfits = "--";                    //!< ������
    QString undividedProfits = "--";                   //!< δ������
    QString operationalCashFlow = "--";                //!< ��Ӫ�ֽ���
    QString totalCashFlow = "--";                      //!< ���ֽ���
    QString shareHoldersCount = "--";                  //!< �ɶ�����
    QString avgShareHolded = "--";                     //!< �˾��ֹ�
    QString avgMarketValue = "--";                     //!< �˾���ֵ
    QString profitOnYoyBasis = "--";                   //!< ����ͬ��%%
    QString incomeOnYoyBasis = "--";                   //!< ����ͬ��%%
    QString pbRatio = "--";                            //!< �о���
    QString pcfRatio = "--";                           //!< ������
    QString priceSalesRatio = "--";                    //!< ������
    QString earningsPerShare = "--";                   //!< ÿ������
    QString netAssetPerShare = "--";                   //!< ÿ�ɾ���
    QString adjustedNetAssetPerShare = "--";           //!< ��������
    QString capitalSurplusPerShare = "--";             //!< ÿ�ɹ���
    QString undistributedProfitPreShare = "--";        //!< ÿ��δ����
    QString debtEquityRatio = "--";                    //!< Ȩ���
    QString roe = "--";                                //!< ������%%
    QString grossProfitRate = "--";                    //!< ë����%%
    QString operatingProfitRatio = "--";               //!< Ӫҵ������%%
    QString netProfitRate = "--";                      //!< ��������%%
    QString tradingCode = "--";                        //!< ���״���
    QString selfSelectDate = "--";                     //!< ��ѡ��
    QString selfSelectPrice = "--";                    //!< ��ѡ��
    QString selfSelectEarnings = "--";                 //!< ��ѡ����%%
};

class TRADEDATASHARED_EXPORT TdxDataFrameReader
{
public:
    bool readTdxFile(const QString& path);
    void update(int index, const QString& value, TdxStock& item);
    bool writeStdFile(const QString& path) const;

    const QMap<QString, TdxStock>& data() const;

private:
    QMap<QString, TdxStock> mData;
};

class TRADEDATASHARED_EXPORT TdxDataFrame
{
public:
    void update(const QString& line);

private:
    QVector<TdxStock> mData;
};

#endif // TDXDATA_H
