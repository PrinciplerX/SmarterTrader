#ifndef INTEGERAXISDRAW_H
#define INTEGERAXISDRAW_H

#include "tradingcurvewidgets_global.h"
#include "rectangularaxisdraw.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT IntegerAxisDraw : public RectangularAxisDraw
{
public:
    IntegerAxisDraw();
    virtual QwtText label( double value ) const override;
};

#endif // INTEGERAXISDRAW_H
