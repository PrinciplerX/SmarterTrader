#ifndef TESTPANEL_H
#define TESTPANEL_H

#include <QWidget>
#include <roundprogressbar.h>
#include <maskwidget.h>

class TestPanel : public QWidget
{
    Q_OBJECT
public:
    TestPanel(QWidget* parent = nullptr);

private:
    MaskWidget* maskWidget;
    RoundProgressBar* roundProgressBar;
};

#endif // TESTPANEL_H
