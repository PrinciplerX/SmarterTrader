#ifndef VOIDAXISDRAW_H
#define VOIDAXISDRAW_H

#include "tradingcurvewidgets_global.h"
#include "rectangularaxisdraw.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT VoidAxisDraw : public RectangularAxisDraw
{
public:
    VoidAxisDraw();

    virtual QwtText label( double value ) const override;
    virtual void drawTick(QPainter* painter, double val, double len) const override;
    virtual void drawLabel(QPainter* painter, double val) const override;
};

#endif // VOIDAXISDRAW_H
