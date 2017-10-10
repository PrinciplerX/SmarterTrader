#ifndef SIMPLEPLOT_H
#define SIMPLEPLOT_H

#include "tradingcurvewidgets_global.h"
#include "qwt_text.h"
#include "qwt_scale_map.h"
#include <qframe.h>
#include <qlist.h>
#include <qvariant.h>
#include "abstractplot.h"

class AbstractPlotLayout;
//class QwtAbstractLegend;
class SimpleScaleWidget;
class QwtScaleEngine;
class QwtScaleDiv;
class RectangularAxisDraw;
class QwtTextLabel;
class QwtInterval;

/*!
 * \brief һ��2D��ͼ�ؼ���
 *
 * SimplePlot��һ�����ڻ��ƶ�άͼ�εĿؼ����������仭������ʾ���������Ļ�ͼ��λ��plot items������ͼ
 * Ԫ�ؿ��������ߣ�QwtPlotCurve������ǣ�QwtPlotMarker��������QwtPlotGrid���������κδӼ̳��Ի�ͼ
 * Ԫ�ػ���QwtPlotItem�����͡�
 *
 * һ������ͼ��plot����������4�������ᣬÿ����ͼ��λ����һ�����������һ�����������������ÿ��������
 * �ϵķֶ�ֵ������ʾ�趨��QwtScaleDiv��������ʹ��һЩ�㷨��QwtScaleEngine���ӻ�ͼ��λ�м��㣬���ַ�ʽ
 * ���Զ�ÿ����������зֱ�ָ������ʹ�õ��㷨��
 *
 * һ��SimplePlotʵ����һ���˽������������ͼ�ؼ��ĺõ���㣺
 *
 * \image html plot.png
 *
 * \par Example
 * ��������Ӵ��±�ʾ��SimplePlot��򵥵�ʹ�÷�����Ĭ������£�ֻ����ߵ���������͵ײ��ĺ�������ɼ���
 * ������ֶ�ֵ�����Զ�����ġ�
 * \verbatim
 * #include <qwt_plot.h>
 * #include <qwt_plot_curve.h>
 *
 * QwtPlot *myPlot = new QwtPlot("Two Curves", parent);
 *
 * // add curves
 * QwtPlotCurve *curve1 = new QwtPlotCurve("Curve 1");
 * QwtPlotCurve *curve2 = new QwtPlotCurve("Curve 2");
 *
 * // connect or copy the data to the curves
 * curve1->setData(...);
 * curve2->setData(...);
 *
 * curve1->attach(myPlot);
 * curve2->attach(myPlot);
 *
 * // finally, refresh the plot
 * myPlot->replot();
 * \endverbatim
 * \todo Title��Footer��ʾλ�ò���ȷ�����ǳ��������Ͻǣ��������⣩������û�м���QwtPlotItem��֧��Ч
 * ����ͬʱ��Ҳû�м���Ƿ�֧��Qwt�汾��Legend��
 */

class TRADINGCURVEWIDGETSSHARED_EXPORT SimplePlot: public AbstractPlot
{
    Q_OBJECT

    Q_PROPERTY( QBrush canvasBackground
        READ canvasBackground WRITE setCanvasBackground )

#if 0
    // This property is intended to configure the plot
    // widget from a special dialog in the deigner plugin.
    // Disabled until such a dialog has been implemented.

    Q_PROPERTY( QString propertiesDocument
        READ grabProperties WRITE applyProperties )
#endif

public:

    /*!
        Position of the legend, relative to the canvas.

        \sa insertLegend()
     */
    enum LegendPosition
    {
        //! The legend will be left from the QwtPlot::yLeft axis.
        LeftLegend,

        //! The legend will be right from the QwtPlot::yRight axis.
        RightLegend,

        //! The legend will be below the footer
        BottomLegend,

        //! The legend will be above the title
        TopLegend
    };

    explicit SimplePlot( QWidget * = NULL );
    explicit SimplePlot( const QwtText &title, QWidget * = NULL );

