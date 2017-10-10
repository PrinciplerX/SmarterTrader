#ifndef ABSTRACTSCALEDRAW_H
#define ABSTRACTSCALEDRAW_H

#include "tradingcurvewidgets_global.h"
#include <qwt_scale_div.h>
#include <qwt_text.h>
#include <QFont>
#include <QPalette>
#include <QPainter>
#include <qwt_transform.h>
#include <qwt_scale_map.h>

/*!
 * \brief 用于坐标轴绘制工具的抽象基类。
 *
 * AbstractScaleDraw可以绘制基于连续数值的坐标轴，该坐标轴既可以是线性（linear）的，也可以是对
 * 数（logarithmic）的。
 *
 * 在表示坐标轴分度值（scale divison）的 QwtScaleDiv 对象通过 setScaleDiv() 被指定后，坐标轴
 * 可以通过成员函数 draw() 绘制自身内容。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractScaleDraw
{
public:
    /*!
     * \brief 坐标轴组件。
     * \sa enableComponent(), hasComponent
     */
    enum ScaleComponent
    {
        //! 坐标轴轴线（backbone） = 定位刻度（ticks）的轴线。
        Backbone = 0x01,
        //! 刻度（ticks）
        Ticks = 0x02,
        //! 刻度脚标（labels）
        Labels = 0x04
    };

    //! 坐标轴组件。
    typedef QFlags<ScaleComponent> ScaleComponents;

    AbstractScaleDraw();
    virtual ~AbstractScaleDraw();

    void setScaleDiv( const QwtScaleDiv &s );
    const QwtScaleDiv& scaleDiv() const;
    QwtScaleDiv& scaleDiv();

    void setTransformation( QwtTransform * );
    const QwtScaleMap &scaleMap() const;
    QwtScaleMap &scaleMap();

    void enableComponent( ScaleComponent, bool enable = true );
    bool hasComponent( ScaleComponent ) const;

    void setTickLength( QwtScaleDiv::TickType, double length );
    double tickLength( QwtScaleDiv::TickType ) const;
    double maxTickLength() const;

    void setSpacing( double margin );
    double spacing() const;

    void setPenWidth( int width );
    int penWidth() const;

    virtual void draw( QPainter *, const QPalette & ) const;

    virtual QwtText label( double ) const;

    /*!
     * \brief 计算扩展（extent）。
     *
     * 扩展是从坐标轴轴线（baseline）开始，在其垂直方向上，到 scale draw 最外边的像素点的距离。它至少
     * 为 minimumExtent()。
     *
     * \param font 用来绘制刻度脚标（tick labels）的字体。
     * \return 像素数。
     * \sa setMinimumExtent(), minimumExtent()
     */
    virtual double extent( const QFont &font ) const = 0;

    void setMinimumExtent( double );
    double minimumExtent() const;

protected:

    /*!
     * \brief 绘制一个刻度（tick）。
     * \param painter 画笔。
     * \param value 刻度代表的数值。
     * \param len 刻度的长度。
     * \sa drawBackbone(), drawLabel()
     */
    virtual void drawTick( QPainter *painter, double value, double len ) const = 0;

    /*!
     * \brief 绘制坐标轴线（backbone）。
     * \param painter 画笔。
     * \sa drawTick(), drawLabel()
     */
    virtual void drawBackbone( QPainter *painter ) const = 0;

    /*!
     * \brief 绘制刻度（tick）脚标（label），只有主刻度（major tick）才有脚标。
     * \param painter 画笔。
     * \param value 脚标显示的数值。
     * \sa drawTick(), drawBackbone()
     */
    virtual void drawLabel( QPainter *painter, double value ) const = 0;

    void invalidateCache();
    const QwtText &tickLabel( const QFont &, double value ) const;

private:
    AbstractScaleDraw( const AbstractScaleDraw & );
    AbstractScaleDraw &operator=( const AbstractScaleDraw & );

    class PrivateData;
    PrivateData *d_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( AbstractScaleDraw::ScaleComponents )

#endif // ABSTRACTSCALEDRAW_H
