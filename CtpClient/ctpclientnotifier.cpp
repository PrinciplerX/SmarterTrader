#include "ctpclientnotifier.h"

CtpClientNotifier::CtpClientNotifier(QObject *parent) : QObject(parent)
{

}

void CtpClientNotifier::NotifyOnFrontConnected()
{
    emit OnFrontConnected();
}

void CtpClientNotifier::NotifyOnFrontDisconnected(int nReason)
{
    emit OnFrontDisconnected(nReason);
}

void CtpClientNotifier::NotifyOnRspUserLogin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspUserLogin(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspOrderInsert(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspOrderAction(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspSettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspSettlementInfoConfirm(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspQryOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspQryOrder(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspQryTrade(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspQryTrade(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspQryInvestorPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspQryInvestorPosition(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspQryTradingAccount(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspQryTradingAccount(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspQrySettlementInfo(int nErrorId, const QString &errorMsg, const QString &content, int nRequestID, bool bIsLast)
{
    emit OnRspQrySettlementInfo(nErrorId, errorMsg, content, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspQrySettlementInfoConfirm(int nErrorId, const QString &errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspQrySettlementInfoConfirm(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRspError(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast)
{
    emit OnRspError(nErrorId, errorMsg, nRequestID, bIsLast);
}

void CtpClientNotifier::NotifyOnRtnOrder(CThostFtdcOrderField *pOrder)
{
    emit OnRtnOrder(pOrder);
}

void CtpClientNotifier::NotifyOnErrRtnOrderInsert(int nErrorId, const QString& errorMsg)
{
    emit OnErrRtnOrderInsert(nErrorId, errorMsg);
}
