#ifndef TECHNIQUEINDICATOR_H
#define TECHNIQUEINDICATOR_H

#include "tradedata.h"
#include "tradedata_global.h"

/*!
 * \brief The TechniqueIndicator class
 */
class TRADEDATASHARED_EXPORT TechniqueIndicator
{
public:
    TechniqueIndicator(const QString& name = QString("UNKNOWN"));

    virtual void onDataLoaded() = 0;
    virtual void onDataUpdated() = 0;
    virtual void onDataAppended() = 0;

    /*!
     * \brief ��ȡ���ڱ�ʶһ��ָ������ơ��߱���ͬ������ͬһ����ָ�����߱���ͬ��ָ����
     * �ơ�����������ʱ������ͬʱ����ָ�����ơ�
     * \return ָ�����ơ�
     */
    QString indicatorName() const;

    /*!
     * \return ����һ�����ڱ�ʶָ�������ַ�����
     */
    virtual QString category() const = 0;

    AbstractHistoryData* historyData();

protected:
    void setIndicatorName(const QString& name);

private:
    AbstractHistoryData* mHistoryData;
    QString mIndicatorName;
};

#endif // TECHNIQUEINDICATOR_H
