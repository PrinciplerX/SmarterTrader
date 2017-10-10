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
    QString code = "--";                               //!< 代码
    QString name = "--";                               //!< 名称
    QString growthRate = "--";                         //!< 涨幅%%
    QString price = "--";                              //!< 现价
    QString priceChange = "--";                        //!< 涨跌
    QString buyPrice = "--";                           //!< 买价
    QString sellPrice = "--";                          //!< 卖价
    QString volume = "--";                             //!< 总量
    QString currVolume = "--";                         //!< 现量
    QString growthSpeed = "--";                        //!< 涨速%%
    QString turnoverRate = "--";                       //!< 换手%%
    QString open = "--";                               //!< 今开
    QString high = "--";                               //!< 最高
    QString low = "--";                                //!< 最低
    QString lastClose = "--";                          //!< 昨收
    QString dynamicProfits = "--";                     //!< 市盈(动)
    QString amount = "--";                             //!< 总金额
    QString volumeRatio = "--";                        //!< 量比
    QString industry = "--";                           //!< 细分行业
    QString region = "--";                             //!< 地区
    QString swingRange = "--";                         //!< 振幅%%
    QString avgPrice = "--";                           //!< 均价
    QString initiativeSellVolume = "--";               //!< 内盘
    QString initiativeBuyVolume = "--";                //!< 外盘
    QString initiativeVolumeRatio = "--";              //!< 内外比
    QString buyVolume = "--";                          //!< 买量
    QString sellVolume = "--";                         //!< 卖量
    QString unmatchedVolume = "--";                    //!< 未匹配量
    QString circulationStock = "--";                   //!< 流通股(亿)
    QString circulationMarketValue = "--";             //!< 流通市值
    QString abTotalValue = "--";                       //!< AB股总市值
    QString strehgthRatio = "--";                      //!< 强弱度%%
    QString activeRatio = "--";                        //!< 活跃度
    QString volumePreTick = "--";                      //!< 笔均量
    QString turnoverPreTick = "--";                    //!< 笔换手
    QString riseDayCount = "--";                       //!< 连涨天
    QString growthIn3days = "--";                      //!< 3日涨幅%%
    QString betaCoefficient = "--";                    //!< 贝塔系数
    QString ttm = "--";                                //!< 市盈(TTM)
    QString staticProfits = "--";                      //!< 市盈(静)
    QString openPrecent = "--";                        //!< 开盘%%
    QString highPrecent = "--";                        //!< 最高%%
    QString lowPrecent = "--";                         //!< 最低%%
    QString avgPrecent = "--";                         //!< 均涨幅%%
    QString entityPrecent = "--";                      //!< 实体涨幅%%
    QString pullbackPrecent = "--";                    //!< 回头波%%
    QString pushPrecent = "--";                        //!< 攻击波%%
    QString financeUpdate = "--";                      //!< 财务更新
    QString listingDate = "--";                        //!< 上市日期
    QString equityBase = "--";                         //!< 总股本(亿)
    QString bSharesOrA = "--";                         //!< B股(或A)
    QString hShares = "--";                            //!< H股(亿)
    QString totalAssets = "--";                        //!< 总资产(万)
    QString netAssets = "--";                          //!< 净资产
    QString minorityInterest = "--";                   //!< 少数股权
    QString assetLiabilityRatio = "--";                //!< 资产负债率%%
    QString currentAssets = "--";                      //!< 流动资产
    QString plantAssets = "--";                        //!< 固定资产
    QString intangibleAssets = "--";                   //!< 无形资产
    QString currentLiabilities = "--";                 //!< 流动负债
    QString capitalSurplus = "--";                     //!< 公积金
    QString inventory = "--";                          //!< 存货
    QString accountReceivable = "--";                  //!< 应收帐款
    QString operatingReceipt = "--";                   //!< 营业收入
    QString operatingCosts = "--";                     //!< 营业成本
    QString operatingProfit = "--";                    //!< 营业利润
    QString incomeFromInvestment = "--";               //!< 投资收益
    QString totalProfit = "--";                        //!< 利润总额
    QString afterTaxProfits = "--";                    //!< 税后利润
    QString retainedProfits = "--";                    //!< 净利润
    QString undividedProfits = "--";                   //!< 未分利润
    QString operationalCashFlow = "--";                //!< 经营现金流
    QString totalCashFlow = "--";                      //!< 总现金流
    QString shareHoldersCount = "--";                  //!< 股东人数
    QString avgShareHolded = "--";                     //!< 人均持股
    QString avgMarketValue = "--";                     //!< 人均市值
    QString profitOnYoyBasis = "--";                   //!< 利润同比%%
    QString incomeOnYoyBasis = "--";                   //!< 收入同比%%
    QString pbRatio = "--";                            //!< 市净率
    QString pcfRatio = "--";                           //!< 市现率
    QString priceSalesRatio = "--";                    //!< 市销率
    QString earningsPerShare = "--";                   //!< 每股收益
    QString netAssetPerShare = "--";                   //!< 每股净资
    QString adjustedNetAssetPerShare = "--";           //!< 调整后净资
    QString capitalSurplusPerShare = "--";             //!< 每股公积
    QString undistributedProfitPreShare = "--";        //!< 每股未分配
    QString debtEquityRatio = "--";                    //!< 权益比
    QString roe = "--";                                //!< 净益率%%
    QString grossProfitRate = "--";                    //!< 毛利率%%
    QString operatingProfitRatio = "--";               //!< 营业利润率%%
    QString netProfitRate = "--";                      //!< 净利润率%%
    QString tradingCode = "--";                        //!< 交易代码
    QString selfSelectDate = "--";                     //!< 自选日
    QString selfSelectPrice = "--";                    //!< 自选价
    QString selfSelectEarnings = "--";                 //!< 自选收益%%
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