    virtual ~SimplePlot();

    void applyProperties( const QString & );
    QString grabProperties() const;

    virtual AbstractAxis *getAxisWidget( int axisId ) override;
    virtual SimpleScaleWidget *getSimpleScaleWidget( int axisId );

    virtual const AbstractAxis *getAxisWidget( int axisId ) const override;
    virtual const SimpleScaleWidget *getSimpleScaleWidget( int axisId ) const;

    virtual AbstractPlotLayout *getPlotLayout() override;
    virtual const AbstractPlotLayout *getPlotLayout() const override;
    void setPlotLayout(AbstractPlotLayout* layout);

    // Title

    void setTitle( const QString & );
    void setTitle( const QwtText &t );
    QwtText title() const;

    QwtTextLabel *titleLabel();
    const QwtTextLabel *titleLabel() const;

    // Footer

    void setFooter( const QString & );
    void setFooter( const QwtText &t );
    QwtText footer() const;

    QwtTextLabel *footerLabel();
    const QwtTextLabel *footerLabel() const;

    // Canvas

    void setCanvasBackground( const QBrush & );
    QBrush canvasBackground() const;

    virtual QwtScaleMap canvasMap( int axisId ) const;

    double invTransform( int axisId, int pos ) const;
    double transform( int axisId, double value ) const;

    // Axes

    virtual void createDefaultAxis( int axisId ) override;

    QwtScaleEngine *axisScaleEngine( int axisId );
    const QwtScaleEngine *axisScaleEngine( int axisId ) const;
    void setAxisScaleEngine( int axisId, QwtScaleEngine * );

    void setAxisAutoScale( int axisId, bool on = true );
    bool axisAutoScale( int axisId ) const;

    void setAxisFont( int axisId, const QFont &f );
    QFont axisFont( int axisId ) const;

    void setAxisScale( int axisId, double min, double max, double step = 0 );
    void setAxisScaleDiv( int axisId, const QwtScaleDiv & );
    void setAxisScaleDraw( int axisId, RectangularAxisDraw * );
    void setAxisWidget(int axisId, SimpleScaleWidget* scaleWidget);

    double axisStepSize( int axisId ) const;
    QwtInterval axisInterval( int axisId ) const;

    const QwtScaleDiv &axisScaleDiv( int axisId ) const;

    const RectangularAxisDraw *axisScaleDraw( int axisId ) const;
    RectangularAxisDraw *axisScaleDraw( int axisId );

    void setAxisLabelAlignment( int axisId, Qt::Alignment );
    void setAxisLabelRotation( int axisId, double rotation );

    void setAxisTitle( int axisId, const QString & );
    void setAxisTitle( int axisId, const QwtText & );
    QwtText axisTitle( int axisId ) const;

    void setAxisMaxMinor( int axisId, int maxMinor );
    int axisMaxMinor( int axisId ) const;

    void setAxisMaxMajor( int axisId, int maxMajor );
    int axisMaxMajor( int axisId ) const;

    // Misc

    virtual QSize sizeHint() const;

    ///////////////////////////////////////////////////////////////////////
    //virtual QSize minimumSizeHint() const;                             //
    ///////////////////////////////////////////////////////////////////////

    virtual void drawCanvas( QWidget* canvas, QPainter * painter ) override;

    virtual void updateAxes() override;

    void updateCanvasMargins();

    virtual void getCanvasMarginsHint(
        const QwtScaleMap maps[], const QRectF &canvasRect,
        double &left, double &top, double &right, double &bottom) const;

    virtual void drawItems( QPainter *, const QRectF &,
        const QwtScaleMap maps[axisCnt] ) const;

private:
    friend class QwtPlotItem;

    void initAxesData();
    void deleteAxesData();
    void updateScaleDiv();

    void initPlot( const QwtText &title );

    class AxisData;
    AxisData *d_axisData[axisCnt];

    class PrivateData;
    PrivateData *d_data;
};

#endif // SIMPLEPLOT_H
