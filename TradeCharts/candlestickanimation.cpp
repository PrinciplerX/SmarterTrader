#include "candlestickanimation.h"
#include "csplotanimation.h"

CandleStickAnimation::CandleStickAnimation(CandleStick *box, CSPlotAnimation *boxPlotAnimation, int duration,
                                           QEasingCurve &curve)
    : ChartAnimation(box),
      m_box(box),
      m_changeAnimation(false),
      m_boxPlotAnimation(boxPlotAnimation)
{
    setDuration(duration);
    setEasingCurve(curve);
}

CandleStickAnimation::~CandleStickAnimation()
{
    if (m_boxPlotAnimation)
        m_boxPlotAnimation->removeBoxAnimation(m_box);
}

QVariant CandleStickAnimation::interpolated(const QVariant &from, const QVariant &to, qreal progress) const
{
    CandleStickData startData = qvariant_cast<CandleStickData>(from);
    CandleStickData endData = qvariant_cast<CandleStickData>(to);
    CandleStickData result;

    qreal mid = (endData.m_lowerQuartile + endData.m_upperQuartile) / 2;

    if (m_changeAnimation) {
        result.m_lowerExtreme = startData.m_lowerExtreme + progress * (endData.m_lowerExtreme - startData.m_lowerExtreme);
        result.m_lowerQuartile = startData.m_lowerQuartile + progress * (endData.m_lowerQuartile - startData.m_lowerQuartile);
        result.m_upperQuartile = startData.m_upperQuartile + progress * (endData.m_upperQuartile - startData.m_upperQuartile);
        result.m_upperExtreme = startData.m_upperExtreme + progress * (endData.m_upperExtreme - startData.m_upperExtreme);
    } else {
        result.m_lowerExtreme = mid + progress * (endData.m_lowerExtreme - mid);
        result.m_lowerQuartile = mid + progress * (endData.m_lowerQuartile - mid);
        result.m_upperQuartile = mid + progress * (endData.m_upperQuartile - mid);
        result.m_upperExtreme = mid + progress * (endData.m_upperExtreme - mid);
    }
    result.m_index = endData.m_index;
    result.m_boxItems = endData.m_boxItems;

    result.m_maxX = endData.m_maxX;
    result.m_minX = endData.m_minX;
    result.m_maxY = endData.m_maxY;
    result.m_minY = endData.m_minY;
    result.m_seriesIndex = endData.m_seriesIndex;
    result.m_seriesCount = endData.m_seriesCount;

    return qVariantFromValue(result);
}

void CandleStickAnimation::updateCurrentValue(const QVariant &value)
{
    CandleStickData data = qvariant_cast<CandleStickData>(value);
    m_box->setLayout(data);
}

void CandleStickAnimation::setup(const CandleStickData &startData, const CandleStickData &endData)
{
    setKeyValueAt(0.0, qVariantFromValue(startData));
    setKeyValueAt(1.0, qVariantFromValue(endData));
}

void CandleStickAnimation::setEndData(const CandleStickData &endData)
{
    if (state() != QAbstractAnimation::Stopped)
        stop();

    setEndValue(qVariantFromValue(endData));
}

void CandleStickAnimation::setStartData(const CandleStickData &endData)
{
    if (state() != QAbstractAnimation::Stopped)
        stop();

    setStartValue(qVariantFromValue(endData));
}
