#include "statisticpanel.h"
#include <QFrame>

StatisticPanel::StatisticPanel(QWidget *parent):
    QWidget(parent),
    calendar(new QCalendarWidget())
{
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* calendarLayout = new QVBoxLayout();
    calendarLayout->setSpacing(1);
    calendarLayout->setMargin(0);

    QFrame* calendarFrame = new QFrame();
    calendarFrame->setFrameShape(QFrame::StyledPanel);
    calendarFrame->setLayout(calendarLayout);
    calendarFrame->setFixedWidth(300);

    calendar->setLocale(QLocale::Chinese);
    calendar->setFirstDayOfWeek(Qt::Monday);
    QFont f = calendar->font();
    f.setPointSize(10);
    calendar->setFont(f);
    calendarLayout->addWidget(calendar);
    calendar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    calendarLayout->addWidget(new QWidget());

    mainLayout->addWidget(calendarFrame);
    mainLayout->addWidget(new QWidget());
    mainLayout->setMargin(0);
    mainLayout->setSpacing(1);
    this->setLayout(mainLayout);
}
