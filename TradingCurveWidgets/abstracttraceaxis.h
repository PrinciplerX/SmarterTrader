#ifndef ABSTRACTTRACEAXIS_H
#define ABSTRACTTRACEAXIS_H

#include <QWidget>
#include "abstractaxis.h"

/*!
 * \brief 抽象的游标（Tracer）坐标轴。包含了一些游标坐标轴都应该具备的功能。
 *
 * 相对于基类 AbstractAxis 新增的一些概念：
 * 1. Tracer（游标）：游标在坐标轴上显示，用来指示坐标轴上的一个位置，游标的形状可以是任意的。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractTraceAxis : public AbstractAxis
{
    Q_OBJECT
public:
    explicit AbstractTraceAxis( QWidget *parent = NULL );
    explicit AbstractTraceAxis( AxisType axisType, QWidget *parent = NULL );

    void enableTracking(bool enable = true);
    inline bool isTrackingEnabled() const;

    /*!
     * \brief 设置游标的本地坐标位置。
     * \param pos 坐标轴轴线方向上，游标的坐标。例如，横轴对应的横坐标，纵轴对应的纵坐标。
     */
    void setTracerLocalPos(int pos);

    /*!
     * \brief 获取Tracer位置
     * \return Tracer在坐标轴方向上的本地坐标
     */
    int getTracerPos() const;

signals:

public:
    virtual void onTargetLeaved();

protected:
    /*!
     * \brief 绘制游标（Tracer）。
     * \param painter 画笔。
     * \param value 游标在坐标轴方向上的本地坐标。
     */
    virtual void drawTracer(QPainter* painter, int tracerPos) const;

    /*!
     * \brief 绘制坐标图上的固定部分内容（不包括游标）。
     * \param painter 画笔。
     */
    virtual void drawAxis(QPainter* painter) const = 0;

    /*!
     * \brief 绘制坐标图内容。这里绘制过程是固定的，必须先绘制坐标轴内容，再绘制游标。否则会出现
     * 坐标轴内容覆盖游标的情况。
     * \param painter
     */
    virtual void drawContents( QPainter* painter ) const override final;

    /*!
     * \brief 供子类自定义游标位置改变时的动作。
     * \param pos 游标位置。
     * \warning 不要在该函数内设置游标位置，否则会造成函数递归调用产生死循环。
     */
    virtual void onTargetMoved(int pos);
    virtual void onTargetMoved(QPoint globalPos);

private:

    int tracerPosition = -1;

    //! 是否显示游标。
    bool tracking = false;
};

#endif // ABSTRACTTRACEAXIS_H
