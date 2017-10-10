#ifndef REALTIMECURVEPLOT_H
#define REALTIMECURVEPLOT_H

#include <QWidget>
#include "realtimeplot.h"
#include "tradingcurvewidgets_global.h"
#include "percentageaxis.h"

namespace TradingCurveWidgets{

/*!
 * \brief ������ʾʵʱ�۸����ߣ�������ֵ������Ͱٷֱ������ᡣ
 *
 * �Զ��������᷶Χ�Ϳ̶�ֵ��ʹ��ʾ�����ݶ����ڻ���ĳ����ֵ(centralValue)�ĶԳ����䷶Χ�ڡ�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimeCurvePlot : public RealTimePlot
{
    Q_OBJECT
public:
    /*!
     * \brief ���캯��
     * \param value ����λ����ֵ
     * \param parent ���ؼ�
     */
    explicit RealTimeCurvePlot(double value = 0.0, QWidget *parent = nullptr);

    /*!
     * \todo ����û�н�����ȫ�����ã�����ֻ������percentageAxis��
     */
    void setCentralValue(double value);

    virtual void drawCanvasGrid(QPainter* painter) override;

private:
    double centralValue;
    PercentageScaleDraw* percentageScaleDraw;
};

}

#endif // REALTIMECURVEPLOT_H
