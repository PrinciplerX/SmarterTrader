#include "ctpaccountmanager.h"
#include <string.h>

using namespace std;

CtpAccountManager::CtpAccountManager()
{

}

CtpAccountManager &CtpAccountManager::instance()
{
    static CtpAccountManager mInstance;
    return mInstance;
}

void CtpAccountManager::addAccount(const CtpAccountData &account)
{
    string id = account.UserID;
    CtpAccount item;
    item.accountData = account;
    mData.insert(pair<string, CtpAccount>(id, item));
}

bool CtpAccountManager::addSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField &field)
{
    string user_id = field.InvestorID;
    std::map<string, CtpAccount>::iterator i = mData.find(user_id);
    if (i != mData.end()) {
        i->second.settlementInfoConfirm = field;
        return true;
    }
    return false;
}

bool CtpAccountManager::addSettlementInfo(CThostFtdcSettlementInfoField &field)
{
    string user_id = field.InvestorID;
    std::map<string, CtpAccount>::iterator i = mData.find(user_id);
    if (i != mData.end()) {
        i->second.settlementInfo = field;
        return true;
    }
    return false;
}

bool CtpAccountManager::getSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField &field, const string &user_id)
{
    std::map<string, CtpAccount>::const_iterator i = mData.find(user_id);
    if (i != mData.end()) {
        field = i->second.settlementInfoConfirm;
        return true;
    }
    return false;
}

bool CtpAccountManager::getSettlementInfo(CThostFtdcSettlementInfoField &field, const string &user_id)
{
    std::map<string, CtpAccount>::const_iterator i = mData.find(user_id);
    if (i != mData.end()) {
        field = i->second.settlementInfo;
        return true;
    }
    return false;
}

bool CtpAccountManager::initField(CThostFtdcQrySettlementInfoField &field, const string &user_id) const
{
    std::map<string, CtpAccount>::const_iterator i = mData.find(user_id);
    if (i != mData.end()) {
        strcpy(field.BrokerID, i->second.accountData.BrokerID);
        strcpy(field.InvestorID, i->second.accountData.UserID);
        strcpy(field.TradingDay, i->second.accountData.TradingDay);
        return true;
    }
    return false;
}
