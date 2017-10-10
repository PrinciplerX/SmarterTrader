#ifndef TRADEDATAANALYSER_H
#define TRADEDATAANALYSER_H

#include "tradedata.h"
#include "tradedata_global.h"
#include <QList>
#include <QMap>

enum TrendType
{
    UpTrend,
    DownTrend,
    ExpandTrend,
    ShrinkTrend,
    UnknownTrend
};

struct Trend
{
    TrendType type;
    int length;
    QString startTime;
    QString endTime;
};

class TRADEDATASHARED_EXPORT TradeDataAnalyser
{
public:
    TradeDataAnalyser();
    TradeDataAnalyser(const TradeDataSeries& series);

    static TrendType trendType(const TradeData& first, const TradeData& second);

    void analyseTrend(const TradeDataSeries& series);
    static void analyseBars(const TradeDataSeries& series, int& inTrendBarCount, int& insideBarCount, int& outsideBarCount);

    QList<Trend> trends() const;

private:
    QList<Trend> mTrends;
};

#endif // TRADEDATAANALYSER_H
