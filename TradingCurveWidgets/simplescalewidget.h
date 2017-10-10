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
 * \brief ���õ�������ؼ����ܹ���ʾ�ֶ�ֵ�������ṩ�˻�����canvas���������ʵ��ֵ��ӳ�䡣
 *
 * ����ڻ��� AbstractAxis ������һЩ���
 * 1. Margin
 * 2. Spacing
 * 3. Title
 * 4. ScaleEngine
 * 6. ScaleDraw
 *
 * \todo Title�޷�������ʾ��Ӧ���ǲ������⣩��
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
    //! \todo ��scale map�ı�ĵط��������źš�
    void scaleMapChanged();

public:
    AbstractPlot::Axis getAxisIndex() const;

    void setLayoutFlag( LayoutFlag, bool on );
    bool testLayoutFlag( LayoutFlag ) const;

    virtual void getBorderDistHint( int &start, int &end ) const override;

    /*!
     * \brief �����������Ӧ������ʾ����ֵ��Χ��
     *
     * ����ֵ��Χ��ʾ�������������������ʾ����������������߶ȷ�Χ���ķ�Χ��
     *
     * \warning ����ֵ���������ڲ�������ʾ�����ScaleDiv����ֵ��Χ��ͬ��
     * \param min ������Χ��Сֵ��
     * \param max ������Χ��Сֵ��
     * \param stepSize Major Tick������������� <code>step == 0</code>, �򲽳���������Major�����Զ����㡣
     * \todo stepSize������û��������ʹ�á�
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
      \brief �����������Ͽ�����ʾ�����Major Interval��
      \param maxMajor ����������������ʾ�����Major Interval��
      \sa axisMaxMajor()
    */
    void setMaxMajor(int maxMajor);
    int getMaxMajor() const;

    /*!
      \brief �����������ϵ���Major Interval�п���ʾ�����Minor Interval��
      \param maxMinor ����Major Interval�п���ʾ�����Minor Interval��
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

    double minValue;            //�������ϵ���Сֵ
    double maxValue;            //�������ϵ����ֵ
    double stepSize;            //Major Interval�Ĳ���

    int maxMinor = 5;           //����Major Interval�п���ʾ�����Minor Interval��
    int maxMajor = 8;           //����������������ʾ�����Major Interval��

    QwtScaleEngine *scaleEngine = nullptr;

    class PrivateData;
    PrivateData *d_data;
};

//}

#endif // SIMPLESCALEWIDGET_H
