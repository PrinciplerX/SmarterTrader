#include "globalaverage.h"
#include <cfloat>
#include "tradedataoperations.h"

const QString GlobalAverage::mCategory = QString("GLOBALAVERAGE");

GlobalAverage::GlobalAverage(AbstractHistoryData *data, const QPen &pen):
    TechniqueIndicator(mCategory),
    mHistoryData(data)
{
    setName(QString("GlobalAverage"));

    // 输入
    CLOSE close(data);

    // 计算式
    ReusableOperationWrapper<double> fOutput(SUM<double>(close, 0) / INDEX<double>(close));

    // 曲线
    curve.setInput(fOutput);
    curve.setName(QString("总平均"));
    curve.setPen(pen);
}

void GlobalAverage::draw(const QVector<int>& itemPosX, TradingCurveWidgets::IndexRange indexRange, int bodyWidth, const QwtScaleMap &yMap, QPainter *painter){
    painter->save();
    curve.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    painter->restore();
}

double GlobalAverage::regionalMaxValue(int minIndex, int maxIndex) const {
    return curve.regionalMaxValue(minIndex, maxIndex);
}

double GlobalAverage::regionalMinValue(int minIndex, int maxIndex) const {
    return curve.regionalMinValue(minIndex, maxIndex);
}

void GlobalAverage::onDataLoaded()
{

}

void GlobalAverage::onDataUpdated()
{

}

void GlobalAverage::onDataAppended()
{

}

int GlobalAverage::size() const {
    return curve.size();
}

int GlobalAverage::tracerTarget(int itemIndex, const QwtScaleMap &yMap) const {
    return yMap.transform(curve.at(itemIndex));
}

QString GlobalAverage::dateString(int index){
    return mHistoryData->at(index).date;
}

QString GlobalAverage::titleLabelString(int index){
    return QString("<font color=\"") + curve.getPen().color().name(QColor::HexRgb) + QString("\">") +
           QString("均价:") + QString::number(curve.at(index), 'f', 2) +QString("</font>");
}
