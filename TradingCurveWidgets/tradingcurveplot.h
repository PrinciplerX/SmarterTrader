#ifndef TRADINGCURVEPLOT_H
#define TRADINGCURVEPLOT_H

#include "tradingcurvewidgets_global.h"
#include "traceplot.h"
#include <QKeyEvent>
#include "qwt_date_scale_draw.h"
#include "qwt_date_scale_engine.h"
#include "abstractitemplot.h"
#include "tradingcurveitemaxis.h"
#include <QResizeEvent>
#include <tradedata.h>
#include <QList>
#include <QLabel>

#define BASIC_SCALE_LEVEL_COUNT 11

namespace TradingCurveWidgets{

/*!
 * \brief 用于显示价格走势的专用控件。可以绘制价格走势图以及辅助的技术分析线图。
 *
 * 该类包含了要表示的OHLC数据。绘制前，该控件必须知道坐标合理的范围，以便能够完
 * 全显示在特定时间段内的价格走势图。当用户调整显示的时间段时，即重新绘制前，需
 * 要调整坐标轴。
 *
 * 坐标轴控件（数值轴和时间轴）只负责显示，显示范围以及时间刻度值（label）的计算
 * 部分都由本类负责。
 *
 * 合理的坐标显示范围应该能够包括目前包括技术分析线图在内的所有数据。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT TradingCurvePlot : public AbstractItemPlot
{
    Q_OBJECT
public:
    explicit TradingCurvePlot(QWidget* parent=nullptr);

    virtual ~TradingCurvePlot();

    virtual void keyPressEvent(QKeyEvent* event) override;

    virtual AbstractItemAxis *getItemAxis() override;
    virtual const AbstractItemAxis *getItemAxis() const override;

    virtual int bodyWidth() const override;

    void setHistoryData(AbstractHistoryData* data);

    void zoomIn();
    void zoomOut();

public slots:
    void onDataUpdated();
    void onDataLoaded();

private:
    TradingCurveItemAxis* itemAxis;
    AbstractHistoryData* data = nullptr;
};

}
#endif // TRADINGCURVEPLOT_H
