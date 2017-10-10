#ifndef TITLEDPLOTAXIS_H
#define TITLEDPLOTAXIS_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "tracescalewidget.h"
#include "abstractitemplot.h"

namespace TradingCurveWidgets{

/*!
 * \brief �䱸TitledPlotCanvas��TitledPlot�е�������ؼ���
 *
 * Ĭ������£���������������ߺͻ������롣���Ƕ��ڰ���һ������ؼ��Ļ���������ʹ��border dist������������֣���������Plot�������⡣
 * ���������border dist����Ϊ0�������������һ����������title widget�Ĳ��֡�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT TitledPlotAxis : public TraceScaleWidget
{
    Q_OBJECT
public:
    explicit TitledPlotAxis(AbstractAxis::AxisType align, AbstractItemPlot *parent = nullptr);

    static AbstractPlot::Axis axisId(AbstractAxis::AxisType alignment);

    virtual void updateScaleDiv() override;
    virtual void updateAxisMap();
    virtual QwtScaleDiv caculateScaleDiv() const;
    virtual QwtScaleMap caculateCanvasMap(const QWidget* canvas) const override;

    void setTitleHeight(int height);

private:
    AbstractItemPlot* plot;

    //����������major tick�������ȡ������᳤�ȱ仯ʱ�������¼���̶�ֵscaleDiv������
    //major tick��Ŀ��ʹmajor tick֮��Ŀ��ʼ�ղ��������ֵ��
    int maxMajorTickSpace = 60;

    // ��¼��ǰ�����ᱻ�̶ȷֳɵ���������
    mutable int majorIntervalCount;

    int titleHeight = 15;
};

}
#endif // TITLEDPLOTAXIS_H
