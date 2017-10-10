#ifndef CANDLESTICKANIMATION_H
#define CANDLESTICKANIMATION_H

#include <ChartAnimation>
#include "candlestickdata.h"
#include "candlestick.h"
#include "tradecharts_global.h"

SCHARTS_USE_NAMESPACE

class CSPlotAnimation;

class TRADECHARTS_EXPORT CandleStickAnimation : public ChartAnimation
{
    Q_OBJECT
public:
    CandleStickAnimation(CandleStick *box, CSPlotAnimation *boxPlotAnimation, int duration,
                         QEasingCurve &curve);
    ~CandleStickAnimation();

public: // from QVariantAnimation
    virtual QVariant interpolated(const QVariant &from, const QVariant &to, qreal progress) const;
    virtual void updateCurrentValue(const QVariant &value);

    void setup(const CandleStickData &startData, const CandleStickData &endData);
    void setEndData(const CandleStickData &endData);
    void setStartData(const CandleStickData &endData);

    void moveMedianLine(bool move);

protected:
    friend class CSPlotAnimation;
    CandleStick *m_box;
    bool m_changeAnimation;
    CSPlotAnimation *m_boxPlotAnimation;
};

#endif // CANDLESTICKANIMATION_H
