#ifndef SIMPLESCALEWIDGET_H
#define SIMPLESCALEWIDGET_H

#include "qwt_text.h"
#include "rectangularaxisdraw.h"
#include "tradingcurvewidgets_global.h"
#include "abstractaxis.h"
#include <qwidget.h>
#include <qfont.h>
#include <qcolor.h>
#include <qstring.h>
#include <QPaintEvent>
#include "qwt_scale_engine.h"
#include "abstracttraceaxis.h"
#include "abstractplot.h"

class QPainter;
class QwtTransform;
class QwtScaleDiv;
class QwtColorMap;

//namespace TradingCurveWidgets{

/*!
 * \brief 常用的坐标轴控件，能够显示分度值，并且提供了画布（canvas）坐标和真实数值的映射。
 *
 * 相对于基类 AbstractAxis 新增的一些概念：
 * 1. Margin
 * 2. Spacing
 * 3. Title
 * 4. ScaleEngine
 * 6. ScaleDraw
 *
 * \todo Title无法正常显示（应该是布局问题）。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT SimpleScaleWidget : public AbstractTraceAxis
{
    Q_OBJECT

public:
    //! Layout flags of the title
    enum LayoutFlag
    {
        /*!
          The title of vertical scales is painted from top to bottom.
          Otherwise it is painted from bottom to top.
         */
        TitleInverted = 1
    };

    //! Layout flags of the title
    typedef QFlags<LayoutFlag> LayoutFlags;

    explicit SimpleScaleWidget( QWidget *parent = NULL );
    explicit SimpleScaleWidget( AbstractAxis::AxisType, QWidget *parent = NULL );
    virtual ~SimpleScaleWidget();

signals:
    //! Signal emitted, whenever the scale division changes
    void scaleDivChanged();
    //! \todo 在scale map改变的地方发出此信号。
    void scaleMapChanged();

public:
    AbstractPlot::Axis getAxisIndex() const;

    void setLayoutFlag( LayoutFlag, bool on );
    bool testLayoutFlag( LayoutFlag ) const;

    virtual void getBorderDistHint( int &start, int &end ) const override;

    /*!
     * \brief 设置坐标轴对应画布显示的数值范围。
     *
     * 该数值范围表示的是坐标轴可以正常显示（即不超出坐标轴尺度范围）的范围。
     *
     * \warning 该数值和坐标轴内部用于显示自身的ScaleDiv的数值范围不同。
     * \param min 画布范围最小值。
     * \param max 画布范围最小值。
     * \param stepSize Major Tick步长，如果设置 <code>step == 0</code>, 则步长将会利用Major设置自动计算。
     * \todo stepSize参数并没有真正被使用。
     * \sa setAxisMaxMajor(), setAxisAutoScale(), axisStepSize(), QwtScaleEngine::divideScale()
     */
    void setCanvasValueRange(double min, double max, double stepSize);
    virtual void updateScaleDiv();

    double getCanvasMaxValue() const;
    double getCanvasMinValue() const;
    void getCanvasValueRange(double& min, double& max) const;
    inline double getStepSize() const { return stepSize; }

    void setSpacing( int td );
    int spacing() const;

    void setMargin(int margin);
    int getMargin() const;

    /*!
      \brief 设置坐标轴上可以显示的最多Major Interval数
      \param maxMajor 可以在坐标轴上显示的最多Major Interval数
      \sa axisMaxMajor()
    */
    void setMaxMajor(int maxMajor);
    int getMaxMajor() const;

    /*!
      \brief 设置坐标轴上单个Major Interval中可显示的最多Minor Interval数
      \param maxMinor 单个Major Interval中可显示的最多Minor Interval数
      \sa axisMaxMinor()
    */
    void setAxisMaxMinor(int maxMinor);
    int getMaxMinor() const;

    void setScaleDiv( const QwtScaleDiv &sd );
    void setScaleEngine( QwtScaleEngine* engine);
    const QwtScaleEngine* getScaleEngine() const;
    QwtScaleEngine* getScaleEngine();

    void setTransformation( QwtTransform * );

    void setScaleDraw( RectangularAxisDraw * );
    const RectangularAxisDraw *scaleDraw() const;
    RectangularAxisDraw *scaleDraw();

    virtual QwtScaleMap caculateCanvasMap(const QWidget* canvas) const;

    void setLabelAlignment( Qt::Alignment );
    void setLabelRotation( double rotation );

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    virtual int dimHint() const;

    int titleHeightForWidth( int width ) const;
    int dimForLength( int length, const QFont &scaleFont ) const;

    void setAlignment( AbstractAxis::AxisType );
    AbstractAxis::AxisType alignment() const;

    virtual void drawAxis( QPainter* painter ) const override;

    void scaleChange();
    virtual void updateLayout( bool update = true ) override;

private:
    void initScale( AbstractAxis::AxisType );

    double minValue;            //坐标轴上的最小值
    double maxValue;            //坐标轴上的最大值
    double stepSize;            //Major Interval的步长

    int maxMinor = 5;           //单个Major Interval中可显示的最多Minor Interval数
    int maxMajor = 8;           //可以在坐标轴上显示的最多Major Interval数

    QwtScaleEngine *scaleEngine = nullptr;

    class PrivateData;
    PrivateData *d_data;
};

//}

#endif // SIMPLESCALEWIDGET_H
