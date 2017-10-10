#ifndef CTPACCOUNTMANAGER_H
#define CTPACCOUNTMANAGER_H

#include "ctpclient_global.h"
#include <map>
#include <string>

struct CtpAccountData
{
    ///������
    TThostFtdcDateType	TradingDay;
    ///��¼�ɹ�ʱ��
    TThostFtdcTimeType	LoginTime;
    ///���͹�˾����
    TThostFtdcBrokerIDType	BrokerID;
    ///�û�����
    TThostFtdcUserIDType	UserID;
    ///����ϵͳ����
    TThostFtdcSystemNameType	SystemName;
    ///ǰ�ñ��
    TThostFtdcFrontIDType	FrontID;
    ///�Ự���
    TThostFtdcSessionIDType	SessionID;
    ///��󱨵�����
    TThostFtdcOrderRefType	MaxOrderRef;
};

struct CtpAccount{
    CtpAccountData accountData;
    CThostFtdcSettlementInfoField settlementInfo;
    CThostFtdcSettlementInfoConfirmField settlementInfoConfirm;
};

class CTPCLIENTSHARED_EXPORT CtpAccountManager
{
public:
    static CtpAccountManager& instance();

    void addAccount(const CtpAccountData& account);

    bool addSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField& field);
    bool addSettlementInfo(CThostFtdcSettlementInfoField& field);

    bool getSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField& field, const std::string& user_id);
    bool getSettlementInfo(CThostFtdcSettlementInfoField& field, const std::string& user_id);

    bool initField(CThostFtdcQrySettlementInfoField& field, const std::string& user_id) const;

private:
    CtpAccountManager();

    CtpAccountManager(const CtpAccountManager&) = delete;
    CtpAccountManager& operator=(const CtpAccountManager&) = delete;

    std::map<std::string, CtpAccount> mData;

};

#endif // CTPACCOUNTMANAGER_H
