#include "techniqueindicator.h"
#include "tradedata.h"

/*!
 * \brief ���ڲ����ڹ��캯���е����麯������������ʹ�����޲����Ĺ��캯������������ɺ󣬿���
 * ͨ������setHistoryData()���ֶ��������ݡ�
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
