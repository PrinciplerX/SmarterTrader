#include "macd.h"
#include <cfloat>
#include "tradedataoperations.h"
#include <QtMath>

const QString MACD::mCategory = QString("MACD");

MACD::MACD(AbstractHistoryData* data, int macdShortPeriod, int macdLongPeriod, int macddiffPeriod):
    TechniqueIndicator(mCategory),
    shortPeriod(macdShortPeriod),
    longPeriod(macdLongPeriod),
    diffPeriod(macddiffPeriod),
    mHistoryData(data),
    fDiff(ReusableOperationWrapper<double>(EMA(CLOSE(data), macdShortPeriod) - EMA(CLOSE(data), macdLongPeriod))),
    fDea(ReusableOperationWrapper<double>(EMA(fDiff, macddiffPeriod))),
    fMacd(ReusableOperationWrapper<double>(2 * (fDiff - fDea)))
{
    diff = new PlotableCurve(fDiff, "DIFF");
    dea  = new PlotableCurve(fDea,  "DEA");

    setName(QString("MACD(%1,%2,%3)").arg(QString::number(shortPeriod),
                                          QString::number(longPeriod),
                                          QString::number(diffPeriod)));
}

void MACD::draw(const QVector<int>& itemPosX, TradingCurveWidgets::IndexRange indexRange, int bodyWidth, const QwtScaleMap &yMap, QPainter *painter){
    Q_UNUSED(bodyWidth)
    painter->save();

    //绘制柱状线
    double baseLine=yMap.transform(0.0);
    int maxIndex = indexRange.maxIndex();

    for(int i = indexRange.minIndex; i <= maxIndex; i++){
        double macdY = yMap.transform(fMacd.at(i));
        QPen pen;
        if(getTheme() == Dark)
            pen.setColor(fMacd.at(i)<0.0 ? Qt::green : Qt::red );
        else
            pen.setColor(fMacd.at(i)<0.0 ? Qt::darkGreen : Qt::red );
        painter->setPen(pen);
        painter->drawLine(QPoint(itemPosX.at(i-indexRange.minIndex),baseLine),
                          QPoint(itemPosX.at(i-indexRange.minIndex),macdY));
    }

    //绘制diff和dea
    diff->draw(itemPosX,indexRange,bodyWidth,yMap,painter);
    dea->draw(itemPosX,indexRange,bodyWidth,yMap,painter);

    painter->restore();
}

double MACD::regionalMaxValue(int minIndex, int maxIndex) const {
    double result = DBL_MIN;
    result = qMax(result, fMacd.regionalMaxValue(minIndex, maxIndex));
    result = qMax(result, fDiff.regionalMaxValue(minIndex, maxIndex));
    result = qMax(result, fDea.regionalMaxValue(minIndex, maxIndex));
    return result;
}

double MACD::regionalMinValue(int minIndex, int maxIndex) const
{
    double result = DBL_MAX;
    result = qMin(result, fMacd.regionalMinValue(minIndex, maxIndex));
    result = qMin(result, fDiff.regionalMinValue(minIndex, maxIndex));
    result = qMin(result, fDea.regionalMinValue(minIndex, maxIndex));
    return result;
}

void MACD::onDataLoaded()
{
    fDiff.onLoaded();
    fDea.onLoaded();
    fMacd.onLoaded();

    createTitleCache();
}

void MACD::onDataUpdated()
{
    fDiff.onUpdated();
    fDea.onUpdated();
    fMacd.onUpdated();
}

void MACD::onDataAppended()
{
    fDiff.onAppended();
    fDea.onAppended();
    fMacd.onAppended();
}

QString MACD::dateString(int index){
    return mHistoryData->at(index).date;
}

QString MACD::titleLabelString(int index){
    return titleCache[index];
}

void MACD::createTitleCache(){
    titleCache.resize(size());
    if(titleCache.isEmpty()) return;

    QString former = QString("<font color= \"black\">") + name() + QString("</font>") +
                     QString("<font color= \"");

    QString red = QColor(Qt::red).name();
    QString green = QColor(Qt::green).name();

    QString redFormer = former + red + QString("\"> MACD:");
    QString greenFormer = former + green + QString("\"> MACD:");

    QString latter = QString("</font>");

    for(int i = 0; i < titleCache.size(); i++){
        titleCache[i] = (fMacd.at(i) > 0.0 ? redFormer : greenFormer) +
                        QString::number(fMacd.at(i),'f',3) +
                        latter + ' ' +
                        diff->titleLabelString(i) + ' ' +
                        dea->titleLabelString(i);
    }
}
