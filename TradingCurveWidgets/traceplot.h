#ifndef TRACEPLOT_H
#define TRACEPLOT_H

#include "simpleplot.h"
#include "tracescalewidget.h"
#include "traceplotcanvas.h"
#include "splitterplotcanvas.h"

namespace TradingCurveWidgets{

/*!
 * \brief The TracePlot class
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT TracePlot : public SimplePlot
{
    Q_OBJECT

public:
    TracePlot(QWidget* parent=nullptr);
    TracePlot(TracePlotCanvas::TracerSetting, QWidget* parent=nullptr);

    void hideAxisTick();

    virtual QWidget *getCanvas() override {return traceCanvas->getTraceCanvas();}
    virtual const QWidget *getCanvas() const override;

    /*!
     * \brief canvas widgetָ��������Χס�����������ڻ�ͼ���֣�canvas���Ŀؼ���
     *
     * ע�⣺canvas widget����һ����ͬ��canvas���������ڻ�ͼ��canvas����ֻ��canvas widget��һ���֡�
     * \return canvas widget
     */
    virtual QWidget *canvasWidget() {return static_cast<QWidget*>(traceCanvas);}
    virtual const QWidget *canvasWidget() const { return traceCanvas; }

    /*!
     * \brief �趨titled plot canvas�ı���ؼ���
     * \param titleWidget ����ؼ�����ʾ��canvas�Ϸ���
     */
    void setTitleWidget(QWidget* titleWidget);
    QWidget *getTitleWidget();

    virtual void setTracking(bool enable);

    inline void setTracerSetting(TracePlotCanvas::TracerSetting setting){
        traceCanvas->getTraceCanvas()->setTracerSetting(setting);
    }

    void setScaleWidget(SimplePlot::Axis axis, TraceScaleWidget* scaleWidget);
    TraceScaleWidget *getScaleWidget(SimplePlot::Axis axis);
    void setAxisTracerSetting(SimplePlot::Axis axis,TraceScaleWidget::TracerSetting setting);
    void setAxisTracerSetting(TraceScaleWidget::TracerSetting setting);

    /*!
     * \brief ����tracerλ�á�
     *
     * ͨ���ú���ֱ������tracerλ�ã����������λ�����ġ�
     * \param tracerPosInCanvas tracer��canvas����ϵ�е�����λ�á�
     */
    void setTracerCanvasPosDirectly(QPoint pos);
    /*!
     * \brief ����tracerλ�á�
     *
     * ͨ���ú���ֱ������tracerλ�ã����������λ�����ġ�
     * \param tracerGlobalPos tracer����Ļ����ϵ�е�����λ�á�
     */
    void setTracerGlobalPosDirectly(QPoint tracerGlobalPos);

    void setTracerCanvasPos(QPoint pos);
    void setTracerGlobalPos(QPoint pos);

    TraceScaleWidget *getTraceScaleWidget(SimplePlot::Axis axisId);
    const TracePlotCanvas* getTracePlotCanvas() const { return traceCanvas->getTraceCanvas(); }
    inline TracePlotCanvas* getTracePlotCanvas(){ return traceCanvas->getTraceCanvas(); }

    void hideTracer();

signals:
    void targetMoved(QPoint mouseGlobalPos);
    void targetLeaved();

public slots:
    virtual void onTargetMove(QPoint mouseGlobalPos);
    virtual void onTargetLeave();

protected:
    void init();

    inline SplitterPlotCanvas* getTitledPlotCanvas(){return traceCanvas;}

    /*!
     * \brief ����Tracer���������߽��㣩��λ�á�
     * \param mousePosIncanvas �����canvas����ϵ�е�����
     * \return ����canvas����ϵ��Tracer��ʾλ�á�
     */
    virtual QPoint caculateAndUpdateTracerPos(QPoint mousePosInCanvas);

private:
    TraceScaleWidget* tTsw;
    TraceScaleWidget* bTsw;
    TraceScaleWidget* lTsw;
    TraceScaleWidget* rTsw;

    //TitledPlotCanvas* traceCanvas;
    SplitterPlotCanvas* traceCanvas;

    TracePlotCanvas::TracerSetting tracerSetting;

    /*!�Ƿ������ꡣ*/
    bool tracking;
};

}

#endif // TRACEPLOT_H
