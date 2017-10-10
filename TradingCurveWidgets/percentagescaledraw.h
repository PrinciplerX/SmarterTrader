#ifndef PERCENTAGESCALEDRAW_H
#define PERCENTAGESCALEDRAW_H

#include "rectangularaxisdraw.h"
#include "tradingcurvewidgets_global.h"

namespace TradingCurveWidgets {

class TRADINGCURVEWIDGETSSHARED_EXPORT PercentageScaleDraw : public RectangularAxisDraw
{
public:
    PercentageScaleDraw(double value = 50.0);

    /*!
     * \brief ����ֵת��Ϊ�ٷֱȡ��ٷֱȵļ������baseValue��
     * \param value Ҫת������ֵ��
     * \return �ٷֱȸ�ʽ���ַ�����
     */
    virtual QwtText label(double value) const override;

    virtual void drawLabel(QPainter *painter, double value) const override;

    void setBaseValue(double value);
    double baseValue() const;

private:
    //ʹ�ø�ֵ��������ֵ�ı仯�ٷֱ�
    double mBaseValue;
};

}

#endif // PERCENTAGESCALEDRAW_H
