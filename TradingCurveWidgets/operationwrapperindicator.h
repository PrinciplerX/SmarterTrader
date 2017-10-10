#ifndef OPERATIONINDICATOR_H
#define OPERATIONINDICATOR_H

#include "tradingcurvewidgets_global.h"
#include "techniqueindicator.h"
#include "operation.h"

/*!
 * \brief 用于封装基于Operation<double>的操作，封装后的操作可以作为技术指标使用（链接到AbstractHistoryData)。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT OperationWrapperIndicator : public TechniqueIndicator
{
public:
    OperationWrapperIndicator(Operation<double>* input, const QString& category = QString("UNKNOWN"),
                       const QString& name = QString("UNKNOWN"));

    virtual void onDataLoaded() override;
    virtual void onDataUpdated() override;
    virtual void onDataAppended() override;

    virtual QString category() const;

private:
    Operation<double>* mInput;
    QString mCategory;
};

#endif // OPERATIONINDICATOR_H
