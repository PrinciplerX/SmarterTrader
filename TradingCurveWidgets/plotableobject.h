#ifndef PLOTABLEOBJECT_H
#define PLOTABLEOBJECT_H

#include <QObject>
#include "tradingcurvewidgets_global.h"
#include "plotableitem.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT PlotableObject : public QObject
{
    Q_OBJECT
public:
    explicit PlotableObject(PlotableItem* item, QObject *parent = 0);

signals:

public slots:
};

#endif // PLOTABLEOBJECT_H
