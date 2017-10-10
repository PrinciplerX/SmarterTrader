#ifndef TRACESCALEWIDGET_H
#define TRACESCALEWIDGET_H

#include "tradingcurvewidgets_global.h"
#include <QMouseEvent>
#include <QPainter>
#include "simplescalewidget.h"
#include <qwt_text.h>

namespace TradingCurveWidgets{

class TRADINGCURVEWIDGETSSHARED_EXPORT ValueAxisHelper
{
public:
    static double caculateScaleDivRange(int backboneLength, double& min, double& max){
        double pInterval = (max - min) / (backboneLength - 1);
        min = min - 0.5 * pInterval;
        max = min + backboneLength * pInterval;
        return pInterval;
    }

    static QwtScaleDiv caculateBondScaleDiv(double min, double max, int backboneLength, int stepSize){
        caculateScaleDivRange(backboneLength, min, max);

        // ���м�ƽ�֣��ϰ벿�ֺ��°첿�ּ���õ���interval��Ŀ����ͬ�ģ���tick����Ϊ������
        int intervalCount = qMax((backboneLength / 2) / stepSize, 1) * 2;
        double interval = (max - min) / intervalCount;

        //����Ӷ�����tick���ٽ�����Ŀռ�ȷ�ȷ��ʣ���tick
        QList<double> ticks[QwtScaleDiv::NTickTypes];

        ticks[QwtScaleDiv::MajorTick].append(min);
        for(int i = 1; i < intervalCount; i++){
            ticks[QwtScaleDiv::MajorTick].append(min + interval * i);
        }
        ticks[QwtScaleDiv::MajorTick].append(max);

        return QwtScaleDiv(min, max, ticks);
    }

    static QwtScaleDiv caculateMidScaleDiv(double min, double max, int backboneLength, int stepSize){
        caculateScaleDivRange(backboneLength, min, max);

        int intervalCount = qMax(backboneLength / stepSize, 2);
        double interval = (max - min) / intervalCount;

        //����Ӷ�����tick���ٽ�����Ŀռ�ȷ�ȷ��ʣ���tick
        QList<double> ticks[QwtScaleDiv::NTickTypes];

        for(int i = 1; i < intervalCount; i++){
            ticks[QwtScaleDiv::MajorTick].append(min + interval * i);
        }
        return QwtScaleDiv(min, max, ticks);
    }
};

class TRADINGCURVEWIDGETSSHARED_EXPORT TraceScaleWidget : public SimpleScaleWidget
{
    Q_OBJECT
public:
    /*!
     * \brief ���ڴ洢��λ���ߵ����ͣ���ɫ�����ѡ��Լ���λ�߽��㴦��ͼ�����ࡣ
     */
    struct TracerSetting{
        QPen tracerPen;
        QBrush tracerBrush;
        QPen labelPen;
    };

    explicit TraceScaleWidget(AbstractAxis::AxisType align = AbstractAxis::LeftScale,
                              QWidget *parent=nullptr);

    virtual ~TraceScaleWidget();

    inline void setTracerSetting(TracerSetting setting){tracerSetting=setting;}
    inline TracerSetting getTracerSetting() const {return tracerSetting;}
    inline int getTracerLabelMargin() const {return tracerLabelMargin;}

    /*************************************************************
    ����������ʱ��������Tick��Ĭ�ϻ�ͼ���ȡ�
    *************************************************************/
    enum DefaultTickLength{
        MinorTickLength = 4,
        MediumtickLength = 6,
        MajortickLength = 8
    };

    /*************************************************************
    ���û���������ʱ�������͵�Tick����
    *************************************************************/
    void setTickLength(int minorTickLength=MinorTickLength,
                       int mediumTickLength=MajortickLength,
                       int majortickLength=MajortickLength);

    /*!
     * \brief ���·ֶ�ֵ�������غ�����SimplePlot����Ҫ���·ֶ�ֵʱʹ�ã�����
     * �����ڱ𴦸������ʹ�á�
     *
     * ʵ�ֶ�QwtScaleDiv�Ķ�̬������ʹ�������ϵ�tick֮�䲻������̫����̫Զ��
     */
    virtual void updateScaleDiv() override;

    virtual void drawTracerLabel(QPainter* painter, double val) const;

    /*!
     * \brief drawTracer
     * \param painter
     * \param value
     */
    virtual void drawTracer(QPainter* painter, int tracerPos) const;

protected:
    /*! Major Tick֮�����󳤶ȡ���updateScaleDiv��ʹ�á� */
    int maxMajorIntervalSpace = 75;
    int lastMajorIntervalCount = -1;

private:
    TracerSetting tracerSetting;

    int tracerLabelMargin = 4;
};

}

#endif // TRACESCALEWIDGET_H
