#include "themesettingspanel.h"
#include <QtWidgets>

ThemeSettingsPanel::ThemeSettingsPanel(QWidget *parent) : QWidget(parent)
{
    QGridLayout* settingsLayout = new QGridLayout();
    settingsLayout->addWidget(new QLabel(""));
}

