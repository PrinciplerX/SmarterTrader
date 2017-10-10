#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QToolBox>
#include <QStackedWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include "aboutwidget.h"
#include "uiconfigwidget.h"
#include "mainframe_global.h"

class SettingsPanel : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPanel(QWidget *parent = 0);

signals:

public slots:
    void settingsPageChanged(int index);
    void aboutPageChanged(int index);

private:
    QToolBox* settingsListToolBox;
    QStackedWidget* pageStackedWidget;
    QListWidget* aboutSettingsList;

    AboutWidget* aboutWidget;
    UiConfigWidget* mUiConfigWidget;
};

#endif // SETTINGSWIDGET_H
