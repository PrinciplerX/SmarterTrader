#ifndef OPERATIONINDICATOR_H
#define OPERATIONINDICATOR_H

#include "tradingcurvewidgets_global.h"
#include "techniqueindicator.h"
#include "operation.h"

/*!
 * \brief ���ڷ�װ����Operation<double>�Ĳ�������װ��Ĳ���������Ϊ����ָ��ʹ�ã����ӵ�AbstractHistoryData)��
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
