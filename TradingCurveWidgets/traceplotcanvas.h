#ifndef TRACEPLOTCANVAS_H
#define TRACEPLOTCANVAS_H
#include <QMouseEvent>
#include "simpleplotcanvas.h"
#include <QPen>

namespace TradingCurveWidgets{

class TRADINGCURVEWIDGETSSHARED_EXPORT TracePlotCanvas : public SimplePlotCanvas
{
    Q_OBJECT

public:
    /*!
     * \brief 用于存储定位的线的类型，颜色及宽度选项。以及定位线交点处的图形种类。
     */
    struct TracerSetting{
        QPen pen;
    };

    explicit TracePlotCanvas( AbstractPlot * = NULL );
    explicit TracePlotCanvas( TracerSetting setting, AbstractPlot *parent=NULL);

    inline void setTracerSetting(TracePlotCanvas::TracerSetting setting){
        tracerSetting=setting;
    }
    virtual void setTracking(bool enable);

    /*!
     * \brief 设置tracer的位置
     * \param tracerPos 基于本地坐标系的tracer坐标
     */
    void setTracerPos(QPoint tracerPos);
    inline QPoint getTracerPos(){return tracerPos;}
    /*!
     * \brief 光标已经离开感应范围，清除游标。
     */
    void clearTracer();

signals:
    /*!
     * \param targetGlobalPos 目标在显示屏上的全局坐标位置。
     */
    void targetMoved(QPoint targetGlobalPos);
    /*!
     * \brief 目标离开canvas区域
     */
    void targetLeaved();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

private:
    QPoint tracerPos = QPoint(-1,-1);
    TracerSetting tracerSetting;
    bool tracking;
};

}

#endif // TRACEPLOTCANVAS_H
