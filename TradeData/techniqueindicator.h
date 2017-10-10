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
     * \brief 获取用于标识一个指标的名称。具备相同参数的同一类型指标必须具备相同的指标名
     * 称。当参数更新时，必须同时更新指标名称。
     * \return 指标名称。
     */
    QString indicatorName() const;

    /*!
     * \return 返回一个用于标识指标类别的字符串。
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
