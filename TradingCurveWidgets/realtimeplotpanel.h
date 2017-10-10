#ifndef REALTIMEPLOTPANEL_H
#define REALTIMEPLOTPANEL_H

#include <QFrame>
#include <QSplitter>
#include <QList>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "realtimeplot.h"

namespace TradingCurveWidgets {

class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimePlotPanel : public QFrame
{
    Q_OBJECT
public:
    explicit RealTimePlotPanel(QWidget *parent = 0);

    /*!
     * \brief 添加一个TradingCurvePlot。
     * \param plot 已经初始化乐的TradingCurvePlot。
     */
    RealTimePlotPanel& operator<<(RealTimePlot* plot);
    /*!
     * \brief 添加一个TradingCurvePlot。
     * \param plot 已经初始化乐的TradingCurvePlot。
     */
    void addRealTimePlot(RealTimePlot* plot);

    /*!
     * \brief 设为public函数，以便在更大的控件里调用。
     */
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual bool event(QEvent *e) override;

public slots:
    void onTracerMoved(QPoint tracerGlobalPos);
    void onTracerLeaved();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void customizePlot(RealTimePlot* plot);

private:
    QList<RealTimePlot*> plotList;
    QSplitter* splitter;
    RealTimePlot* bottomPlot = nullptr;
    QPalette plotPalette;
};

}

#endif // REALTIMEPLOTPANEL_H
