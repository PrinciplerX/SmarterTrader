#ifndef REALTIMECOMPOSITEPLOT_H
#define REALTIMECOMPOSITEPLOT_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "percentagescaledraw.h"
#include "realtimecurvescaledraw.h"
#include "splitterscalewidget.h"
#include "splitterplot.h"

namespace TradingCurveWidgets{

/*!
 * \brief ���ϻ�ͼ�ؼ���������ʾʵʱ���۹�ϵͼ��
 *
 * ��������SplitterPlot�����������ӵ�canvas��ɡ������canvas��ʾ�۸����ƣ��ͼ۸񸽼���Ϣ����
 * �����canvas��ʾ�ɽ�����Ϣ��������������Ϣ��������canvas����һ������ؼ���
 *
 * +-------------------------------------------------+---+
 * |    [TITLE]                                          |
 * + v +---------------------------------------------+ % +
 * |   |                                             |   |
 * |   |                                             |   |
 * |   |                                             |   |
 * |   |                    canvas 1                 |   |
 * |   |                                             |   |
 * |   |                                             |   |
 * |   |                                             |   |
 * +   +---------------------------------------------+   |
 * |   |                                             |   |
 * |   |                                             |   |
 * |   |                                             |   |
 * |   |                    canvas 2                 |   |
 * |   |                                             |   |
 * |   |                                             |   |
 * |   |                                             |   |
 * +---+---------------------------------------------+---+
 * |   date axis                                         |
 * +-----------------------------------------------------+
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimeCompositePlot : public SplitterPlot
{
    Q_OBJECT
public:
    explicit RealTimeCompositePlot(double baseValue = 50.0, QWidget *parent = 0);
    void setBaseValue(double value);
    double baseValue() const;

    void addPricePlotItem(PlotableItem* item);
    void addVolumePlotItem(PlotableItem* item);

    void setData(AbstractHistoryData* data);

    virtual void drawCanvasGrid(QWidget* canvas, QPainter* painter) override;
    virtual void drawVerticalGrid(QWidget* canvas, QPainter* painter);

    virtual void updateAxis(int canvasIndex) override;

    bool isVerticalGridVisible() const;
    void setVerticalGridVisible(bool visible);

protected:
    void initPricePlotScaleDraw(RectangularAxisDraw* scaleDraw);
    void initPlotColor();

protected slots:
    void onDataChanged();

private:
    PercentageScaleDraw* percentageScaleDraw;
    RealTimeCurveScaleDraw* realTimeCurveScaleDraw;

    QMap<int, QPen> mVerticalGrid;

    AbstractHistoryData* mData = nullptr;
    bool mVerticalGridVisible = true;
};

inline void RealTimeCompositePlot::addPricePlotItem(PlotableItem *item) { addPlotableItem(item, 0); }
inline void RealTimeCompositePlot::addVolumePlotItem(PlotableItem *item) { addPlotableItem(item, 1); }

}

#endif // REALTIMECOMPOSITEPLOT_H
