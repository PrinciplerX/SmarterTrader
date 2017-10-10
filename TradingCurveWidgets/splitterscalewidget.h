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
 * \brief һ������ͬʱ��һ��������ֱ��ˮƽ������ʾ���������Ŀؼ���
 *
 * ����ע�⣡�� ���뽫����� SplitterPlotCanvas ���ʹ�ò������塣�ÿؼ��Ĳ��ֺ����ݽ�ȡ����canvas��
 *
 * �ÿؼ����� SplitterPlotCanvas �е�canvas��С��ϵ��ȷ��
 *
 * ��ͳ��QwtScaleWidgetֻ��������ʾ��������ʾһ�������ᣬ�ں�\a SplitterPlotCanvas ��ϣ��ر��ǵ�
 * \a SplitterPlotCanvas �������canvasʱ������������Э����ʾ���⡣����������������һ�����
 * ���������һ��������ؼ�����\sa SplitterPlotCanvas ������canvas���������Э����ʾ���⡣
 *
 * ����ͼ��ʾ����\a SplitterPlotCanvas �ж��canvas����ֱ�������������һ��\a SplitterScaleWidget
 * ����
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
 * ���˾�����ʾ���������Ĺ������⣬\a SplitterScaleWidget �����ֶ��϶��ֱ��ı��ڲ��������С��ϵ
 * �Ĺ��ܣ�����һ��QSplitter�����Ǹ�������
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
    //! ��������ֶ�ֵ��Scale Division���ı�ʱ���������źš�
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
