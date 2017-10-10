#include "bar.h"

const QString Bar::mCategory = QString("BAR");

Bar::Bar(AbstractHistoryData *data):
    TechniqueIndicator(QString("BAR")),
    open(OPEN(data)),
    high(HIGH(data)),
    low(LOW(data)),
    close(CLOSE(data)),
    mHistoryData(data)
{
    gain = new ReusableOperationWrapper<bool>(IF<bool>((close == open), (close > REF<double>(close, 1)), (close > open)));
    initData();
}

void Bar::draw(const QVector<int>& itemPosX, TradingCurveWidgets::IndexRange indexRange, int bodyWidth, const QwtScaleMap &yMap, QPainter *painter){
    for(int i=0; i < indexRange.offset; i++){
        drawItem(itemPosX.at(i), bodyWidth, i + indexRange.minIndex, yMap, painter);
    }
}

void Bar::drawItem(int itemPosX, int bodyWidth, int itemIndex, const QwtScaleMap& yMap, QPainter* painter){
    double highY=yMap.transform(high(itemIndex));
    double lowY=yMap.transform(low(itemIndex));
    double openY=yMap.transform(open(itemIndex));
    double closeY=yMap.transform(close(itemIndex));
    painter->save();

    QPen pen;
    if(gain->at(itemIndex)){
        pen.setColor(Qt::red);
        pen.setWidth(1);
        painter->setPen(pen);
    }else{
        if(getTheme() == Dark)
            pen.setColor(Qt::white);
        else
            pen.setColor(Qt::darkGreen);

        pen.setWidth(1);
        painter->setPen(pen);
    }

    painter->drawLine(QPoint(itemPosX,highY),QPoint(itemPosX,lowY));
    painter->drawLine(QPoint(itemPosX,openY),QPoint(itemPosX-(bodyWidth-1)/2,openY));
    painter->drawLine(QPoint(itemPosX,closeY),QPoint(itemPosX+(bodyWidth-1)/2,closeY));

    painter->restore();
}

double Bar::regionalMaxValue(int minIndex, int maxIndex) const
{
    if(size()==0) return -1;
    minIndex = qMax(0, minIndex);
    maxIndex = qMin(size(), maxIndex);
    double result = high(minIndex);
    for(int i = minIndex; i<maxIndex; i++){
        result = qMax(result, high(i));
    }
    return result;
}

double Bar::regionalMinValue(int minIndex, int maxIndex) const{
    if(size()==0) return -1;
    minIndex = qMax(0, minIndex);
    maxIndex = qMin(size(), maxIndex);
    double result = low(minIndex);
    for(int i = minIndex; i<maxIndex; i++){
        result = qMin(result, low(minIndex));
    }
    return result;
}

void Bar::onDataLoaded()
{
    initData();
}

void Bar::onDataUpdated()
{
    gain->onUpdated();
}

void Bar::onDataAppended()
{
    gain->onAppended();
}

QString Bar::dateString(int index){
    return mHistoryData->at(index).date;
}

QString Bar::titleLabelString(int index){
    Q_UNUSED(index)
    return QString("");
}

void Bar::initData(){
    gain->onLoaded();
}
