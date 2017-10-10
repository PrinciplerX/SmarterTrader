#ifndef SIMPLEPLOTLAYOUT_H
#define SIMPLEPLOTLAYOUT_H

#include "qwt_global.h"
#include "simpleplot.h"
#include "tradingcurvewidgets_global.h"
#include "abstractplotlayout.h"

/*!
 * \brief SimplePlot的布局引擎。
 *
 * SimplePlot控件使用该类来组织内部控件的布局，或者在SimplePlot::print()中使用，来将其内容渲染到
 * 一个 QPaintDevice（如 QPrinter，QPixmap/QImage，或者QSvgRender）中。
 *
 * \sa SimplePlot::setPlotLayout()
 */

class TRADINGCURVEWIDGETSSHARED_EXPORT SimplePlotLayout : public AbstractPlotLayout
{
public:
    /*!
      Options to configure the plot layout engine
      \sa activate(), QwtPlotRenderer
     */
    enum Option
    {
        //! Unused
        AlignScales = 0x01,

        /*!
          Ignore the dimension of the scrollbars. There are no
          scrollbars, when the plot is not rendered to widgets.
         */
        IgnoreScrollbars = 0x02,

        //! Ignore all frames.
        IgnoreFrames = 0x04,

        //! Ignore the legend.
        IgnoreLegend = 0x08,

        //! Ignore the title.
        IgnoreTitle = 0x10,

        //! Ignore the footer.
        IgnoreFooter = 0x20
    };

    //! Layout options
    typedef QFlags<Option> Options;

    explicit SimplePlotLayout();
    virtual ~SimplePlotLayout();

    void setSpacing( int );
    int spacing() const;

    void setLegendPosition( SimplePlot::LegendPosition pos, double ratio );
    void setLegendPosition( SimplePlot::LegendPosition pos );
    SimplePlot::LegendPosition legendPosition() const;

    void setLegendRatio( double ratio );
    double legendRatio() const;

    virtual QSize minimumSizeHint( const SimplePlot * ) const;

    virtual void activate(const SimplePlot* plot, const QRectF &rect);

    QRectF titleRect() const;
    QRectF footerRect() const;
    QRectF legendRect() const;

    class LayoutData;

protected:

    void setTitleRect( const QRectF & );
    void setFooterRect( const QRectF & );
    void setLegendRect( const QRectF & );

    QRectF layoutLegend( Options options, const QRectF & ) const;
    QRectF alignLegend( const QRectF &canvasRect,
        const QRectF &legendRect ) const;

    void expandLineBreaks( Options options, const QRectF &rect,
        int &dimTitle, int &dimFooter, int dimAxes[AbstractPlot::axisCnt] ) const;

private:

    void invalidate();

    Options layoutOptions = 0x00;

    class PrivateData;

    PrivateData *d_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( SimplePlotLayout::Options )

#endif // SIMPLEPLOTLAYOUT_H
