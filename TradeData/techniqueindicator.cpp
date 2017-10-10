#include "techniqueindicator.h"
#include "tradedata.h"

/*!
 * \brief 由于不能在构造函数中调用虚函数，所以这里使用了无参数的构造函数。当构造完成后，可以
 * 通过调用setHistoryData()来手动链接数据。
 */
TechniqueIndicator::TechniqueIndicator(const QString &name)
{
    setIndicatorName(name);
}

QString TechniqueIndicator::indicatorName() const
{
    return mIndicatorName;
}

AbstractHistoryData *TechniqueIndicator::historyData()
{
    return mHistoryData;
}

void TechniqueIndicator::setIndicatorName(const QString &name)
{
    mIndicatorName = name;
}
