#include "tradingcurveplotpanel.h"
#include "tradedatahelper.h"
#include "integeraxisdraw.h"

namespace TradingCurveWidgets{

class CascadePlotLayout : public AbstractPlotLayout
{
public:
    CascadePlotLayout(TradingCurvePlotPanel* panel)
    {
        mPlotPanel = panel;
    }

    virtual int axisDimHint(const AbstractPlot *plot, int axisId) const override
    {
        if(plot->isAxisEnabled(axisId))
            return mPlotPanel->axisDimHint(plot, axisId);
        else
            return 0;
    }

private:
    TradingCurvePlotPanel* mPlotPanel;
};

TradingCurvePlotPanel::TradingCurvePlotPanel(QWidget *parent):
    QFrame(parent),
    splitter(new QSplitter(Qt::Vertical)),
    bottomPlot(nullptr)
{
    QVBoxLayout* mainLayout=new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->addWidget(splitter);
    this->setFrameShape(QFrame::StyledPanel);

    splitter->setHandleWidth(1);
    splitter->setStyleSheet("QSplitter::handle { background: darkgray;}");
    splitter->setContentsMargins(QMargins(0,0,0,0));
    splitter->setChildrenCollapsible(false);
    this->setLayout(mainLayout);
}

void TradingCurvePlotPanel::setData(AbstractHistoryData *historyData){
    if(!historyData) return;

    if(data){
        disconnect(data, SIGNAL(dataLoaded()), this, SLOT(onDataLoaded()));
        disconnect(data, SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));
        disconnect(data, SIGNAL(dataAppended()), this, SLOT(onDataAppended()));
    }

    data = historyData;
    connect(data, SIGNAL(dataLoaded()), this, SLOT(onDataLoaded()));
    connect(data, SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));
    connect(data, SIGNAL(dataAppended()), this, SLOT(onDataAppended()));

    for(TradingCurvePlot* plot:plotList){
        plot->setHistoryData(historyData);
    }
    onDataLoaded();
}

TradingCurvePlotPanel& TradingCurvePlotPanel::operator<<(TradingCurvePlot *plot){
    addTradingCurvePlot(plot);
    return *this;
}

void TradingCurvePlotPanel::addTradingCurvePlot(TradingCurvePlot *plot){
    connect(plot, SIGNAL(targetMoved(QPoint)), this, SLOT(onTracerMoved(QPoint)));
    connect(plot, SIGNAL(targetLeaved()), this, SLOT(onTracerLeaved()));

    plot->setPlotLayout(new CascadePlotLayout(this));
    plotList.push_back(plot);
    if(plotList.size() == 2){
        IntegerAxisDraw* scaleDraw = new IntegerAxisDraw();
        scaleDraw->setTickLength(QwtScaleDiv::MajorTick, 0);
        scaleDraw->setTickLength(QwtScaleDiv::MediumTick, 0);
        scaleDraw->setTickLength(QwtScaleDiv::MinorTick, 0);
        plot->getScaleWidget(AbstractPlot::yRight)->setScaleDraw(scaleDraw);
    }

    splitter->addWidget(plot);

    if(bottomPlot != nullptr){
        bottomPlot->disableAxis(AbstractPlot::xBottom);
        disconnect(bottomPlot, SIGNAL(canvasMapChanged(int)), this, SLOT(onBottomPlotCanvasMapChanged(int)));
        disconnect(((TradingCurveItemAxis*)bottomPlot->getItemAxis()), SIGNAL(scaleLevelChanged()), this, SLOT(onScaleLevelChanged()));
        plot->setIndexRange(bottomPlot->getIndexRange());
    }

    connect(plot, SIGNAL(canvasMapChanged(int)), this, SLOT(onBottomPlotCanvasMapChanged(int)));
    connect(((TradingCurveItemAxis*)plot->getItemAxis()), SIGNAL(scaleLevelChanged()), this, SLOT(onScaleLevelChanged()));
    bottomPlot = plot;
}

TradingCurvePlot *TradingCurvePlotPanel::getPlotAt(int index)
{
    return plotList.at(index);
}

void TradingCurvePlotPanel::keyPressEvent(QKeyEvent *event){
    if(bottomPlot != nullptr) {
        switch (event->key()) {
        case Qt::Key_Up:    bottomPlot->zoomIn();  return;
        case Qt::Key_Down:  bottomPlot->zoomOut(); return;
        }
    }
    QFrame::keyPressEvent(event);
}

void TradingCurvePlotPanel::resizeEvent(QResizeEvent *event)
{
    if(bottomPlot != nullptr){
        // 这里需要手动更新。
        bottomPlot->updateAxes();
        for(TradingCurvePlot* plot : plotList){
            if(plot != bottomPlot){
                // 隐藏时间轴的plot在改变大小时，不会触发时间轴得到ResizeEvent，必须在这里手动同步，调整PlotIndexRange。
                plot->setIndexRange(bottomPlot->getIndexRange());
            }
        }
    }
    QFrame::resizeEvent(event);
}

bool TradingCurvePlotPanel::event(QEvent *e)
{
    bool result = QFrame::event(e);
    return result;
}

int TradingCurvePlotPanel::axisDimHint(const AbstractPlot *plot, int axisId) const
{
    if(axisId == AbstractPlot::yRight)
    {
        int result = 0;
        for(TradingCurvePlot* itemPlot : plotList){
            result = qMax(result, itemPlot->getAxisWidget(axisId)->dimHint());
        }
        return result;
    }
    return plot->getAxisWidget(axisId)->dimHint();
}

void TradingCurvePlotPanel::zoomIn()
{
    if (bottomPlot) {
        bottomPlot->zoomIn();
    }
}

void TradingCurvePlotPanel::zoomOut()
{
    if (bottomPlot) {
        bottomPlot->zoomOut();
    }
}

void TradingCurvePlotPanel::onTracerMoved(QPoint tracerGlobalPos){
    for(TradingCurvePlot* plot:plotList){
        plot->setTracerGlobalPos(tracerGlobalPos);
    }
}

void TradingCurvePlotPanel::onTracerLeaved(){
    for(TradingCurvePlot* plot:plotList){
        plot->hideTracer();
    }
}

void TradingCurvePlotPanel::onDataLoaded()
{
    for(TradingCurvePlot* plot:plotList){
        plot->replot();
    }
}

void TradingCurvePlotPanel::onDataUpdated()
{

}

void TradingCurvePlotPanel::onDataAppended()
{

}

void TradingCurvePlotPanel::onBottomPlotCanvasMapChanged(int axisId)
{
    if(axisId != AbstractPlot::xBottom) return;

    for(TradingCurvePlot* plot:plotList){
        if(plot != bottomPlot){
            plot->setIndexRange(bottomPlot->getIndexRange());
            plot->setCanvasMap(bottomPlot->getCanvasMap(axisId), axisId);
        }
    }
}

void TradingCurvePlotPanel::onScaleLevelChanged()
{
    for(TradingCurvePlot* plot:plotList){
        if(plot != bottomPlot){
            plot->setIndexRange(bottomPlot->getIndexRange());
            ((TradingCurveItemAxis*)plot->getItemAxis())->setScaleLevel(
                        ((TradingCurveItemAxis*)bottomPlot->getItemAxis())->getScaleLevel());
            plot->replot();
        }
    }
}

void TradingCurvePlotPanel::mouseMoveEvent(QMouseEvent *event){
    QWidget::mouseMoveEvent(event);
}

}
