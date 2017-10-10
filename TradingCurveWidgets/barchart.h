#ifndef BARCHART_H
#define BARCHART_H

#include "tradingcurvewidgets_global.h"
#include <QList>
#include "plotableitem.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT BarSet : public QObject
{
    Q_OBJECT
public:
    BarSet(const QString& name, QObject* parent = nullptr);
    BarSet& operator<<(double value);
private:
    QString name;
    QList<double> valueList;
};

class TRADINGCURVEWIDGETSSHARED_EXPORT BarChart : public PlotableItem
{
public:
    BarChart();

    void append(BarSet* item);

    virtual void draw(const QVector<int>& itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int bodyWidth,
                      const QwtScaleMap& yMap,
                      QPainter* painter) = 0;

    virtual double regionalMaxValue(int minIndex,int maxIndex) const override;
    virtual double regionalMinValue(int minIndex, int maxIndex) const override;

    virtual int size() const { return barsetVector.size(); }

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const override;

    virtual QString dateString(int index) override;

    virtual QString titleLabelString(int index) override;

private:
    QVector<BarSet*> barsetVector;
};

#endif // BARCHART_H
