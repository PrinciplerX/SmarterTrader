#include "tradedataanalyser.h"

TradeDataAnalyser::TradeDataAnalyser()
{

}

TradeDataAnalyser::TradeDataAnalyser(const TradeDataSeries &series)
{
    analyseTrend(series);
}

TrendType TradeDataAnalyser::trendType(const TradeData &first, const TradeData &second)
{
    if ((second.high > first.high) && (second.low > first.low))
    {
        return UpTrend;
    }

    if ((second.high < first.high) && (second.low < first.low))
    {
        return DownTrend;
    }

    if (((second.high < first.high) && (second.low > first.low)))
    {
        return ShrinkTrend;
    }

    if (((second.high >= first.high) && (second.low <= first.low)))
    {
        return ExpandTrend;
    }

    return UnknownTrend;
}

void TradeDataAnalyser::analyseTrend(const TradeDataSeries &series)
{
    mTrends.clear();
    if (series.size() < 2) return;

    Trend currentTrend;
    currentTrend.startTime = series.at(0).date;
    currentTrend.type = trendType(series.at(0), series.at(1));
    currentTrend.length = 1;

    for (int i = 2; i < series.size(); i++) {
        TrendType newType = trendType(series.at(i - 1), series.at(i));
        if (newType != currentTrend.type) {
            currentTrend.endTime = series.at(i - 1).date;
            mTrends.push_back(currentTrend);

            currentTrend.startTime = series.at(i).date;
            currentTrend.length = 1;
            currentTrend.type = newType;
        } else {
            currentTrend.length++;
        }
    }

    currentTrend.endTime = series.at(series.size() - 1).date;
    mTrends.push_back(currentTrend);
}

void TradeDataAnalyser::analyseBars(const TradeDataSeries &series, int &inTrendBarCount, int &insideBarCount, int &outsideBarCount)
{
    inTrendBarCount = 0;
    insideBarCount = 0;
    outsideBarCount = 0;

    for (int i = 1; i < series.size(); i++) {
        TrendType newType = trendType(series[i - 1], series[i]);

        if (newType == UpTrend || newType == DownTrend)
        {
            inTrendBarCount++;
        }

        if (newType == ShrinkTrend)
        {
            insideBarCount++;
        }

        if (newType == ExpandTrend)
        {
            outsideBarCount++;
        }
    }
}

QList<Trend> TradeDataAnalyser::trends() const
{
    return mTrends;
}
