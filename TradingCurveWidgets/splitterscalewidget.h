#ifndef SPLITTERSCALEWIDGET_H
#define SPLITTERSCALEWIDGET_H

#include "tradingcurvewidgets_global.h"
#include <QWidget>
#include <QVector>
#include "abstractaxis.h"
#include "tracescalewidget.h"
#include "abstracttraceaxis.h"
#include "rectangularaxisdraw.h"
#include "splitterplotcanvas.h"
#include "axisdata.h"
#include <qwt_scale_engine.h>

namespace TradingCurveWidgets{

class SplitterPlot;

/*!
 * \brief 一个可以同时在一个方向（竖直、水平）上显示多个坐标轴的控件。
 *
 * ！！注意！！ 必须将该类和 SplitterPlotCanvas 组合使用才有意义。该控件的布局和内容仅取决于canvas。
 *
 * 该控件利用 SplitterPlotCanvas 中的canvas大小关系来确定
 *
 * 传统的QwtScaleWidget只能在其显示区域内显示一个坐标轴，在和\a SplitterPlotCanvas 组合，特别是当
 * \a SplitterPlotCanvas 包括多个canvas时会出现坐标轴的协调显示问题。该类代表对这个问题的一个解决
 * 方案：设计一个坐标轴控件管理\sa SplitterPlotCanvas 中所有canvas的坐标轴的协调显示问题。
 *
 * 如下图所示：在\a SplitterPlotCanvas 中多个canvas的竖直方向的坐标轴由一个\a SplitterScaleWidget
 * 管理。
 * +------------------------------------------------+
 * | S |Title Legend                                |
 * + p +--------------------------------------------+
 * | l |                                            |
 * | i |                                            |
 * | t |                   canvas                   |
 * | t |                                            |
 * | e |                                            |
 * | r +--------------------------------------------+
 * | S |                                            |
 * | c |                                            |
 * | a |                   canvas                   |
 * | l |                                            |
 * | e |                                            |
 * | W +--------------------------------------------+
 * | i |                                            |
 * | d |                                            |
 * | g |                   canvas                   |
 * | e |                                            |
 * | t |                                            |
 * +---+--------------------------------------------+
 *
 * 除了具有显示多个坐标轴的功能以外，\a SplitterScaleWidget 还有手动拖动手柄改变内部坐标轴大小关系
 * 的功能（类似一个QSplitter，但是更加灵活）。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT SplitterScaleWidget : public AbstractTraceAxis
{
    Q_OBJECT
public:
    explicit SplitterScaleWidget( AbstractAxis::AxisType, int axisCount = 0, SplitterPlot *parent = nullptr );
    explicit SplitterScaleWidget( TraceScaleWidget* mainAxis, int axisCount = 0, SplitterPlot* parent = nullptr);

    void appendAxis();
    void appendAxis(TraceScaleWidget* axis);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // axes configuration
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setTickLength(int minorTickLength, int mediumTickLength, int majortickLength);
    void setValueRange(double lowerBound, double upperBound, int axisId);
    void setAxisPalette(const QPalette& palette, int axisIndex);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // scale map, scale div & canvas map
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    QwtScaleMap& scaleMap(int index);
    const QwtScaleMap& scaleMap(int index) const;

    const QwtScaleMap &canvasMap(int index) const;
    QwtScaleMap caculateCanvasMap(int index) const;
    virtual void updateScaleDiv(int index);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // scale draw
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const RectangularAxisDraw* scaleDraw(int index) const;
    RectangularAxisDraw* scaleDraw(int index);
    void setScaleDraw(RectangularAxisDraw* scaleDraw, int index);
    virtual void drawAxis( QPainter *p ) const override;
    virtual void drawTracer(QPainter* painter, int tracerPos) const override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // layout
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void scaleChange();
    virtual void updateLayout( bool update = true ) override;
    virtual void getBorderDistHint( int &start, int &end ) const override;
    virtual int dimHint() const override;

public slots:
    void onCanvasResized(int index);

signals:
    //! 当坐标轴分度值（Scale Division）改变时，发出该信号。
    void scaleDivChanged();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void onTargetMoved(int pos) override;

private:
    QVector<QwtScaleMap> canvasMapVector;
    QVector<TraceScaleWidget*> axesVector;
    QVector<int> axesOffset;
    SplitterPlot *plot;
};

inline QwtScaleMap &SplitterScaleWidget::scaleMap(int index) { return scaleDraw(index)->scaleMap(); }
inline const QwtScaleMap &SplitterScaleWidget::scaleMap(int index) const { return scaleDraw(index)->scaleMap(); }

}

#endif // SPLITTERSCALEWIDGET_H
