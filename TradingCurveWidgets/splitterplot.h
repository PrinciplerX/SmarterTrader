#ifndef SPLITTERPLOT_H
#define SPLITTERPLOT_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "abstracttraceplot.h"
#include "splitterplotcanvas.h"
#include "splitterscalewidget.h"
#include "realtimeitemaxis.h"
#include "plottitlewidget.h"
#include "plotableitem.h"

namespace TradingCurveWidgets{

/*!
 * \brief The SplitterPlot class
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT SplitterPlot : public AbstractTracePlot
{
    Q_OBJECT
public:
    explicit SplitterPlot(Qt::Orientation orientation = Qt::Vertical, int canvasCount = 1, QWidget *parent = 0);

    Qt::Orientation orientation() const;

    // axis widget
    void setTraceAxis(int axisId, AbstractTraceAxis* traceAxis);
    void setSplitterAxis(int axisId, SplitterScaleWidget* splitterAxis);
    virtual AbstractTraceAxis* getTraceAxis( int axisId ) override;
    virtual const AbstractTraceAxis* getTraceAxis( int axisId ) const override;
    bool isSplitterAxis(int axisId);
    void setTracerLabels(QVector<QString>& labels);

    // layout
    inline virtual AbstractPlotLayout *getPlotLayout() override;
    inline virtual const AbstractPlotLayout* getPlotLayout() const override;

    // canvas
    int getCanvasCount() const;
    TracePlotCanvas* getCanvasAt(int index);
    const TracePlotCanvas* getCanvasAt(int index) const;
    virtual QwtScaleMap canvasMap( TracePlotCanvas* canvas, int axisId ) const;
    virtual QwtScaleMap canvasMap( int canvasIndex, int axisId ) const;

    // scale
    virtual void updateAxes() override;
    virtual void updateAxis(int canvasIndex);

    // title
    PlotTitleWidget* getTitleWidget();
    const PlotTitleWidget* getTitleWidget() const;
    void setTitleText(const QString& text);

    /*!
     * \brief 重新计算画布上显示元素的最大值和最小值。
     */
    virtual void updateCanvasRange();
    virtual void updateCanvasRange(int canvasIndex);

    virtual void updateAxesDiv(int canvasIndex);

    virtual void updateCanvasMap();
    virtual void createDefaultAxis( int axisId ) override;

    void addPlotableItem(PlotableItem* item, int canvasIndex);
    int plotableItemCount(int canvasIndex) const;
    void clearPlotableItem();
    void clearPlotableItem(int canvasIndex);

    virtual double regionalMaxValue(int minIndex, int maxIndex, int canvasId) const;
    virtual double regionalMinValue(int minIndex, int maxIndex, int canvasId) const;

    virtual void drawCanvas( QWidget* canvas, QPainter * painter ) override;
    virtual void drawCanvasGrid( QWidget* canvas, QPainter* painter);
    int dataSize(int canvasIndex = 0);

public slots:
    virtual void onTargetMove(QPoint mouseGlobalPos) override;
    virtual void onTargetLeave() override;
    void onCanvasResized(int index);
    void onCanvasResized();

protected:
    virtual QPoint caculateAndUpdateTracerPos(QPoint globalPos) override;

    /*!
     * \brief 确保在显示时总是处于正确状态。
     */
    virtual void showEvent(QShowEvent* event) override;
    /*!
     * \brief 改变大小时，坐标映射关系可能改变，因此必须实时更新。
     */
    virtual void resizeEvent(QResizeEvent* event) override;

    RealTimeItemAxis* itemAxis();

private:
    SplitterPlotCanvas* splitterCanvas;
    AbstractPlotLayout* plotLayout;

    RealTimeItemAxis* mItemAxis;

    PlotTitleWidget* titleWidget;

    AbstractTraceAxis* axisArray[AbstractPlot::axisCnt];

    QVector<QList<PlotableItem*>> itemVector;
};

inline AbstractTraceAxis *SplitterPlot::getTraceAxis(int axisId){ return axisArray[axisId]; }
inline const AbstractTraceAxis *SplitterPlot::getTraceAxis(int axisId) const { return axisArray[axisId]; }

inline AbstractPlotLayout *SplitterPlot::getPlotLayout(){ return plotLayout; }
inline const AbstractPlotLayout *SplitterPlot::getPlotLayout() const { return plotLayout; }

inline int SplitterPlot::getCanvasCount() const { return splitterCanvas->getCanvasCount(); }
inline TracePlotCanvas *SplitterPlot::getCanvasAt(int index){ return splitterCanvas->getCanvasAt(index); }
inline const TracePlotCanvas *SplitterPlot::getCanvasAt(int index) const { return splitterCanvas->getCanvasAt(index); }

inline PlotTitleWidget *SplitterPlot::getTitleWidget() { return titleWidget; }
inline const PlotTitleWidget *SplitterPlot::getTitleWidget() const { return titleWidget; }

inline void SplitterPlot::setTitleText(const QString &text) { titleWidget->setText(text); }

}

#endif // SPLITTERPLOT_H
