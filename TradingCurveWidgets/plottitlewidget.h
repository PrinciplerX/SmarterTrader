#ifndef PLOTTITLEWIDGET_H
#define PLOTTITLEWIDGET_H

#include "tradingcurvewidgets_global.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

namespace TradingCurveWidgets{

class TRADINGCURVEWIDGETSSHARED_EXPORT PlotTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotTitleWidget(QWidget *parent = 0);
    explicit PlotTitleWidget(const QString& text, QWidget *parent = 0);

    void addWidget(QWidget* widget);

signals:

public slots:
    void setText(const QString& text);

private:
    QLabel* textLabel;
    QHBoxLayout* mainLayout;
};

}

#endif // PLOTTITLEWIDGET_H
