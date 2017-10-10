#ifndef ABSTRACTPLOTLAYOUT_H
#define ABSTRACTPLOTLAYOUT_H

#include "tradingcurvewidgets_global.h"
#include "abstractplot.h"

/*!
 * \brief The AbstractPlotLayout class
 *
 * 相关概念：
 * 1. Spacing：坐标图各组件之间的间距，最小为0。\sa setSpacing(), getSpacing()
 * 2. CanvasMargin：坐标轴控件和画布的间距。
 * 3. CanvasContentMargin：画布可能是有边框的，画布的内容和画布控件边界的之间的部分就是画布边框的区域。布局时
 * 必须要考虑到这一点，否则会导致坐标轴和画布没有对齐。特别注意 CanvasContentMargin 和 CanvasMargin 的区别。
 *
 * \todo 如果画布有自己的边框，如何布局？
 * \todo Spacing如何体现在布局过程中？
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractPlotLayout
{
public:
    enum LayoutOption{
        // 最小化 CanvasMargin。此时，纵轴和画布的间距总是设为0，横轴总是根据纵轴的BorderDist尽可能贴近画布。
        // 当纵轴的BorderDist都为0时，画布在四条边上全部贴紧坐标轴。
        MinimizeCanvasMargin,

        // 最大化 CanvasMargin。此时，画布在四条边上和坐标轴的间距总是相等，并且等于四个间距在使用最小化画布
        // 间距选项（MinimizeCanvasMargin）时的最大值。
        MaximizeCanvasMargin,
    };

    explicit AbstractPlotLayout();
    virtual ~AbstractPlotLayout();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 布局信息
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    QRectF getScaleRect( int axis ) const;
    QRectF getCanvasRect() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 布局选项
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setSpacing( int );
    int getSpacing() const;

    void setAlignCanvasToScale( int axisId, bool );
    void setAlignCanvasToScales( bool );

    bool isCanvasAlignedToScale( int axisId ) const;

    void setLayoutOption(LayoutOption option);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 组件2：画布（canvas）
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setCanvasMargin( int margin, int axis = -1 );
    int getCanvasMargin( int axis ) const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 布局操作
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void activate(const AbstractPlot* plot, const QRectF &rect);

    virtual int axisDimHint(const AbstractPlot *plot, int axisId) const;

    virtual QSize minimumSizeHint(const AbstractPlot * ) const;

    class LayoutData;

protected:

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 供具备更多布局功能的子类来设置该类的数据成员
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setScaleRect( int axis, const QRectF &rect );
    void setCanvasRect( const QRectF &rect );

private:

    void invalidate();

    class PrivateData;

    PrivateData *d_data;

    QRectF axisRect[AbstractPlot::axisCnt];
    QRectF canvasRect;

    unsigned int canvasMargin[AbstractPlot::axisCnt];

    bool alignCanvasToScales[AbstractPlot::axisCnt];
    unsigned int spacing;

    LayoutOption layoutOption = LayoutOption::MinimizeCanvasMargin;
};

#endif // ABSTRACTPLOTLAYOUT_H
