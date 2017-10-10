#ifndef CSPLOTANIMATION_H
#define CSPLOTANIMATION_H

#include "candlestick.h"
#include "candlestickanimation.h"
#include "tradecharts_global.h"

class CSPlotChartItem;

class TRADECHARTS_EXPORT CSPlotAnimation : public QObject
{
    Q_OBJECT
public:
    CSPlotAnimation(CSPlotChartItem *item, int duration, QEasingCurve &curve);
    ~CSPlotAnimation();

    void addBox(CandleStick *box);
    ChartAnimation *boxAnimation(CandleStick *box);
    ChartAnimation *boxChangeAnimation(CandleStick *box);

    void setAnimationStart(CandleStick *box);
    void stopAll();
    void removeBoxAnimation(CandleStick *box);

protected:
    CSPlotChartItem *m_item;
    QHash<CandleStick*, CandleStickAnimation*> m_animations;
    int m_animationDuration;
    QEasingCurve m_animationCurve;
};


#endif // CSPLOTANIMATION_H
