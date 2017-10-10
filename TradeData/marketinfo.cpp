#include "marketinfo.h"

//��̬ʵ�����ڳ�������ʱ����������ر�ʱ���١�
MarketInfo MarketInfo::sInstance;

MarketInfo::MarketInfo()
{
    //a���г�����ʱ�䣺����9:15-11:30������13:00-15:00
    ContinuouTimeInterval aSharesAm;
    aSharesAm.startTime = QTime(9, 15);
    aSharesAm.endTime = QTime(11, 30);
    ContinuouTimeInterval aSharesPm;
    aSharesPm.startTime = QTime(13, 0);
    aSharesPm.endTime = QTime(15, 0);
    aSharesTradeHour.append(aSharesAm);
    aSharesTradeHour.append(aSharesPm);

    //h���г�����ʱ�䣺����9:30-12:00������13:00-16:00
    ContinuouTimeInterval hSharesAm;
    hSharesAm.startTime = QTime(9, 30);
    hSharesAm.endTime = QTime(12, 0);
    ContinuouTimeInterval hSharesPm;
    hSharesPm.startTime = QTime(13, 0);
    hSharesPm.endTime = QTime(16, 0);
    hSharesTradeHour.append(hSharesAm);
    hSharesTradeHour.append(hSharesPm);
}

MarketInfo &MarketInfo::instance()
{
    return sInstance;
}

DailyTradeHour MarketInfo::tradeHour(MarketInfo::Market market) const
{
    switch(market){
    case A_Shares: return aSharesTradeHour;
    case H_Shares: return hSharesTradeHour;
    default:       return DailyTradeHour();
    }
}

