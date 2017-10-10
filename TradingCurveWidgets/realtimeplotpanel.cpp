#include "realtimeplotpanel.h"

namespace TradingCurveWidgets {

RealTimePlotPanel::RealTimePlotPanel(QWidget *parent):
    QFrame(parent),
    splitter(new QSplitter(Qt::Vertical)),
    bottomPlot(nullptr)
{
    QVBoxLayout* mainLayout=new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->addWidget(splitter);
    this->setFrameShape(QFrame::StyledPanel);

    splitter->setHandleWidth(0);
    splitter->setContentsMargins(QMargins(0,0,0,0));
    splitter->setChildrenCollapsible(false);
    this->setLayout(mainLayout);

    // this->setStyleSheet(QString("background: black; font: white;"));
}

RealTimePlotPanel& RealTimePlotPanel::operator<<(RealTimePlot *plot){
    addRealTimePlot(plot);
    return *this;
}

void RealTimePlotPanel::addRealTimePlot(RealTimePlot *plot){
    connect(plot, SIGNAL(targetMoved(QPoint)), this, SLOT(onTracerMoved(QPoint)));
    connect(plot, SIGNAL(targetLeaved()), this, SLOT(onTracerLeaved()));

    customizePlot(plot);
    plotList.push_back(plot);

    splitter->addWidget(plot);

    if(bottomPlot != nullptr){
        bottomPlot->disableAxis(AbstractPlot::xBottom);
        plot->getTitleWidget()->hide();
    }else{
        plot->enableAxis(AbstractPlot::xBottom);
    }

    bottomPlot = plot;
}

void RealTimePlotPanel::keyPressEvent(QKeyEvent *event){
    QFrame::keyPressEvent(event);

    if(bottomPlot != nullptr){
        bottomPlot->keyPressEvent(event);

        for(RealTimePlot* plot:plotList){
            if(plot != bottomPlot){
                plot->keyPressEvent(event);
            }
        }
    }

    event->accept();
}

void RealTimePlotPanel::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
}

bool RealTimePlotPanel::event(QEvent *e)
{
    bool result = QFrame::event(e);
    return result;
}

void RealTimePlotPanel::onTracerMoved(QPoint tracerGlobalPos){
    for(RealTimePlot* plot:plotList){
        plot->setTracerGlobalPos(tracerGlobalPos);
    }
}

void RealTimePlotPanel::onTracerLeaved(){
    for(RealTimePlot* plot:plotList){
        plot->hideTracer();
    }
}
void RealTimePlotPanel::mouseMoveEvent(QMouseEvent *event){
    QWidget::mouseMoveEvent(event);
}

void RealTimePlotPanel::customizePlot(RealTimePlot *plot)
{
    // #02E2F4 - 淡蓝色
    // #800000 - 暗红色
    // #FF3232 - 鲜红色

    // QPalette::Text - 刻度脚标
    // QPalette::WindowText - 坐标轴线
    // QPalette::HighlightedText - 游标文字

    QPalette axisPalette = this->palette();
    axisPalette.setColor(QPalette::Window, Qt::black);
    axisPalette.setColor(QPalette::WindowText, QColor("#800000"));

    if(bottomPlot == nullptr){
        // RealTimeCurvePlot
    }else{
        // RealTimePlot
        QPalette pal= axisPalette;
        pal.setColor(QPalette::Text, QColor("#FF3232"));
        pal.setColor(QPalette::HighlightedText, QColor("#FF3232"));
        plot->getAxisWidget(AbstractPlot::xBottom)->setPalette(pal);

        axisPalette.setColor(QPalette::Text, QColor("#02E2F4"));
    }

    plot->setStyleSheet(QString("background: black; font: white; font-size: 12px;"));
    plot->getAxisWidget(AbstractPlot::xTop)->setPalette(axisPalette);
    plot->getAxisWidget(AbstractPlot::yLeft)->setPalette(axisPalette);
    plot->getAxisWidget(AbstractPlot::yRight)->setPalette(axisPalette);
}

}
