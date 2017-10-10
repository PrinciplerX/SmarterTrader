#include "integeraxisdraw.h"

IntegerAxisDraw::IntegerAxisDraw()
{

}

QwtText IntegerAxisDraw::label(double value) const
{
    return QString::number((int)value);
}
