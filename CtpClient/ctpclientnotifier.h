#ifndef CTPCLIENTNOTIFIER_H
#define CTPCLIENTNOTIFIER_H

#include <QObject>
#include "ctpclient_global.h"

class CtpClientNotifier : public QObject
{
    Q_OBJECT
public:
    explicit CtpClientNotifier(QObject *parent = 0);

    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    void NotifyOnFrontConnected();

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    ///@param nReason ����ԭ��
    ///        0x1001 �����ʧ��
    ///        0x1002 ����дʧ��
    ///        0x2001 ����������ʱ
    ///        0x2002 ��������ʧ��
    ///        0x2003 �յ�������
    void NotifyOnFrontDisconnected(int nReason);

    ///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
    ///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
    // void NotifyOnHeartBeatWarning(int nTimeLapse);

    ///�ͻ�����֤��Ӧ
    // void NotifyOnRspAuthenticate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///��¼������Ӧ
    void NotifyOnRspUserLogin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///�ǳ�������Ӧ
    // void NotifyOnRspUserLogout(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) {}

    ///�û��������������Ӧ
    // void NotifyOnRspUserPasswordUpdate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�ʽ��˻��������������Ӧ
    // void NotifyOnRspTradingAccountPasswordUpdate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����¼��������Ӧ
    void NotifyOnRspOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///Ԥ��¼��������Ӧ
    // void NotifyOnRspParkedOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///Ԥ�񳷵�¼��������Ӧ
    // void NotifyOnRspParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///��������������Ӧ
    void NotifyOnRspOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///��ѯ��󱨵�������Ӧ
    // void NotifyOnRspQueryMaxOrderVolume(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///Ͷ���߽�����ȷ����Ӧ
    void NotifyOnRspSettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ɾ��Ԥ����Ӧ
    // void NotifyOnRspRemoveParkedOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ɾ��Ԥ�񳷵���Ӧ
    // void NotifyOnRspRemoveParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ִ������¼��������Ӧ
    // void NotifyOnRspExecOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ִ���������������Ӧ
    // void NotifyOnRspExecOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ѯ��¼��������Ӧ
    // void NotifyOnRspForQuoteInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����¼��������Ӧ
    // void NotifyOnRspQuoteInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///���۲���������Ӧ
    // void NotifyOnRspQuoteAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����Ӧ��
    // void NotifyOnRspLockInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///������������������Ӧ
    // void NotifyOnRspBatchOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�������¼��������Ӧ
    // void NotifyOnRspCombActionInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ӧ
    void NotifyOnRspQryOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�ɽ���Ӧ
    void NotifyOnRspQryTrade(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���ֲ߳���Ӧ
    void NotifyOnRspQryInvestorPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�ʽ��˻���Ӧ
    void NotifyOnRspQryTradingAccount(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ������Ӧ
    // void NotifyOnRspQryInvestor(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ���ױ�����Ӧ
    // void NotifyOnRspQryTradingCode(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Լ��֤������Ӧ
    // void NotifyOnRspQryInstrumentMarginRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Լ����������Ӧ
    // void NotifyOnRspQryInstrumentCommissionRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��������Ӧ
    // void NotifyOnRspQryExchange(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ʒ��Ӧ
    // void NotifyOnRspQryProduct(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Լ��Ӧ
    // void NotifyOnRspQryInstrument(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ӧ
    // void NotifyOnRspQryDepthMarketData(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���߽�������Ӧ
    void NotifyOnRspQrySettlementInfo(int nErrorId, const QString& errorMsg, const QString& content, int nRequestID, bool bIsLast) ;

    ///�����ѯת��������Ӧ
    // void NotifyOnRspQryTransferBank(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���ֲ߳���ϸ��Ӧ
    // void NotifyOnRspQryInvestorPositionDetail(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�ͻ�֪ͨ��Ӧ
    // void NotifyOnRspQryNotice(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ϣȷ����Ӧ
    void NotifyOnRspQrySettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���ֲ߳���ϸ��Ӧ
    // void NotifyOnRspQryInvestorPositionCombineDetail(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
    // void NotifyOnRspQryCFMMCTradingAccountKey(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�ֵ��۵���Ϣ��Ӧ
    // void NotifyOnRspQryEWarrantOffset(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ����Ʒ��/��Ʒ�ֱ�֤����Ӧ
    // void NotifyOnRspQryInvestorProductGroupMargin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��������֤������Ӧ
    // void NotifyOnRspQryExchangeMarginRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������������֤������Ӧ
    // void NotifyOnRspQryExchangeMarginRateAdjust(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ӧ
    // void NotifyOnRspQryExchangeRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�����������Ա����Ȩ����Ӧ
    // void NotifyOnRspQrySecAgentACIDMap(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ʒ���ۻ���
    // void NotifyOnRspQryProductExchRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ʒ��
    // void NotifyOnRspQryProductGroup(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�����̺�Լ����������Ӧ
    // void NotifyOnRspQryMMInstrumentCommissionRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��������Ȩ��Լ��������Ӧ
    // void NotifyOnRspQryMMOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������������Ӧ
    // void NotifyOnRspQryInstrumentOrderCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ȩ���׳ɱ���Ӧ
    // void NotifyOnRspQryOptionInstrTradeCost(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ȩ��Լ��������Ӧ
    // void NotifyOnRspQryOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯִ��������Ӧ
    // void NotifyOnRspQryExecOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯѯ����Ӧ
    // void NotifyOnRspQryForQuote(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ӧ
    // void NotifyOnRspQryQuote(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ����Ӧ��
    // void NotifyOnRspQryLock(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ����֤ȯ��λӦ��
    // void NotifyOnRspQryLockPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯETF��Ȩ��Լ��������Ӧ
    // void NotifyOnRspQryETFOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���߷ּ�
    // void NotifyOnRspQryInvestorLevel(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯE+1����Ȩ������Ӧ
    // void NotifyOnRspQryExecFreeze(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ϻ�Լ��ȫϵ����Ӧ
    // void NotifyOnRspQryCombInstrumentGuard(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ���������Ӧ
    // void NotifyOnRspQryCombAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯת����ˮ��Ӧ
    // void NotifyOnRspQryTransferSerial(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ����ǩԼ��ϵ��Ӧ
    // void NotifyOnRspQryAccountregister(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����Ӧ��
    void NotifyOnRspError(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����֪ͨ
    void NotifyOnRtnOrder(CThostFtdcOrderField *pOrder) ;

    ///�ɽ�֪ͨ
    // void NotifyOnRtnTrade(CThostFtdcTradeField *pTrade) ;

    ///����¼�����ر�
    void NotifyOnErrRtnOrderInsert(int nErrorId, const QString& errorMsg) ;

    ///������������ر�
    // void NotifyOnErrRtnOrderAction(int nErrorId, const QString& errorMsg) ;

    ///��Լ����״̬֪ͨ
    // void NotifyOnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;

    ///����������֪ͨ
    // void NotifyOnRtnBulletin(CThostFtdcBulletinField *pBulletin) ;

    ///����֪ͨ
    // void NotifyOnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;

    ///��ʾ������У�����
    // void NotifyOnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) ;

    ///ִ������֪ͨ
    // void NotifyOnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) ;

    ///ִ������¼�����ر�
    // void NotifyOnErrRtnExecOrderInsert(int nErrorId, const QString& errorMsg) ;

    ///ִ�������������ر�
    // void NotifyOnErrRtnExecOrderAction(int nErrorId, const QString& errorMsg) ;

    ///ѯ��¼�����ر�
    // void NotifyOnErrRtnForQuoteInsert(int nErrorId, const QString& errorMsg) ;

    ///����֪ͨ
    // void NotifyOnRtnQuote(CThostFtdcQuoteField *pQuote) ;

    ///����¼�����ر�
    // void NotifyOnErrRtnQuoteInsert(int nErrorId, const QString& errorMsg) ;

    ///���۲�������ر�
    // void NotifyOnErrRtnQuoteAction(int nErrorId, const QString& errorMsg) ;

    ///ѯ��֪ͨ
    // void NotifyOnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;

    ///��֤���������û�����
    // void NotifyOnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) ;

    ///����֪ͨ
    // void NotifyOnRtnLock(CThostFtdcLockField *pLock) ;

    ///��������֪ͨ
    // void NotifyOnErrRtnLockInsert(int nErrorId, const QString& errorMsg) ;

    ///����������������ر�
    // void NotifyOnErrRtnBatchOrderAction(int nErrorId, const QString& errorMsg) ;

    ///�������֪ͨ
    // void NotifyOnRtnCombAction(CThostFtdcCombActionField *pCombAction) ;

    ///�������¼�����ر�
    // void NotifyOnErrRtnCombActionInsert(int nErrorId, const QString& errorMsg) ;

    ///�����ѯǩԼ������Ӧ
    // void NotifyOnRspQryContractBank(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯԤ����Ӧ
    // void NotifyOnRspQryParkedOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯԤ�񳷵���Ӧ
    // void NotifyOnRspQryParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ����֪ͨ��Ӧ
    // void NotifyOnRspQryTradingNotice(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ���͹�˾���ײ�����Ӧ
    // void NotifyOnRspQryBrokerTradingParams(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ���͹�˾�����㷨��Ӧ
    // void NotifyOnRspQryBrokerTradingAlgos(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��������û�����
    // void NotifyOnRspQueryCFMMCTradingAccountToken(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///���з��������ʽ�ת�ڻ�֪ͨ
    // void NotifyOnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    ///���з����ڻ��ʽ�ת����֪ͨ
    // void NotifyOnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    ///���з����������ת�ڻ�֪ͨ
    // void NotifyOnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    ///���з�������ڻ�ת����֪ͨ
    // void NotifyOnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    ///�ڻ����������ʽ�ת�ڻ�֪ͨ
    // void NotifyOnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    ///�ڻ������ڻ��ʽ�ת����֪ͨ
    // void NotifyOnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    ///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
    // void NotifyOnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    ///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
    // void NotifyOnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    ///�ڻ������ѯ�������֪ͨ
    // void NotifyOnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) ;

    ///�ڻ����������ʽ�ת�ڻ�����ر�
    // void NotifyOnErrRtnBankToFutureByFuture(int nErrorId, const QString& errorMsg) ;

    ///�ڻ������ڻ��ʽ�ת���д���ر�
    // void NotifyOnErrRtnFutureToBankByFuture(int nErrorId, const QString& errorMsg) ;

    ///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ�����ر�
    // void NotifyOnErrRtnRepealBankToFutureByFutureManual(int nErrorId, const QString& errorMsg) ;

    ///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת���д���ر�
    // void NotifyOnErrRtnRepealFutureToBankByFutureManual(int nErrorId, const QString& errorMsg) ;

    ///�ڻ������ѯ����������ر�
    // void NotifyOnErrRtnQueryBankBalanceByFuture(int nErrorId, const QString& errorMsg) ;

    ///�ڻ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
    // void NotifyOnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    ///�ڻ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
    // void NotifyOnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    ///�ڻ����������ʽ�ת�ڻ�Ӧ��
    // void NotifyOnRspFromBankToFutureByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�ڻ������ڻ��ʽ�ת����Ӧ��
    // void NotifyOnRspFromFutureToBankByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�ڻ������ѯ�������Ӧ��
    // void NotifyOnRspQueryBankAccountMoneyByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///���з������ڿ���֪ͨ
    // void NotifyOnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) ;

    ///���з�����������֪ͨ
    // void NotifyOnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) ;

    ///���з����������˺�֪ͨ
    // void NotifyOnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) ;

signals:
    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    void OnFrontConnected();

    ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
    ///@param nReason ����ԭ��
    ///        0x1001 �����ʧ��
    ///        0x1002 ����дʧ��
    ///        0x2001 ����������ʱ
    ///        0x2002 ��������ʧ��
    ///        0x2003 �յ�������
    void OnFrontDisconnected(int nReason);

    ///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
    ///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
    // void OnHeartBeatWarning(int nTimeLapse);

    ///�ͻ�����֤��Ӧ
    // void OnRspAuthenticate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///��¼������Ӧ
    void OnRspUserLogin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///�ǳ�������Ӧ
    // void OnRspUserLogout(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) {}

    ///�û��������������Ӧ
    // void OnRspUserPasswordUpdate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�ʽ��˻��������������Ӧ
    // void OnRspTradingAccountPasswordUpdate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����¼��������Ӧ
    void OnRspOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///Ԥ��¼��������Ӧ
    // void OnRspParkedOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///Ԥ�񳷵�¼��������Ӧ
    // void OnRspParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///��������������Ӧ
    void OnRspOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///��ѯ��󱨵�������Ӧ
    // void OnRspQueryMaxOrderVolume(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///Ͷ���߽�����ȷ����Ӧ
    void OnRspSettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ɾ��Ԥ����Ӧ
    // void OnRspRemoveParkedOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ɾ��Ԥ�񳷵���Ӧ
    // void OnRspRemoveParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ִ������¼��������Ӧ
    // void OnRspExecOrderInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ִ���������������Ӧ
    // void OnRspExecOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///ѯ��¼��������Ӧ
    // void OnRspForQuoteInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����¼��������Ӧ
    // void OnRspQuoteInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///���۲���������Ӧ
    // void OnRspQuoteAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����Ӧ��
    // void OnRspLockInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///������������������Ӧ
    // void OnRspBatchOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�������¼��������Ӧ
    // void OnRspCombActionInsert(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ӧ
    void OnRspQryOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�ɽ���Ӧ
    void OnRspQryTrade(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���ֲ߳���Ӧ
    void OnRspQryInvestorPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�ʽ��˻���Ӧ
    void OnRspQryTradingAccount(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ������Ӧ
    // void OnRspQryInvestor(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ���ױ�����Ӧ
    // void OnRspQryTradingCode(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Լ��֤������Ӧ
    // void OnRspQryInstrumentMarginRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Լ����������Ӧ
    // void OnRspQryInstrumentCommissionRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��������Ӧ
    // void OnRspQryExchange(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ʒ��Ӧ
    // void OnRspQryProduct(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Լ��Ӧ
    // void OnRspQryInstrument(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ӧ
    // void OnRspQryDepthMarketData(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���߽�������Ӧ
    void OnRspQrySettlementInfo(int nErrorId, const QString& errorMsg, const QString& content, int nRequestID, bool bIsLast) ;

    ///�����ѯת��������Ӧ
    // void OnRspQryTransferBank(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���ֲ߳���ϸ��Ӧ
    // void OnRspQryInvestorPositionDetail(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�ͻ�֪ͨ��Ӧ
    // void OnRspQryNotice(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ϣȷ����Ӧ
    void OnRspQrySettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���ֲ߳���ϸ��Ӧ
    // void OnRspQryInvestorPositionCombineDetail(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
    // void OnRspQryCFMMCTradingAccountKey(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�ֵ��۵���Ϣ��Ӧ
    // void OnRspQryEWarrantOffset(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ����Ʒ��/��Ʒ�ֱ�֤����Ӧ
    // void OnRspQryInvestorProductGroupMargin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��������֤������Ӧ
    // void OnRspQryExchangeMarginRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������������֤������Ӧ
    // void OnRspQryExchangeMarginRateAdjust(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ӧ
    // void OnRspQryExchangeRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�����������Ա����Ȩ����Ӧ
    // void OnRspQrySecAgentACIDMap(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ʒ���ۻ���
    // void OnRspQryProductExchRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ʒ��
    // void OnRspQryProductGroup(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ�����̺�Լ����������Ӧ
    // void OnRspQryMMInstrumentCommissionRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��������Ȩ��Լ��������Ӧ
    // void OnRspQryMMOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������������Ӧ
    // void OnRspQryInstrumentOrderCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ȩ���׳ɱ���Ӧ
    // void OnRspQryOptionInstrTradeCost(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ȩ��Լ��������Ӧ
    // void OnRspQryOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯִ��������Ӧ
    // void OnRspQryExecOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯѯ����Ӧ
    // void OnRspQryForQuote(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ������Ӧ
    // void OnRspQryQuote(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ����Ӧ��
    // void OnRspQryLock(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ����֤ȯ��λӦ��
    // void OnRspQryLockPosition(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯETF��Ȩ��Լ��������Ӧ
    // void OnRspQryETFOptionInstrCommRate(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯͶ���߷ּ�
    // void OnRspQryInvestorLevel(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯE+1����Ȩ������Ӧ
    // void OnRspQryExecFreeze(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��Ϻ�Լ��ȫϵ����Ӧ
    // void OnRspQryCombInstrumentGuard(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ���������Ӧ
    // void OnRspQryCombAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯת����ˮ��Ӧ
    // void OnRspQryTransferSerial(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ����ǩԼ��ϵ��Ӧ
    // void OnRspQryAccountregister(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///����Ӧ��
    void OnRspError(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

    ///����֪ͨ
    void OnRtnOrder(CThostFtdcOrderField *pOrder) ;

    ///�ɽ�֪ͨ
    // void OnRtnTrade(CThostFtdcTradeField *pTrade) ;

    ///����¼�����ر�
    void OnErrRtnOrderInsert(int nErrorId, const QString& errorMsg) ;

    ///������������ر�
    // void OnErrRtnOrderAction(int nErrorId, const QString& errorMsg) ;

    ///��Լ����״̬֪ͨ
    // void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;

    ///����������֪ͨ
    // void OnRtnBulletin(CThostFtdcBulletinField *pBulletin) ;

    ///����֪ͨ
    // void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;

    ///��ʾ������У�����
    // void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) ;

    ///ִ������֪ͨ
    // void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) ;

    ///ִ������¼�����ر�
    // void OnErrRtnExecOrderInsert(int nErrorId, const QString& errorMsg) ;

    ///ִ�������������ر�
    // void OnErrRtnExecOrderAction(int nErrorId, const QString& errorMsg) ;

    ///ѯ��¼�����ر�
    // void OnErrRtnForQuoteInsert(int nErrorId, const QString& errorMsg) ;

    ///����֪ͨ
    // void OnRtnQuote(CThostFtdcQuoteField *pQuote) ;

    ///����¼�����ر�
    // void OnErrRtnQuoteInsert(int nErrorId, const QString& errorMsg) ;

    ///���۲�������ر�
    // void OnErrRtnQuoteAction(int nErrorId, const QString& errorMsg) ;

    ///ѯ��֪ͨ
    // void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;

    ///��֤���������û�����
    // void OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) ;

    ///����֪ͨ
    // void OnRtnLock(CThostFtdcLockField *pLock) ;

    ///��������֪ͨ
    // void OnErrRtnLockInsert(int nErrorId, const QString& errorMsg) ;

    ///����������������ر�
    // void OnErrRtnBatchOrderAction(int nErrorId, const QString& errorMsg) ;

    ///�������֪ͨ
    // void OnRtnCombAction(CThostFtdcCombActionField *pCombAction) ;

    ///�������¼�����ر�
    // void OnErrRtnCombActionInsert(int nErrorId, const QString& errorMsg) ;

    ///�����ѯǩԼ������Ӧ
    // void OnRspQryContractBank(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯԤ����Ӧ
    // void OnRspQryParkedOrder(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯԤ�񳷵���Ӧ
    // void OnRspQryParkedOrderAction(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ����֪ͨ��Ӧ
    // void OnRspQryTradingNotice(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ���͹�˾���ײ�����Ӧ
    // void OnRspQryBrokerTradingParams(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ���͹�˾�����㷨��Ӧ
    // void OnRspQryBrokerTradingAlgos(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�����ѯ��������û�����
    // void OnRspQueryCFMMCTradingAccountToken(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///���з��������ʽ�ת�ڻ�֪ͨ
    // void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    ///���з����ڻ��ʽ�ת����֪ͨ
    // void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    ///���з����������ת�ڻ�֪ͨ
    // void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    ///���з�������ڻ�ת����֪ͨ
    // void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    ///�ڻ����������ʽ�ת�ڻ�֪ͨ
    // void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    ///�ڻ������ڻ��ʽ�ת����֪ͨ
    // void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    ///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
    // void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    ///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
    // void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    ///�ڻ������ѯ�������֪ͨ
    // void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) ;

    ///�ڻ����������ʽ�ת�ڻ�����ر�
    // void OnErrRtnBankToFutureByFuture(int nErrorId, const QString& errorMsg) ;

    ///�ڻ������ڻ��ʽ�ת���д���ر�
    // void OnErrRtnFutureToBankByFuture(int nErrorId, const QString& errorMsg) ;

    ///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ�����ر�
    // void OnErrRtnRepealBankToFutureByFutureManual(int nErrorId, const QString& errorMsg) ;

    ///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת���д���ر�
    // void OnErrRtnRepealFutureToBankByFutureManual(int nErrorId, const QString& errorMsg) ;

    ///�ڻ������ѯ����������ر�
    // void OnErrRtnQueryBankBalanceByFuture(int nErrorId, const QString& errorMsg) ;

    ///�ڻ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
    // void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    ///�ڻ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
    // void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    ///�ڻ����������ʽ�ת�ڻ�Ӧ��
    // void OnRspFromBankToFutureByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�ڻ������ڻ��ʽ�ת����Ӧ��
    // void OnRspFromFutureToBankByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///�ڻ������ѯ�������Ӧ��
    // void OnRspQueryBankAccountMoneyByFuture(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast) ;

    ///���з������ڿ���֪ͨ
    // void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) ;

    ///���з�����������֪ͨ
    // void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) ;

    ///���з����������˺�֪ͨ
    // void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) ;

public slots:

};

#endif // CTPCLIENTNOTIFIER_H
