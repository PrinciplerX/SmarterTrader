#include "csplotanimation.h"
#include "csplotchartitem.h"

CSPlotAnimation::CSPlotAnimation(CSPlotChartItem *item, int duration, QEasingCurve &curve)
    : QObject(item),
    m_item(item),
    m_animationDuration(duration),
    m_animationCurve(curve)
{
}

CSPlotAnimation::~CSPlotAnimation()
{
}

void CSPlotAnimation::addBox(CandleStick *box)
{
    CandleStickAnimation *animation = m_animations.value(box);
    if (!animation) {
        animation = new CandleStickAnimation(box, this, m_animationDuration, m_animationCurve);
        m_animations.insert(box, animation);
        CandleStickData start;
        qreal mid = (box->m_data.m_lowerQuartile + box->m_data.m_upperQuartile) / 2;
        start.m_lowerExtreme = mid;
        start.m_lowerQuartile = mid;
        start.m_upperQuartile = mid;
        start.m_upperExtreme = mid;
        animation->setup(start, box->m_data);

    } else {
        animation->stop();
        animation->setEndData(box->m_data);
    }
}

ChartAnimation *CSPlotAnimation::boxAnimation(CandleStick *box)
{
    CandleStickAnimation *animation = m_animations.value(box);
    if (animation)
        animation->m_changeAnimation = false;

    return animation;
}

ChartAnimation *CSPlotAnimation::boxChangeAnimation(CandleStick *box)
{
    CandleStickAnimation *animation = m_animations.value(box);
    animation->m_changeAnimation = true;
    animation->setEndData(box->m_data);

    return animation;
}

void CSPlotAnimation::setAnimationStart(CandleStick *box)
{
    CandleStickAnimation *animation = m_animations.value(box);
    animation->setStartData(box->m_data);
}

void CSPlotAnimation::stopAll()
{
    foreach (CandleStick *box, m_animations.keys()) {
        CandleStickAnimation *animation = m_animations.value(box);
        animation->stopAndDestroyLater();
        m_animations.remove(box);
    }
}

void CSPlotAnimation::removeBoxAnimation(CandleStick *box)
{
    m_animations.remove(box);
}
