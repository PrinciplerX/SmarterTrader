#ifndef PERCENTAGEAXIS_H
#define PERCENTAGEAXIS_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "titledplotaxis.h"
#include "percentagescaledraw.h"

namespace TradingCurveWidgets{

/*!
 * \brief ����TraceScaleWidget���ðٷֱ���ʾ�̶�ֵ���ٷֱȻ���һ���̶������ֵ�������ֶ����á�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT PercentageAxis : public TitledPlotAxis
{
    Q_OBJECT
public:
    explicit PercentageAxis(double baseValue, AbstractAxis::AxisType align, AbstractItemPlot *parent);

    void setBaseValue(double value);

signals:

public slots:

private:
    PercentageScaleDraw* scaleDraw;
};

}

#endif // PERCENTAGEAXIS_H
