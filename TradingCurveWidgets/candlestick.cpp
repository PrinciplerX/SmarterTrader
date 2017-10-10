#include "candlestick.h"
#include <qwt_scale_map.h>

const QString CandleStick::mCategory = QString("CANDLESTICK");

CandleStick::CandleStick(AbstractHistoryData* data):
    TechniqueIndicator(QString("BIAS")),
    open(OPEN(data)),
    high(HIGH(data)),
    low(LOW(data)),
    close(CLOSE(data)),
    mHistoryData(data)
{
    gain = new ReusableOperationWrapper<bool>(IF<bool>((close == open), (close > REF<double>(close, 1)), (close > open)));
    initData();
}

void CandleStick::draw(const QVector<int>& itemPosX, TradingCurveWidgets::IndexRange indexRange, int bodyWidth, const QwtScaleMap &yMap, QPainter *painter){
    int x;

    double highY;
    double lowY;
    double openY;
    double closeY;

    QPen raisePen;
    QPen fallPen;
    QBrush raiseBrush(Qt::SolidPattern);
    QBrush fallBrush(Qt::SolidPattern);

    switch(getTheme()){
    case Dark:
        fallPen.setColor(Qt::black);
        fallBrush.setColor(Qt::black);
        raisePen.setColor(Qt::red);
        raiseBrush.setColor(Qt::black);
        break;
    case Light:
        fallPen.setColor(Qt::darkGreen);
        fallBrush.setColor(Qt::darkGreen);
        raisePen.setColor(Qt::red);
        raiseBrush.setColor(Qt::white);
        break;
    case HighContrastWhite:
        fallPen.setColor(Qt::black);
        fallBrush.setColor(Qt::black);
        raisePen.setColor(Qt::black);
        raiseBrush.setColor(Qt::white);
        break;
    case HighContrastBlack:
        fallPen.setColor(Qt::white);
        fallBrush.setColor(Qt::white);
        raisePen.setColor(Qt::white);
        raiseBrush.setColor(Qt::black);
        break;
    }

    painter->save();

    int maxIndex = indexRange.maxIndex() + 1;
    for(int i = indexRange.minIndex; i < maxIndex; i++){
        x = itemPosX.at(i-indexRange.minIndex);
        highY=yMap.transform(high(i));
        lowY=yMap.transform(low(i));
        openY=yMap.transform(open(i));
        closeY=yMap.transform(close(i));

        if(gain->at(i)){
            painter->setPen(raisePen);
            painter->setBrush(raiseBrush);
        }else{
            painter->setPen(fallPen);
            painter->setBrush(fallBrush);
        }

        painter->drawLine(QPoint(x,highY),QPoint(x,qMin(openY,closeY)));
        painter->drawLine(QPoint(x,lowY),QPoint(x,qMax(openY,closeY)));
        painter->drawRect(QRect(QPoint(x-(bodyWidth-1)/2,qMin(openY,closeY)),QSize(bodyWidth-1,qAbs(openY-closeY))));
    }

    painter->restore();
}

double CandleStick::regionalMaxValue(int minIndex, int maxIndex) const
{
    if(high.size()==0) return -1;
    minIndex=qMax(0,minIndex);
    maxIndex=qMin(high.size(),maxIndex);
    double result=high(minIndex);
    for(int i=minIndex;i<maxIndex;i++){
        result=qMax(result,high(i));
    }
    return result;
}

double CandleStick::regionalMinValue(int minIndex, int maxIndex) const
{
    if(low.size()==0) return -1;
    minIndex=qMax(0,minIndex);
    maxIndex=qMin(low.size(),maxIndex);
    double result=high(minIndex);
    for(int i=minIndex;i<maxIndex;i++){
        result=qMin(result,low(i));
    }
    return result;
}

void CandleStick::onDataLoaded()
{
    initData();
}

void CandleStick::onDataUpdated()
{
    gain->onUpdated();
}

void CandleStick::onDataAppended()
{
    gain->onAppended();
}

QString CandleStick::dateString(int index){
    return mHistoryData->at(index).date;
}

QString CandleStick::titleLabelString(int index){
    Q_UNUSED(index)
    return QString("");
}

void CandleStick::initData(){
    gain->onLoaded();
}
