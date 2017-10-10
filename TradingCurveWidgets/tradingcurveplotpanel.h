#ifndef TRADINGCURVEPLOTPANEL_H
#define TRADINGCURVEPLOTPANEL_H

#include <QWidget>
#include <QSplitter>
#include <QList>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "tradingcurveplot.h"
#include "abstractplotlayout.h"

namespace TradingCurveWidgets{

/*!
 * \brief 把多个TradingCurvePlot垂直叠放到一个控件中，共用相同的
 * 时间轴，同时对鼠标和按键事件做出相应。
 *
 * 基本效果如下所示：
 * +---+--------------------------------------------+
 * |   |Title Legend                                |
 * | A +--------------------------------------------+
 * | x |                                            |
 * | i |                                            |
 * | s |                  canvas                    |
 * |   |                                            |
 * |   |                                            |
 * +---+--------------------------------------------+
 * |   |Title Legend                                |
 * | A +--------------------------------------------+
 * | x |                                            |
 * | i |                                            |
 * | s |                  canvas                    |
 * |   |                                            |
 * |   |                                            |
 * +---+--------------------------------------------+
 * |   |Title Legend                                |
 * | A +--------------------------------------------+
 * | x |                                            |
 * | i |                                            |
 * | s |                  canvas                    |
 * |   |                                            |
 * |   |                                            |
 * +---+--------------------------------------------+
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT TradingCurvePlotPanel : public QFrame
{
    Q_OBJECT

public:
    explicit TradingCurvePlotPanel(QWidget* parent=0);
    virtual ~TradingCurvePlotPanel(){}

    void setData(AbstractHistoryData* historyData);

    /*!
     * \brief 添加一个TradingCurvePlot。
     * \param plot 已经初始化的TradingCurvePlot。
     */
    TradingCurvePlotPanel& operator<<(TradingCurvePlot* plot);

    /*!
     * \brief 添加一个TradingCurvePlot。
     * \param plot 已经初始化的TradingCurvePlot。
     */
    void addTradingCurvePlot(TradingCurvePlot* plot);

    TradingCurvePlot* getPlotAt(int index);

    /*!
     * \brief 设为public函数，以便在更大的控件里调用。
     */
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual bool event(QEvent *e) override;

    /*!
     * \brief 计算坐标轴控件垂直方向上的最优宽度。
     * \param plot 坐标轴所属的绘图控件指针。
     * \param axisId 坐标轴在绘图控件中的位置。
     * \return 坐标轴控件垂直方向上的最优宽度。
     */
    virtual int axisDimHint(const AbstractPlot *plot, int axisId) const;

    void zoomIn();
    void zoomOut();

public slots:
    void onTracerMoved(QPoint tracerGlobalPos);
    void onTracerLeaved();
    void onDataLoaded();
    void onDataUpdated();
    void onDataAppended();

protected slots:
    void onBottomPlotCanvasMapChanged(int axisId);
    void onScaleLevelChanged();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event);
    /*****************************************************************
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    *****************************************************************/
private:
    QList<TradingCurvePlot*> plotList;
    QSplitter* splitter;
    TradingCurvePlot* bottomPlot = nullptr;
    const AbstractHistoryData* data = nullptr;
};

}

#endif // TRADINGCURVEPLOTPANEL_H
