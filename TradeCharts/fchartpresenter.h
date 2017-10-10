#ifndef FFChartPresenter_H
#define FFChartPresenter_H

#include <SChartGlobal>
#include <ChartPresenter>
#include <glwidget_p.h>
#include <QtCore/QRectF>
#include <QtCore/QMargins>
#include <QtCore/QLocale>
#include <QtCore/QPointer>
#include <QtCore/QEasingCurve>

#include "tradecharts_global.h"

SCHARTS_BEGIN_NAMESPACE

class FChart;

class TRADECHARTS_EXPORT FChartPresenter: public ChartPresenter
{
    Q_OBJECT
public:
    FChartPresenter(FChart *chart);

    QVector<ChartPresenter*> presenters();

public slots:
    void handleChartAdded(QChart* chart);
    void handleChartRemoved(int index);
    void handleChartInserted(int index, QChart* chart);

private:
    QVector<ChartPresenter*> mPresenters;
};

SCHARTS_END_NAMESPACE

#endif /* FChartPresenter_H */
