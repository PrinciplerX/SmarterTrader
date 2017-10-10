#include "kdj.h"
#include "tradedataoperations.h"
#include <climits>
#include <cfloat>

const QString KDJ::mCategory = QString("KDJ");

KDJ::KDJ(AbstractHistoryData *data, int rsvPeriod , int kPeriod, int dPeriod):
    TechniqueIndicator(mCategory),
    rsvPeriod(rsvPeriod),
    kPeriod(kPeriod),
    dPeriod(dPeriod)
{
    setName(QString("KDJ(%1,%2,%3)").arg(QString::number(rsvPeriod),
                                         QString::number(kPeriod),
                                         QString::number(dPeriod)));

    // 输入
    CLOSE close(data);
    HIGH high(data);
    LOW low(data);

    // 计算式
    ReusableOperationWrapper<double> RSV ((close-LLV<double>(low,rsvPeriod)) / (HHV<double>(high,rsvPeriod)-LLV<double>(low,rsvPeriod)) * 100.0);
    ReusableOperationWrapper<double> fK  (BOUND<double>(0.0, SMA(RSV, kPeriod, 1), 100.0));
    ReusableOperationWrapper<double> fD  (BOUND<double>(0.0, SMA(fK, dPeriod, 1),  100.0));
    ReusableOperationWrapper<double> fJ  (BOUND<double>(0.0, (3*fK - 2*fD),        100.0));

    // 曲线
    k.setInput(fK);
    d.setInput(fD);
    j.setInput(fJ);

    k.setName(QString("K"));
    d.setName(QString("D"));
    j.setName(QString("J"));
}

void KDJ::draw(const QVector<int> &itemPosX, TradingCurveWidgets::IndexRange indexRange, int bodyWidth, const QwtScaleMap &yMap, QPainter *painter)
{
    k.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    d.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    j.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
}

double KDJ::regionalMaxValue(int minIndex, int maxIndex) const {
    double result = DBL_MIN;
    result = qMax(result, k.regionalMaxValue(minIndex, maxIndex));
    result = qMax(result, d.regionalMaxValue(minIndex, maxIndex));
    result = qMax(result, j.regionalMaxValue(minIndex, maxIndex));
    return result;
}

double KDJ::regionalMinValue(int minIndex, int maxIndex) const {
    double result = DBL_MAX;
    result = qMin(result, k.regionalMinValue(minIndex, maxIndex));
    result = qMin(result, d.regionalMinValue(minIndex, maxIndex));
    result = qMin(result, j.regionalMinValue(minIndex, maxIndex));
    return result;
}

void KDJ::onDataLoaded()
{
    k.operation()->onLoaded();
    d.operation()->onLoaded();
    j.operation()->onLoaded();
}

void KDJ::onDataUpdated()
{
    k.operation()->onUpdated();
    d.operation()->onUpdated();
    j.operation()->onUpdated();
}

void KDJ::onDataAppended()
{
    k.operation()->onAppended();
    d.operation()->onAppended();
    j.operation()->onAppended();
}

int KDJ::size() const {
    return j.size();
}

QString KDJ::dateString(int index){
    Q_UNUSED(index)
    return QString("not storaged");
}

QString KDJ::titleLabelString(int index){
    if(titleCache.size() == size())
        return titleCache[index];

    QString color = k.getPen().color().name(QColor::HexRgb);
    QString former = QString("<font color=\"%1\">%2</font>").arg(color, name());

    return former + " " + k.titleLabelString(index) +
                    " " + d.titleLabelString(index) +
                    " " + j.titleLabelString(index);
}

void KDJ::createTitleCache(){
    titleCache.resize(size());

    QString color = k.getPen().color().name(QColor::HexRgb);
    QString former = QString("<font color=\"%1\">%2</font>").arg(color, name());

    for(int i = 0; i<titleCache.size(); i++){
        titleCache[i] = former + " " + k.titleLabelString(i) +
                                 " " + d.titleLabelString(i) +
                                 " " + j.titleLabelString(i);
    }
}
