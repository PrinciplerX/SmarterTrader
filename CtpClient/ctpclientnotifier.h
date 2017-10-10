#ifndef CTPCLIENTNOTIFIER_H
#define CTPCLIENTNOTIFIER_H

#include <QObject>
#include "ctpclient_global.h"

class CtpClientNotifier : public QObject
{
    Q_OBJECT
public:
    explicit CtpClientNotifier(QObject *parent = 0);

    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    void NotifyOnFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    ///@param nReason 错误原因
    ///        0x1001 网络读失败
    ///        0x1002 网络写失败
    ///        0x2001 接收心跳超时
    ///        0x2002 发送心跳失败
    ///        0x2003 收到错误报文
    void NotifyOnFrontDisconnected(int nReason);

    ///心跳超时警告。当长时间未收到报文时，该方法被调用。
    ///@param nTimeLapse 距离上次接收报文的时间
    // void NotifyOnHeartBeatWarning(int nTimeLapse);

    ///客户端认证响应
    // void NotifyOnRspAuthenticate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///登录请求响应
    void NotifyOnRspUserLogin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///登出请求响应
    // void NotifyOnRspUserLogout(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) {}

    ///用户口令更新请求响应
    // void NotifyOnRspUserPasswordUpdate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///资金账户口令更新请求响应
    // void NotifyOnRspTradingAccountPasswordUpdate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报单录入请求响应
    void NotifyOnRspOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///预埋单录入请求响应
    // void NotifyOnRspParkedOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///预埋撤单录入请求响应
    // void NotifyOnRspParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报单操作请求响应
    void NotifyOnRspOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///查询最大报单数量响应
    // void NotifyOnRspQueryMaxOrderVolume(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///投资者结算结果确认响应
    void NotifyOnRspSettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///删除预埋单响应
    // void NotifyOnRspRemoveParkedOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///删除预埋撤单响应
    // void NotifyOnRspRemoveParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///执行宣告录入请求响应
    // void NotifyOnRspExecOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///执行宣告操作请求响应
    // void NotifyOnRspExecOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///询价录入请求响应
    // void NotifyOnRspForQuoteInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报价录入请求响应
    // void NotifyOnRspQuoteInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报价操作请求响应
    // void NotifyOnRspQuoteAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///锁定应答
    // void NotifyOnRspLockInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///批量报单操作请求响应
    // void NotifyOnRspBatchOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///申请组合录入请求响应
    // void NotifyOnRspCombActionInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询报单响应
    void NotifyOnRspQryOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询成交响应
    void NotifyOnRspQryTrade(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者持仓响应
    void NotifyOnRspQryInvestorPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询资金账户响应
    void NotifyOnRspQryTradingAccount(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者响应
    // void NotifyOnRspQryInvestor(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易编码响应
    // void NotifyOnRspQryTradingCode(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询合约保证金率响应
    // void NotifyOnRspQryInstrumentMarginRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询合约手续费率响应
    // void NotifyOnRspQryInstrumentCommissionRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易所响应
    // void NotifyOnRspQryExchange(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询产品响应
    // void NotifyOnRspQryProduct(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询合约响应
    // void NotifyOnRspQryInstrument(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询行情响应
    // void NotifyOnRspQryDepthMarketData(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者结算结果响应
    void NotifyOnRspQrySettlementInfo(int nErrorId, const QString& errorMsg, const QString& content, int nRequestID, bool bIsLast) ;

    ///请求查询转帐银行响应
    // void NotifyOnRspQryTransferBank(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者持仓明细响应
    // void NotifyOnRspQryInvestorPositionDetail(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询客户通知响应
    // void NotifyOnRspQryNotice(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询结算信息确认响应
    void NotifyOnRspQrySettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者持仓明细响应
    // void NotifyOnRspQryInvestorPositionCombineDetail(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///查询保证金监管系统经纪公司资金账户密钥响应
    // void NotifyOnRspQryCFMMCTradingAccountKey(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询仓单折抵信息响应
    // void NotifyOnRspQryEWarrantOffset(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者品种/跨品种保证金响应
    // void NotifyOnRspQryInvestorProductGroupMargin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易所保证金率响应
    // void NotifyOnRspQryExchangeMarginRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易所调整保证金率响应
    // void NotifyOnRspQryExchangeMarginRateAdjust(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询汇率响应
    // void NotifyOnRspQryExchangeRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询二级代理操作员银期权限响应
    // void NotifyOnRspQrySecAgentACIDMap(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询产品报价汇率
    // void NotifyOnRspQryProductExchRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询产品组
    // void NotifyOnRspQryProductGroup(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询做市商合约手续费率响应
    // void NotifyOnRspQryMMInstrumentCommissionRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询做市商期权合约手续费响应
    // void NotifyOnRspQryMMOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询报单手续费响应
    // void NotifyOnRspQryInstrumentOrderCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询期权交易成本响应
    // void NotifyOnRspQryOptionInstrTradeCost(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询期权合约手续费响应
    // void NotifyOnRspQryOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询执行宣告响应
    // void NotifyOnRspQryExecOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询询价响应
    // void NotifyOnRspQryForQuote(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询报价响应
    // void NotifyOnRspQryQuote(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询锁定应答
    // void NotifyOnRspQryLock(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询锁定证券仓位应答
    // void NotifyOnRspQryLockPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询ETF期权合约手续费响应
    // void NotifyOnRspQryETFOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者分级
    // void NotifyOnRspQryInvestorLevel(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询E+1日行权冻结响应
    // void NotifyOnRspQryExecFreeze(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询组合合约安全系数响应
    // void NotifyOnRspQryCombInstrumentGuard(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询申请组合响应
    // void NotifyOnRspQryCombAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询转帐流水响应
    // void NotifyOnRspQryTransferSerial(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询银期签约关系响应
    // void NotifyOnRspQryAccountregister(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///错误应答
    void NotifyOnRspError(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报单通知
    void NotifyOnRtnOrder(CThostFtdcOrderField *pOrder) ;

    ///成交通知
    // void NotifyOnRtnTrade(CThostFtdcTradeField *pTrade) ;

    ///报单录入错误回报
    void NotifyOnErrRtnOrderInsert(int nErrorId, const QString& errorMsg) ;

    ///报单操作错误回报
    // void NotifyOnErrRtnOrderAction(int nErrorId, const QString& errorMsg) ;

    ///合约交易状态通知
    // void NotifyOnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;

    ///交易所公告通知
    // void NotifyOnRtnBulletin(CThostFtdcBulletinField *pBulletin) ;

    ///交易通知
    // void NotifyOnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;

    ///提示条件单校验错误
    // void NotifyOnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) ;

    ///执行宣告通知
    // void NotifyOnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) ;

    ///执行宣告录入错误回报
    // void NotifyOnErrRtnExecOrderInsert(int nErrorId, const QString& errorMsg) ;

    ///执行宣告操作错误回报
    // void NotifyOnErrRtnExecOrderAction(int nErrorId, const QString& errorMsg) ;

    ///询价录入错误回报
    // void NotifyOnErrRtnForQuoteInsert(int nErrorId, const QString& errorMsg) ;

    ///报价通知
    // void NotifyOnRtnQuote(CThostFtdcQuoteField *pQuote) ;

    ///报价录入错误回报
    // void NotifyOnErrRtnQuoteInsert(int nErrorId, const QString& errorMsg) ;

    ///报价操作错误回报
    // void NotifyOnErrRtnQuoteAction(int nErrorId, const QString& errorMsg) ;

    ///询价通知
    // void NotifyOnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;

    ///保证金监控中心用户令牌
    // void NotifyOnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) ;

    ///锁定通知
    // void NotifyOnRtnLock(CThostFtdcLockField *pLock) ;

    ///锁定错误通知
    // void NotifyOnErrRtnLockInsert(int nErrorId, const QString& errorMsg) ;

    ///批量报单操作错误回报
    // void NotifyOnErrRtnBatchOrderAction(int nErrorId, const QString& errorMsg) ;

    ///申请组合通知
    // void NotifyOnRtnCombAction(CThostFtdcCombActionField *pCombAction) ;

    ///申请组合录入错误回报
    // void NotifyOnErrRtnCombActionInsert(int nErrorId, const QString& errorMsg) ;

    ///请求查询签约银行响应
    // void NotifyOnRspQryContractBank(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询预埋单响应
    // void NotifyOnRspQryParkedOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询预埋撤单响应
    // void NotifyOnRspQryParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易通知响应
    // void NotifyOnRspQryTradingNotice(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询经纪公司交易参数响应
    // void NotifyOnRspQryBrokerTradingParams(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询经纪公司交易算法响应
    // void NotifyOnRspQryBrokerTradingAlgos(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询监控中心用户令牌
    // void NotifyOnRspQueryCFMMCTradingAccountToken(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///银行发起银行资金转期货通知
    // void NotifyOnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    ///银行发起期货资金转银行通知
    // void NotifyOnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    ///银行发起冲正银行转期货通知
    // void NotifyOnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    ///银行发起冲正期货转银行通知
    // void NotifyOnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    ///期货发起银行资金转期货通知
    // void NotifyOnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    ///期货发起期货资金转银行通知
    // void NotifyOnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    ///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
    // void NotifyOnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    ///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
    // void NotifyOnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    ///期货发起查询银行余额通知
    // void NotifyOnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) ;

    ///期货发起银行资金转期货错误回报
    // void NotifyOnErrRtnBankToFutureByFuture(int nErrorId, const QString& errorMsg) ;

    ///期货发起期货资金转银行错误回报
    // void NotifyOnErrRtnFutureToBankByFuture(int nErrorId, const QString& errorMsg) ;

    ///系统运行时期货端手工发起冲正银行转期货错误回报
    // void NotifyOnErrRtnRepealBankToFutureByFutureManual(int nErrorId, const QString& errorMsg) ;

    ///系统运行时期货端手工发起冲正期货转银行错误回报
    // void NotifyOnErrRtnRepealFutureToBankByFutureManual(int nErrorId, const QString& errorMsg) ;

    ///期货发起查询银行余额错误回报
    // void NotifyOnErrRtnQueryBankBalanceByFuture(int nErrorId, const QString& errorMsg) ;

    ///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
    // void NotifyOnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    ///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
    // void NotifyOnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    ///期货发起银行资金转期货应答
    // void NotifyOnRspFromBankToFutureByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///期货发起期货资金转银行应答
    // void NotifyOnRspFromFutureToBankByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///期货发起查询银行余额应答
    // void NotifyOnRspQueryBankAccountMoneyByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///银行发起银期开户通知
    // void NotifyOnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) ;

    ///银行发起银期销户通知
    // void NotifyOnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) ;

    ///银行发起变更银行账号通知
    // void NotifyOnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) ;

signals:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    void OnFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    ///@param nReason 错误原因
    ///        0x1001 网络读失败
    ///        0x1002 网络写失败
    ///        0x2001 接收心跳超时
    ///        0x2002 发送心跳失败
    ///        0x2003 收到错误报文
    void OnFrontDisconnected(int nReason);

    ///心跳超时警告。当长时间未收到报文时，该方法被调用。
    ///@param nTimeLapse 距离上次接收报文的时间
    // void OnHeartBeatWarning(int nTimeLapse);

    ///客户端认证响应
    // void OnRspAuthenticate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///登录请求响应
    void OnRspUserLogin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///登出请求响应
    // void OnRspUserLogout(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) {}

    ///用户口令更新请求响应
    // void OnRspUserPasswordUpdate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///资金账户口令更新请求响应
    // void OnRspTradingAccountPasswordUpdate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报单录入请求响应
    void OnRspOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///预埋单录入请求响应
    // void OnRspParkedOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///预埋撤单录入请求响应
    // void OnRspParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报单操作请求响应
    void OnRspOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///查询最大报单数量响应
    // void OnRspQueryMaxOrderVolume(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///投资者结算结果确认响应
    void OnRspSettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///删除预埋单响应
    // void OnRspRemoveParkedOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///删除预埋撤单响应
    // void OnRspRemoveParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///执行宣告录入请求响应
    // void OnRspExecOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///执行宣告操作请求响应
    // void OnRspExecOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///询价录入请求响应
    // void OnRspForQuoteInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报价录入请求响应
    // void OnRspQuoteInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///报价操作请求响应
    // void OnRspQuoteAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///锁定应答
    // void OnRspLockInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///批量报单操作请求响应
    // void OnRspBatchOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///申请组合录入请求响应
    // void OnRspCombActionInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询报单响应
    void OnRspQryOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询成交响应
    void OnRspQryTrade(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者持仓响应
    void OnRspQryInvestorPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询资金账户响应
    void OnRspQryTradingAccount(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者响应
    // void OnRspQryInvestor(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易编码响应
    // void OnRspQryTradingCode(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询合约保证金率响应
    // void OnRspQryInstrumentMarginRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询合约手续费率响应
    // void OnRspQryInstrumentCommissionRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易所响应
    // void OnRspQryExchange(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询产品响应
    // void OnRspQryProduct(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询合约响应
    // void OnRspQryInstrument(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询行情响应
    // void OnRspQryDepthMarketData(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者结算结果响应
    void OnRspQrySettlementInfo(int nErrorId, const QString& errorMsg, const QString& content, int nRequestID, bool bIsLast) ;

    ///请求查询转帐银行响应
    // void OnRspQryTransferBank(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者持仓明细响应
    // void OnRspQryInvestorPositionDetail(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询客户通知响应
    // void OnRspQryNotice(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询结算信息确认响应
    void OnRspQrySettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者持仓明细响应
    // void OnRspQryInvestorPositionCombineDetail(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///查询保证金监管系统经纪公司资金账户密钥响应
    // void OnRspQryCFMMCTradingAccountKey(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询仓单折抵信息响应
    // void OnRspQryEWarrantOffset(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者品种/跨品种保证金响应
    // void OnRspQryInvestorProductGroupMargin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易所保证金率响应
    // void OnRspQryExchangeMarginRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易所调整保证金率响应
    // void OnRspQryExchangeMarginRateAdjust(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询汇率响应
    // void OnRspQryExchangeRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询二级代理操作员银期权限响应
    // void OnRspQrySecAgentACIDMap(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询产品报价汇率
    // void OnRspQryProductExchRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询产品组
    // void OnRspQryProductGroup(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询做市商合约手续费率响应
    // void OnRspQryMMInstrumentCommissionRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询做市商期权合约手续费响应
    // void OnRspQryMMOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询报单手续费响应
    // void OnRspQryInstrumentOrderCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询期权交易成本响应
    // void OnRspQryOptionInstrTradeCost(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询期权合约手续费响应
    // void OnRspQryOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询执行宣告响应
    // void OnRspQryExecOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询询价响应
    // void OnRspQryForQuote(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询报价响应
    // void OnRspQryQuote(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询锁定应答
    // void OnRspQryLock(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询锁定证券仓位应答
    // void OnRspQryLockPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询ETF期权合约手续费响应
    // void OnRspQryETFOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询投资者分级
    // void OnRspQryInvestorLevel(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询E+1日行权冻结响应
    // void OnRspQryExecFreeze(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询组合合约安全系数响应
    // void OnRspQryCombInstrumentGuard(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询申请组合响应
    // void OnRspQryCombAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询转帐流水响应
    // void OnRspQryTransferSerial(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询银期签约关系响应
    // void OnRspQryAccountregister(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///错误应答
    void OnRspError(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///报单通知
    void OnRtnOrder(CThostFtdcOrderField *pOrder) ;

    ///成交通知
    // void OnRtnTrade(CThostFtdcTradeField *pTrade) ;

    ///报单录入错误回报
    void OnErrRtnOrderInsert(int nErrorId, const QString& errorMsg) ;

    ///报单操作错误回报
    // void OnErrRtnOrderAction(int nErrorId, const QString& errorMsg) ;

    ///合约交易状态通知
    // void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;

    ///交易所公告通知
    // void OnRtnBulletin(CThostFtdcBulletinField *pBulletin) ;

    ///交易通知
    // void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;

    ///提示条件单校验错误
    // void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) ;

    ///执行宣告通知
    // void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) ;

    ///执行宣告录入错误回报
    // void OnErrRtnExecOrderInsert(int nErrorId, const QString& errorMsg) ;

    ///执行宣告操作错误回报
    // void OnErrRtnExecOrderAction(int nErrorId, const QString& errorMsg) ;

    ///询价录入错误回报
    // void OnErrRtnForQuoteInsert(int nErrorId, const QString& errorMsg) ;

    ///报价通知
    // void OnRtnQuote(CThostFtdcQuoteField *pQuote) ;

    ///报价录入错误回报
    // void OnErrRtnQuoteInsert(int nErrorId, const QString& errorMsg) ;

    ///报价操作错误回报
    // void OnErrRtnQuoteAction(int nErrorId, const QString& errorMsg) ;

    ///询价通知
    // void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;

    ///保证金监控中心用户令牌
    // void OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) ;

    ///锁定通知
    // void OnRtnLock(CThostFtdcLockField *pLock) ;

    ///锁定错误通知
    // void OnErrRtnLockInsert(int nErrorId, const QString& errorMsg) ;

    ///批量报单操作错误回报
    // void OnErrRtnBatchOrderAction(int nErrorId, const QString& errorMsg) ;

    ///申请组合通知
    // void OnRtnCombAction(CThostFtdcCombActionField *pCombAction) ;

    ///申请组合录入错误回报
    // void OnErrRtnCombActionInsert(int nErrorId, const QString& errorMsg) ;

    ///请求查询签约银行响应
    // void OnRspQryContractBank(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询预埋单响应
    // void OnRspQryParkedOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询预埋撤单响应
    // void OnRspQryParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询交易通知响应
    // void OnRspQryTradingNotice(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询经纪公司交易参数响应
    // void OnRspQryBrokerTradingParams(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询经纪公司交易算法响应
    // void OnRspQryBrokerTradingAlgos(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///请求查询监控中心用户令牌
    // void OnRspQueryCFMMCTradingAccountToken(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///银行发起银行资金转期货通知
    // void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    ///银行发起期货资金转银行通知
    // void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    ///银行发起冲正银行转期货通知
    // void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    ///银行发起冲正期货转银行通知
    // void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    ///期货发起银行资金转期货通知
    // void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    ///期货发起期货资金转银行通知
    // void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    ///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
    // void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    ///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
    // void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    ///期货发起查询银行余额通知
    // void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) ;

    ///期货发起银行资金转期货错误回报
    // void OnErrRtnBankToFutureByFuture(int nErrorId, const QString& errorMsg) ;

    ///期货发起期货资金转银行错误回报
    // void OnErrRtnFutureToBankByFuture(int nErrorId, const QString& errorMsg) ;

    ///系统运行时期货端手工发起冲正银行转期货错误回报
    // void OnErrRtnRepealBankToFutureByFutureManual(int nErrorId, const QString& errorMsg) ;

    ///系统运行时期货端手工发起冲正期货转银行错误回报
    // void OnErrRtnRepealFutureToBankByFutureManual(int nErrorId, const QString& errorMsg) ;

    ///期货发起查询银行余额错误回报
    // void OnErrRtnQueryBankBalanceByFuture(int nErrorId, const QString& errorMsg) ;

    ///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
    // void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    ///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
    // void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    ///期货发起银行资金转期货应答
    // void OnRspFromBankToFutureByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///期货发起期货资金转银行应答
    // void OnRspFromFutureToBankByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///期货发起查询银行余额应答
    // void OnRspQueryBankAccountMoneyByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///银行发起银期开户通知
    // void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) ;

    ///银行发起银期销户通知
    // void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) ;

    ///银行发起变更银行账号通知
    // void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) ;

public slots:

};

#endif // CTPCLIENTNOTIFIER_H
