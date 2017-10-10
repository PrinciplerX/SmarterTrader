#ifndef ITEMSCALEDRAW_H
#define ITEMSCALEDRAW_H

#include "rectangularaxisdraw.h"
#include "tradingcurvewidgets_global.h"

namespace TradingCurveWidgets {

class AbstractItemAxis;

class TRADINGCURVEWIDGETSSHARED_EXPORT ItemScaleDraw : public RectangularAxisDraw
{
public:
    ItemScaleDraw(AbstractItemAxis* axis);

    virtual QwtText label(double value) const override;

    virtual void draw(QPainter* painter, const QPalette &palette) const override;

    virtual void drawLabels(QPainter* painter, const QVector<int>& indexPos,
                            const QVector<QString>& indexLabel, const QPalette &palette) const;
    virtual void drawTicks(QPainter* painter, const QVector<int>& indexPos, double len,
                           const QPalette &palette) const;

    virtual int maxLabelHeight( const QFont & ) const override;
    virtual int maxLabelWidth( const QFont & ) const override;

    QPointF itemLabelPosition(int pos) const;

protected:

private:
    AbstractItemAxis* itemAxis;
};

}

#endif // ITEMSCALEDRAW_H
