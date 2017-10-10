#include "valueaxis.h"

ValueAxis::ValueAxis(QWidget *parent) : AbstractTraceAxis(parent)
{

}

void ValueAxis::setRange(double min, double max)
{
    bool changed = false;

    if (min > max)
        return;

    if (!isValidValue(min, max)) {
        qWarning() << "Attempting to set invalid range for value axis: ["
                   << min << " - " << max << "]";
        return;
    }

    bool changeMin = false;
    if (minValue == 0 || min == 0)
        changeMin = !qFuzzyCompare(1 + minValue, 1 + min);
    else
        changeMin = !qFuzzyCompare(minValue, min);

    bool changeMax = false;
    if (maxValue == 0 || max == 0)
        changeMax = !qFuzzyCompare(1 + maxValue, 1 + max);
    else
        changeMax = !qFuzzyCompare(maxValue, max);

    if (changeMin) {
        minValue = min;
        changed = true;
        emit minChanged(min);
    }

    if (changeMax) {
        maxValue = max;
        changed = true;
        emit maxChanged(max);
    }

    if (changed) {
        emit rangeChanged(min,max);
    }
}

bool ValueAxis::isValidValue(double min, double max)
{
    return isValidValue(min) && isValidValue(max);
}

bool ValueAxis::isValidValue(double value)
{
    return !isinf(value) && !isnan(value);
}

void ValueAxis::setMax(double max) {
    setRange(qMin(minValue, max), max);
}

void ValueAxis::setMin(double min) {
    setRange(min, qMax(maxValue, min));
}
