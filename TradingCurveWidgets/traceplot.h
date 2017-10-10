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
     * \brief canvas widget指被坐标轴围住，包含了用于绘图部分（canvas）的控件。
     *
     * 注意：canvas widget并不一定等同于canvas。真正用于绘图的canvas可能只是canvas widget的一部分。
     * \return canvas widget
     */
    virtual QWidget *canvasWidget() {return static_cast<QWidget*>(traceCanvas);}
    virtual const QWidget *canvasWidget() const { return traceCanvas; }

    /*!
     * \brief 设定titled plot canvas的标题控件。
     * \param titleWidget 标题控件，显示在canvas上方。
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
     * \brief 设置tracer位置。
     *
     * 通过该函数直接设置tracer位置，而不管鼠标位置在哪。
     * \param tracerPosInCanvas tracer在canvas坐标系中的坐标位置。
     */
    void setTracerCanvasPosDirectly(QPoint pos);
    /*!
     * \brief 设置tracer位置。
     *
     * 通过该函数直接设置tracer位置，而不管鼠标位置在哪。
     * \param tracerGlobalPos tracer在屏幕坐标系中的坐标位置。
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
     * \brief 计算Tracer（横线竖线交点）的位置。
     * \param mousePosIncanvas 鼠标在canvas坐标系中的坐标
     * \return 基于canvas坐标系的Tracer显示位置。
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

    /*!是否跟踪鼠标。*/
    bool tracking;
};

}

#endif // TRACEPLOT_H
