#include "dmi.h"
#include "tradedataoperations.h"

const QString DMI::mCategory = QString("DMI");

DMI::DMI(AbstractHistoryData *data, int sumPeriod, int adxPeriod):
    TechniqueIndicator(mCategory),
    mHistoryData(data)
{
    this->sumPeriod = sumPeriod;
    this->adxPeriod = adxPeriod;
    setName(QString("DMI(%1,%2)").arg(QString::number(sumPeriod), QString::number(adxPeriod)));

    // 输入
    HIGH high(data);
    LOW low(data);

    // 计算式
    ReusableOperationWrapper<double> tr  (SUM<double>(TR(data), sumPeriod));
    ReusableOperationWrapper<double> HD  (high - REF<double>(high,1));
    ReusableOperationWrapper<double> LD  (REF<double>(low,1) - low);
    ReusableOperationWrapper<double> DMP (SUM<double>(IF<double>(HD>0.0 && HD>LD,HD,0.0), sumPeriod));
    ReusableOperationWrapper<double> DMM (SUM<double>(IF<double>(LD>0.0 && LD>HD,LD,0.0), sumPeriod));
    ReusableOperationWrapper<double> DI1 (DMP*100/tr);
    ReusableOperationWrapper<double> DI2 (DMM*100/tr);
    //可能出现以0为除数得到nan情况，所以将预计算的数据限制在0.0~100.0范围内，除掉值为nan的数据
    ReusableOperationWrapper<double> ADX (MA(BOUND<double>(0.0, ABS<double>(DI2-DI1)/(DI1+DI2)*100, 100.0), adxPeriod));
    ReusableOperationWrapper<double> ADXR((ADX+REF<double>(ADX,adxPeriod))/2);

    // 曲线
    pDI.setInput(DI1);
    nDI.setInput(DI2);
    adx.setInput(ADX);
    adxr.setInput(ADXR);

    pDI.setName(QString("+DI"));
    nDI.setName(QString("-DI"));
    adx.setName(QString("ADX"));
    adxr.setName(QString("ADXR"));
}

void DMI::draw(const QVector<int> &itemPosX, TradingCurveWidgets::IndexRange indexRange, int bodyWidth, const QwtScaleMap &yMap, QPainter *painter)
{
    pDI.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    nDI.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    adx.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
    adxr.draw(itemPosX, indexRange, bodyWidth, yMap, painter);
}

double DMI::regionalMaxValue(int minIndex, int maxIndex) const
{
    double result = pDI.regionalMaxValue(minIndex, maxIndex);
    result = qMax(result, nDI.regionalMaxValue(minIndex, maxIndex));
    result = qMax(result, adx.regionalMaxValue(minIndex, maxIndex));
    result = qMax(result, adxr.regionalMaxValue(minIndex, maxIndex));
    return result;
}

double DMI::regionalMinValue(int minIndex, int maxIndex) const
{
    double result = pDI.regionalMinValue(minIndex, maxIndex);
    result = qMin(result, nDI.regionalMinValue(minIndex, maxIndex));
    result = qMin(result, adx.regionalMinValue(minIndex, maxIndex));
    result = qMin(result, adxr.regionalMinValue(minIndex, maxIndex));
    return result;
}

void DMI::onDataLoaded()
{
    pDI.operation()->onLoaded();
    nDI.operation()->onLoaded();
    adx.operation()->onLoaded();
    adxr.operation()->onLoaded();
}

void DMI::onDataUpdated()
{
    pDI.operation()->onUpdated();
    nDI.operation()->onUpdated();
    adx.operation()->onUpdated();
    adxr.operation()->onUpdated();
}

void DMI::onDataAppended()
{
    pDI.operation()->onAppended();
    nDI.operation()->onAppended();
    adx.operation()->onAppended();
    adxr.operation()->onAppended();
}

int DMI::size() const {
    return pDI.size();
}

QString DMI::dateString(int index){
    Q_UNUSED(index);
    return QString("not storaged");
}

QString DMI::titleLabelString(int index){
    if(titleCache.size() == size())
        return titleCache[index];

    QString color = adx.getPen().color().name(QColor::HexRgb);
    QString former = QString("<font color=\"%1\">%2</font>").arg(color, name());

    return QString("%1 %2 %3 %4 %5").arg(former,
                                         pDI.titleLabelString(index),
                                         nDI.titleLabelString(index),
                                         adx.titleLabelString(index),
                                         adxr.titleLabelString(index));
}

void DMI::createTitleCache()
{
    titleCache.resize(size());
    QString color = adx.getPen().color().name(QColor::HexRgb);
    QString former = QString("<font color=\"%1\">%2</font>").arg(color, name());

    for(int i = 0; i<size(); i++){
        titleCache[i] = QString("%1 %2 %3 %4 %5").arg(former,
                                                      pDI.titleLabelString(i),
                                                      nDI.titleLabelString(i),
                                                      adx.titleLabelString(i),
                                                      adxr.titleLabelString(i));
    }
}

