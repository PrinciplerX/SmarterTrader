#include "ctpclient.h"
#include "stringhelper.h"
#include <QDebug>
#include "ctpaccountmanager.h"

CtpClient::CtpClient()
{
    mNotifier = new CtpClientNotifier();
}

CtpClient &CtpClient::instance()
{
    static CtpClient mInstance;
    return mInstance;
}

bool CtpClient::isConnected() const
{
    return mConnected;
}

CtpClientNotifier &CtpClient::notifier()
{
    return *mNotifier;
}

int CtpClient::nextRequestId()
{
    mRequestId++;
    return mRequestId;
}

void CtpClient::OnFrontConnected()
{
    mConnected = true;
    mNotifier->NotifyOnFrontConnected();
    qDebug() << "[CtpClient] [OnFrontConnected] front connected.";
}

void CtpClient::OnFrontDisconnected(int nReason)
{
    mConnected = false;
    mNotifier->OnFrontDisconnected(nReason);
    qDebug() << "[CtpClient] [OnFrontDisconnected] nReason = " << nReason;
}

void CtpClient::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug() << "[CtpClient] [OnRspUserLogin] nRequestID =" << nRequestID;
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));

    CtpAccountData account;
    strcpy(account.TradingDay,  pRspUserLogin->TradingDay);
    strcpy(account.LoginTime,   pRspUserLogin->LoginTime);
    strcpy(account.BrokerID,    pRspUserLogin->BrokerID);
    strcpy(account.UserID,      pRspUserLogin->UserID);
    strcpy(account.SystemName,  pRspUserLogin->SystemName);
    account.FrontID =           pRspUserLogin->FrontID;
    account.SessionID =         pRspUserLogin->SessionID;
    strcpy(account.MaxOrderRef, pRspUserLogin->MaxOrderRef);
    strcpy(account.BrokerID,    pRspUserLogin->BrokerID);

    CtpAccountManager::instance().addAccount(account);

    mNotifier->NotifyOnRspUserLogin(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

void CtpClient::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug() << "[CtpClient] [OnRspSettlementInfoConfirm] nRequestID =" << nRequestID;
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    if (pRspInfo->ErrorID == 0)
        CtpAccountManager::instance().addSettlementInfoConfirm(*pSettlementInfoConfirm);

    mNotifier->NotifyOnRspSettlementInfoConfirm(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

void CtpClient::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    mNotifier->NotifyOnRspOrderInsert(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

void CtpClient::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    mNotifier->NotifyOnRtnOrder(pOrder);
}

void CtpClient::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    mNotifier->NotifyOnErrRtnOrderInsert(pRspInfo->ErrorID, errorMsg);
}

void CtpClient::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    mNotifier->NotifyOnRspQryOrder(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

void CtpClient::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    mNotifier->NotifyOnRspQryTrade(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

void CtpClient::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    mNotifier->NotifyOnRspQryInvestorPosition(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

void CtpClient::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    mNotifier->NotifyOnRspQryTradingAccount(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

void CtpClient::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug() << "[CtpClient] [OnRspQrySettlementInfo] nRequestID =" << nRequestID;
    QString content;
    QString errorMsg;
    int errorID;

    if (pRspInfo) {
        errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
        errorID = pRspInfo->ErrorID;
    } else {
        errorMsg = "Null CThostFtdcRspInfoField pointer received.";
        qDebug() << errorMsg;
        errorID = 0;
    }

    if (pSettlementInfo) {
        CtpAccountManager::instance().addSettlementInfo(*pSettlementInfo);
        content = QString::fromStdString(StringHelper::convertCodec(pSettlementInfo->Content));
    } else {
        content = "[NULL]";
        qDebug() << "Null CThostFtdcSettlementInfoField pointer received.";
    }

    mNotifier->NotifyOnRspQrySettlementInfo(errorID, errorMsg, content, nRequestID, bIsLast);
}

void CtpClient::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug() << "[CtpClient] [OnRspQrySettlementInfoConfirm]";
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    if (pRspInfo->ErrorID == 0)
        CtpAccountManager::instance().addSettlementInfoConfirm(*pSettlementInfoConfirm);
    mNotifier->NotifyOnRspQrySettlementInfoConfirm(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

void CtpClient::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    mNotifier->NotifyOnRspError(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
    qDebug() << "[CtpClient] [OnRspError] nRequestId =" << nRequestID;
}

void CtpClient::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    QString errorMsg = QString::fromStdString(StringHelper::convertCodec(pRspInfo->ErrorMsg));
    mNotifier->NotifyOnRspOrderAction(pRspInfo->ErrorID, errorMsg, nRequestID, bIsLast);
}

CThostFtdcTraderApi *CtpClient::getTraderApi(const std::string &bacid)
{
    CThostFtdcTraderApi* pUserApi = nullptr;

    auto it = userTraderApi.find(bacid);
    if(it != userTraderApi.end()){
        pUserApi = it->second;
    } else {
        pUserApi = this->CreateTraderApi(bacid);
        if(pUserApi){
            userTraderApi[bacid] = pUserApi;
        }
    }

    return pUserApi;
}

CThostFtdcTraderApi *CtpClient::CreateTraderApi(const std::string &bacid)
{
    std::lock_guard<std::mutex> dataLocker(traderApiMutex);

    std::string ip = "218.202.237.33";
    std::string port = "10002";
    std::string frontAddress = "tcp://" + ip + ":" + port;

    CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    if(pUserApi) {
        pUserApi->RegisterSpi((CThostFtdcTraderSpi*)this);
        pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
        pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
        pUserApi->RegisterFront((char *)frontAddress.c_str());
        pUserApi->Init();
    } else {
        pUserApi = nullptr;
    }

    return pUserApi;
}

bool CtpClient::checkRequestResult(int result, std::string& errMsg)
{
    switch (result) {
    case 0:  errMsg = "发送成功";                    return true;
    case -1: errMsg = "因网络原因发送失败";          return false;
    case -2: errMsg = "未处理请求队列总数量超限。";  return false;
    case -3: errMsg = "每秒发送请求数量超限。";      return false;
    default: errMsg = "未知错误";                    return false;
    }
}
