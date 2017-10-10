#include "voidaxisdraw.h"

VoidAxisDraw::VoidAxisDraw()
{

}

QwtText VoidAxisDraw::label(double value) const
{
    Q_UNUSED(value);
    return QString("-");
}

void VoidAxisDraw::drawTick(QPainter *painter, double val, double len) const
{
    Q_UNUSED(painter);
    Q_UNUSED(val);
    Q_UNUSED(len);
}

void VoidAxisDraw::drawLabel(QPainter *painter, double val) const
{
    Q_UNUSED(painter);
    Q_UNUSED(val);
}
