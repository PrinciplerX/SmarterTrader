#ifndef MARKETINFO_H
#define MARKETINFO_H

#include "tradedata_global.h"
#include "tradedata.h"
#include <QTime>

class TRADEDATASHARED_EXPORT MarketInfo
{
public:
    MarketInfo();

    enum Market{
        A_Shares,               //A��
        H_Shares,               //H��
        Futures                 //�ڻ��г�
    };

    static MarketInfo &instance();

    DailyTradeHour tradeHour(Market market) const;

private:
    static MarketInfo sInstance;

    DailyTradeHour aSharesTradeHour;
    DailyTradeHour hSharesTradeHour;

};

#endif // MARKETINFO_H
