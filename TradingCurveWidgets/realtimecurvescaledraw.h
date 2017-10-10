#ifndef REALTIMECURVESCALEDRAW_H
#define REALTIMECURVESCALEDRAW_H

#include "rectangularaxisdraw.h"
#include "tradingcurvewidgets_global.h"
#include <QPainter>

namespace TradingCurveWidgets {

/*!
 * \brief ��������ֵ��central value�����ƽűꡣ
 *
 * Ĭ�Ͻ���������ֵ�Ľű���Ƴɺ�ɫ��С������ֵ�Ľű���Ƴ���ɫ����������ֵ�Ļ���Ϊ��ɫ��
 *
 * \sa TradingCurveWidgets::RealTimeCurvePlot
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimeCurveScaleDraw : public RectangularAxisDraw
{
public:
    RealTimeCurveScaleDraw(double centralValue);

    virtual void drawLabel(QPainter *painter, double value) const override;

    void setBaseValue(double baseValue);

private:
    double centralValue;
};

}

#endif // REALTIMECURVESCALEDRAW_H
