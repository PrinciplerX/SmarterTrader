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
        A_Shares,               //A股
        H_Shares,               //H股
        Futures                 //期货市场
    };

    static MarketInfo &instance();

    DailyTradeHour tradeHour(Market market) const;

private:
    static MarketInfo sInstance;

    DailyTradeHour aSharesTradeHour;
    DailyTradeHour hSharesTradeHour;

};

#endif // MARKETINFO_H
