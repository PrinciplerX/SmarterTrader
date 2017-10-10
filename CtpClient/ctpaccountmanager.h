#ifndef CTPACCOUNTMANAGER_H
#define CTPACCOUNTMANAGER_H

#include "ctpclient_global.h"
#include <map>
#include <string>

struct CtpAccountData
{
    ///交易日
    TThostFtdcDateType	TradingDay;
    ///登录成功时间
    TThostFtdcTimeType	LoginTime;
    ///经纪公司代码
    TThostFtdcBrokerIDType	BrokerID;
    ///用户代码
    TThostFtdcUserIDType	UserID;
    ///交易系统名称
    TThostFtdcSystemNameType	SystemName;
    ///前置编号
    TThostFtdcFrontIDType	FrontID;
    ///会话编号
    TThostFtdcSessionIDType	SessionID;
    ///最大报单引用
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
