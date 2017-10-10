#include "datagenetator.h"
#include <QTime>
#include <QtMath>

TradeDataGenerator::TradeDataGenerator(AbstractHistoryData *data) { mData = data; }

void TradeDataGenerator::setHistoryData(AbstractHistoryData *data) { mData = data; }
const AbstractHistoryData *TradeDataGenerator::historyData() const { return mData; }

RandomDataGenerator::RandomDataGenerator(AbstractHistoryData *data) : TradeDataGenerator(data)
{
    QTime t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
}

TradeData RandomDataGenerator::createNextData()
{
    if (!historyData() || historyData()->size() == 0) {
        return TradeData{1.0, 1.05, 0.95, 1.02, 100, QString("2017-01-01")};
    }

    const TradeData& data = historyData()->at(historyData()->size() - 1);
    double upper = data.close * 1.1;
    double lower = data.close * 0.9;
    double range = upper - lower;

    TradeData next;
    /*******
    next.open   = qrand() / (double)RAND_MAX * range + lower;
    next.high   = qrand() / (double)RAND_MAX * (upper - next.open) + next.open;
    next.low    = qrand() / (double)RAND_MAX * (next.open  - lower) + lower;
    next.close  = qrand() / (double)RAND_MAX * (next.high - next.low) + next.low;
    ********/

    next.open  = upper - qrand() / (double)RAND_MAX * range;
    next.close = upper - qrand() / (double)RAND_MAX * range;
    double r1  = upper - qrand() / (double)RAND_MAX * range;
    double r2  = upper - qrand() / (double)RAND_MAX * range;
    next.high  = qMax(qMax(qMax(next.open, next.close), r1), r2);
    next.low   = qMin(qMin(qMin(next.open, next.close), r1), r2);
    next.volume = (next.high - next.low) * 1000;
    next.date   = QDate::fromString(data.date, QString("yyyy-MM-dd")).addDays(1).toString(QString("yyyy-MM-dd"));

    return next;
}
