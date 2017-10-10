#ifndef STATISTICPANEL_H
#define STATISTICPANEL_H

#include <QWidget>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLocale>

class StatisticPanel : public QWidget
{
    Q_OBJECT
public:
    explicit StatisticPanel(QWidget *parent = 0);

signals:

public slots:

private:
    QCalendarWidget* calendar;
};

#endif // STATISTICPANEL_H
