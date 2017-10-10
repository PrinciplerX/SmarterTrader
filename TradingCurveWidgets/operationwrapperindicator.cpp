#include "operationwrapperindicator.h"

OperationWrapperIndicator::OperationWrapperIndicator(Operation<double> *input, const QString &category, const QString &name):
    TechniqueIndicator(name),
    mCategory(category),
    mInput(input)
{
}

void OperationWrapperIndicator::onDataLoaded()
{
    mInput->onLoaded();
}

void OperationWrapperIndicator::onDataUpdated()
{
    mInput->onUpdated();
}

void OperationWrapperIndicator::onDataAppended()
{
    mInput->onAppended();
}

QString OperationWrapperIndicator::category() const
{
    return mCategory;
}
