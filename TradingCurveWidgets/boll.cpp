#include "boll.h"
#include "plotablecurvefactory.h"

const QString BOLL::mCategory = QString("BOLL");

BOLL::BOLL(AbstractHistoryData *data, int period, double times):
    TechniqueIndicator(QString("BOLL(%1,%2)").arg(QString::number(period), QString::number(times, 'f', 1))),
    mTimes(times),
    std(ReusableOperationWrapper<double>(new STD(CLOSE(data), period))),
    bar(CandleStick(data))
{
    // 输入
    CLOSE close(data);

    // 计算式
    ReusableOperationWrapper<double> fMid  (MA(close, period));
    ReusableOperationWrapper<double> fUpper(fMid + (mTimes * std));
    ReusableOperationWrapper<double> fLower(fMid - (mTimes * std));

    // 曲线
    mid   = new PlotableCurve(fMid,    "MID");
    upper = new PlotableCurve(fUpper,  "UPPER");
    lower = new PlotableCurve(fLower,  "LOWER");

    setName(indicatorName());
    createTitleCache();
}

void BOLL::draw(const QVector<int>& itemPosX, TradingCurveWidgets::IndexRange indexRange, int bodyWidth, const QwtScaleMap &yMap, QPainter *painter)
{
    bar.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    mid->draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    upper->draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    lower->draw(itemPosX, indexRange, bodyWidth, yMap, painter);
}

double BOLL::regionalMaxValue(int minIndex, int maxIndex) const
{
    if(bar.size() == 0){
        return upper->regionalMaxValue(minIndex, maxIndex);
    }
    return qMax(bar.regionalMaxValue(minIndex, maxIndex),
                upper->regionalMaxValue(minIndex, maxIndex));
}

double BOLL::regionalMinValue(int minIndex, int maxIndex) const
{
    if(bar.size() == 0){
        return upper->regionalMinValue(minIndex, maxIndex);
    }
    return qMin(bar.regionalMinValue(minIndex, maxIndex),
                lower->regionalMinValue(minIndex, maxIndex));
}

void BOLL::onDataLoaded()
{
    mid->operation()->onLoaded();
    upper->operation()->onLoaded();
    lower->operation()->onLoaded();
    bar.onDataLoaded();
}

void BOLL::onDataUpdated()
{

}

void BOLL::onDataAppended()
{

}

int BOLL::size() const {return mid->size();}

QString BOLL::dateString(int index){
    Q_UNUSED(index)
    return QString("not stored");
}

QString BOLL::titleLabelString(int index)
{
    if(titleCache.size() == size())
        return titleCache[index];

    QString color = mid->getPen().color().name(QColor::HexRgb);
    QString former = QString("<font color=\"") + color + QString("\">") + name() + QString("</font>");
    return former + " " + mid->titleLabelString(index) +
                    " " + upper->titleLabelString(index) +
                    " " + lower->titleLabelString(index);
}

void BOLL::createTitleCache()
{
    titleCache.resize(size());

    QString color = mid->getPen().color().name(QColor::HexRgb);
    QString former = QString("<font color=\"") + color + QString("\">") + name() + QString("</font>");
    for(int i = 0; i<titleCache.size(); i++){
        titleCache[i] = former + " " + mid->titleLabelString(i) +
                                 " " + upper->titleLabelString(i) +
                                 " " + lower->titleLabelString(i);
    }
}
