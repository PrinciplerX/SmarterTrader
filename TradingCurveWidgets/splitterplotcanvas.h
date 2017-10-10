#ifndef SPLITTERPLOTCANVAS_H
#define SPLITTERPLOTCANVAS_H

#include "tradingcurvewidgets_global.h"
#include "traceplotcanvas.h"
#include <QSplitter>
#include <QList>
#include <QMap>

class AbstractPlot;

namespace TradingCurveWidgets{

/*!
 * \brief 主体部分是一个QSplitter的canvas。可以手动调整内部各部分内容的大小。
 *
 * 设计该类的主要目的是实现多个共用横坐标轴，并且可以手动调整大小的canvas的同步显示。
 * 同时，还可以在代码中自定义不同canvas的坐标轴的显示。
 * 该类刚开始默认仅有一个traceCanvas，但是可以在使用时添加其他控件，包括traceCanvas。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT SplitterPlotCanvas : public QSplitter
{
    Q_OBJECT
public:
    SplitterPlotCanvas(Qt::Orientation orientation = Qt::Vertical, int canvasCount = 1, AbstractPlot* parent = nullptr);

    virtual ~SplitterPlotCanvas();

    void setPlot(AbstractPlot* plot);
    const AbstractPlot* getPlot() const;

    TracePlotCanvas* getMainCanvas();
    int getMainCanvasIndex();
    int getCanvasCount() const;
    int indexOfCanvas(QWidget* canvas) const;
    TracePlotCanvas *getCanvasAt(int index = 0);
    TracePlotCanvas* getTraceCanvas(int index = 0);

    void setCanvasBackground(const QBrush &brush);
    void setCanvasBackground(const QBrush &brush, int index);

    /*!
     * \brief 设置tracer的位置
     * \param tracerPos 基于本地坐标系的tracer坐标
     */
    void setTracerLocalPos(QPoint tracerPos);
    QPoint getTracerLocalPos();

    void setTracerGlobalPos(QPoint tracerPos);
    QPoint getTracerGlobalPos();

    /*!
     * \brief 光标已经离开感应范围，清除游标。
     */
    void clearTracer();

    void appendCanvas();

signals:
    /*!
     * \brief Splitter中的某个canvas大小发生了变化。
     * \param index 大小发生变化的canvas的序号。
     */
    void canvasResized(int index);

    /*!
     * \brief 整个控件大小发生了变化。其中的canvas有些大小改变，有一些可能不变。
     */
    void canvasResized();

    /*!
     * \param targetGlobalPos 目标在显示屏上的全局坐标位置。该信号一般和plot对象绑定。
     */
    void targetMoved(QPoint targetGlobalPos);

    /*!
     * \brief 目标离开canvas区域。该信号一般和plot对象绑定。
     */
    void targetLeaved();

public slots:
    void replot();

protected:
    virtual bool eventFilter(QObject* obj, QEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    /*! canvas之间的序号关系，canvas向量长度最短为1。 */
    QVector<TracePlotCanvas*> canvasVector;

    AbstractPlot* plot;
};

inline const AbstractPlot *SplitterPlotCanvas::getPlot() const { return canvasVector[0]->plot(); }

inline TracePlotCanvas *SplitterPlotCanvas::getMainCanvas() { return canvasVector[getMainCanvasIndex()]; }

inline int SplitterPlotCanvas::getMainCanvasIndex() { return 0; }

inline int SplitterPlotCanvas::getCanvasCount() const { return canvasVector.size(); }

inline TracePlotCanvas *SplitterPlotCanvas::getCanvasAt(int index) { return canvasVector[index]; }

inline TracePlotCanvas *SplitterPlotCanvas::getTraceCanvas(int index) { return canvasVector[index]; }

//inline QPoint SplitterPlotCanvas::getTracerLocalPos() { return canvasVector[index]->mapFrom( tracerPos); }

//inline QPoint SplitterPlotCanvas::getTracerGlobalPos() { return mapToGlobal(tracerPos); }

}

#endif // SPLITTERPLOTCANVAS_H
