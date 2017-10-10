#ifndef ABSTRACTTRACEPLOT_H
#define ABSTRACTTRACEPLOT_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "abstractplot.h"
#include "abstracttraceaxis.h"

/*!
 * \brief 跟踪光标位置的坐标图控件。
 *
 * 当光标（鼠标指针）位置在画布上移动时。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractTracePlot : public AbstractPlot
{
    Q_OBJECT
public:
    explicit AbstractTracePlot(QWidget *parent = 0);

    virtual AbstractAxis *getAxisWidget( int axisId ) override final;
    virtual const AbstractAxis *getAxisWidget(int axisId) const override final;

    inline virtual AbstractTraceAxis* getTraceAxis( int axisId ) = 0;
    inline virtual const AbstractTraceAxis* getTraceAxis( int axisId ) const = 0;

signals:
    void targetMoved(QPoint mouseGlobalPos);
    void targetLeaved();

public slots:
    virtual void onTargetMove(QPoint mouseGlobalPos) = 0;
    virtual void onTargetLeave() = 0;

protected:
    /*!
     * \brief 计算Tracer（横线竖线交点）的位置。
     * \param mousePosIncanvas 鼠标在canvas坐标系中的坐标
     * \return 基于全局坐标系（屏幕坐标系）的Tracer显示位置。
     */
    virtual QPoint caculateAndUpdateTracerPos(QPoint globalPos) = 0;

private:

};

#endif // ABSTRACTTRACEPLOT_H
