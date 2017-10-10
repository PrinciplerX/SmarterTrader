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

        // 从中间平分，上半部分和下办部分计算得到的interval数目是相同的，而tick数总为奇数。
        int intervalCount = qMax((backboneLength / 2) / stepSize, 1) * 2;
        double interval = (max - min) / intervalCount;

        //先添加顶部的tick，再将下面的空间等分确认剩余的tick
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

        //先添加顶部的tick，再将下面的空间等分确认剩余的tick
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
     * \brief 用于存储定位的线的类型，颜色及宽度选项。以及定位线交点处的图形种类。
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
    绘制坐标轴时三种类型Tick的默认绘图长度。
    *************************************************************/
    enum DefaultTickLength{
        MinorTickLength = 4,
        MediumtickLength = 6,
        MajortickLength = 8
    };

    /*************************************************************
    设置绘制坐标轴时三种类型的Tick长度
    *************************************************************/
    void setTickLength(int minorTickLength=MinorTickLength,
                       int mediumTickLength=MajortickLength,
                       int majortickLength=MajortickLength);

    /*!
     * \brief 更新分度值。该重载函数由SimplePlot中需要更新分度值时使用，或者
     * 可以在别处根据情况使用。
     *
     * 实现对QwtScaleDiv的动态调整，使坐标轴上的tick之间不会贴得太近或太远。
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
    /*! Major Tick之间的最大长度。在updateScaleDiv中使用。 */
    int maxMajorIntervalSpace = 75;
    int lastMajorIntervalCount = -1;

private:
    TracerSetting tracerSetting;

    int tracerLabelMargin = 4;
};

}

#endif // TRACESCALEWIDGET_H
