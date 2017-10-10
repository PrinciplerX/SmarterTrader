///===========================================================================///
/// \date 2016-08-19
/// \author 杨翌超
/// \copyright Copyright ? 2015 - 2017 by YiChao Yang. All Rights Reserved.
///===========================================================================///

#ifndef TRADEDATAOPERATIONS_H
#define TRADEDATAOPERATIONS_H

#include "operation.h"
#include <tradedata.h>

template <typename T> class TradeDataOperation : public Operation<T>
{
public:
    explicit TradeDataOperation(const AbstractHistoryData* input) { mInput = input; }

    virtual int size() const override final { return mInput->size(); }
    virtual T at(int i) const = 0;

    virtual Operation<T>* clone() const = 0;

protected:
    const AbstractHistoryData* historyData() const { return mInput; }

private:
    const AbstractHistoryData* mInput;
};

class HIGH : public TradeDataOperation<double>
{
public:
    explicit HIGH(const AbstractHistoryData* input) : TradeDataOperation<double>(input) {}

    double operator()(int index) const { return historyData()->at(index).high; }
    virtual double at(int i) const override { return historyData()->at(i).high; }

    virtual Operation<double>* clone() const override { return new HIGH(historyData()); }
};

class OPEN : public TradeDataOperation<double>
{
public:
    explicit OPEN(const AbstractHistoryData* input) : TradeDataOperation<double>(input) {}

    double operator()(int index) const { return historyData()->at(index).open; }
    virtual double at(int i) const override { return historyData()->at(i).open; }

    virtual Operation<double>* clone() const override { return new OPEN(historyData()); }
};

class LOW : public TradeDataOperation<double>
{
public:
    explicit LOW(const AbstractHistoryData* input) : TradeDataOperation<double>(input) {}

    double operator()(int index) const { return historyData()->at(index).low; }
    virtual double at(int i) const override { return historyData()->at(i).low; }

    virtual Operation<double>* clone() const override { return new LOW(historyData()); }
};

class CLOSE : public TradeDataOperation<double>
{
public:
    explicit CLOSE(const AbstractHistoryData* input) : TradeDataOperation<double>(input) {}

    double operator()(int index) const { return historyData()->at(index).close; }
    virtual double at(int i) const override { return historyData()->at(i).close; }

    virtual Operation<double>* clone() const override { return new CLOSE(historyData()); }
};

class VOL : public TradeDataOperation<long>
{
public:
    explicit VOL(const AbstractHistoryData* input) : TradeDataOperation<long>(input) {}

    long operator()(int index) const { return historyData()->at(index).volume; }
    virtual long at(int i) const override { return historyData()->at(i).volume; }

    virtual Operation<long>* clone() const override { return new VOL(historyData()); }
};

/*!
 * \brief 真实波幅（True Range，TR）
 * \sa https://en.wikipedia.org/wiki/Average_true_range
 */
class TR : public TradeDataOperation<double>
{
public:
    explicit TR(const AbstractHistoryData* input) : TradeDataOperation<double>(input) {
        HIGH high(input);
        LOW low(input);
        CLOSE close(input);

        mOutput = new MAX<double>(MAX<double>(high - low,
                                              ABS<double>(high - REF<double>(close,1))),
                                              ABS<double>(low - REF<double>(close,1)));
    }

    virtual double at(int i) const override { return mOutput->at(i); }
    virtual Operation<double>* clone() const override { return new TR(historyData()); }

private:
    Operation<double>* mOutput;
};

/*!
 * \brief 平均真实波幅（Average True Range，ATR）
 * \sa https://en.wikipedia.org/wiki/Average_true_range
 */
class ATR : public TradeDataOperation<double>
{
public:
    ATR(const AbstractHistoryData* input, int period) : TradeDataOperation<double>(input) {
        mPeriod = period;
        mOutput = new SMA(TR(input), mPeriod, mPeriod - 1);
    }

    virtual double at(int i) const override { return mOutput->at(i); }
    virtual Operation<double>* clone() const override { return new ATR(historyData(), mPeriod); }

private:
    Operation<double>* mOutput;
    int mPeriod;
};

class OBV : public TradeDataOperation<double>
{
public:
    OBV(const AbstractHistoryData* input) : TradeDataOperation<double>(input)
    {
        CLOSE close(input);
        REF<double> lc = REF<double>(close, 1);

        TypeCastOperation<long, double> vol((VOL(input)));

        mOutput = (SUM<double>(IF<double>(close>lc, vol, IF<double>(close<lc, -vol, 0.0)), 0) / 10000.0).clone();
    }

    virtual void onUpdated() override { mOutput->onUpdated(); }
    virtual void onLoaded() override { mOutput->onLoaded(); }
    virtual void onAppended() override { mOutput->onAppended(); }

    virtual double at(int i) const override { return mOutput->at(i); }
    virtual Operation<double>* clone() const override { return new ATR(historyData(), mPeriod); }

private:
    Operation<double>* mOutput;
    int mPeriod;
};

#endif // TRADEDATAOPERATIONS_H
