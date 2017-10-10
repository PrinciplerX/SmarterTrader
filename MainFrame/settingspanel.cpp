#include "settingspanel.h"
#include <QHBoxLayout>
#include <QAction>
#include <QMessageBox>

SettingsPanel::SettingsPanel(QWidget *parent):
    QWidget(parent),
    settingsListToolBox(new QToolBox()),
    pageStackedWidget(new QStackedWidget()),
    aboutSettingsList(new QListWidget()),
    aboutWidget(new AboutWidget()),
    mUiConfigWidget(new UiConfigWidget())
{
    //QAction* aboutAppAction = new QAction(tr("版本信息"), settingsListToolBox);
    //QAction* featureAction = new QAction(tr("版本特性"), settingsListToolBox);
    //QAction* checkForUpdateAction = new QAction(tr("检查更新"), settingsListToolBox);

    QListWidgetItem* aboutAppItem = new QListWidgetItem(tr("版本信息"), aboutSettingsList);
    QListWidgetItem* aboutQtItem = new QListWidgetItem(tr("关于Qt"), aboutSettingsList);

    connect(aboutSettingsList, SIGNAL(currentRowChanged(int)), this, SLOT(aboutPageChanged(int)));

    settingsListToolBox->addItem(new QWidget(), tr("界面"));
    settingsListToolBox->addItem(new QWidget(), tr("网络"));
    settingsListToolBox->addItem(new QWidget(), tr("数据"));
    settingsListToolBox->addItem(new QWidget(), tr("其他"));
    settingsListToolBox->addItem(aboutSettingsList, tr("关于"));

    settingsListToolBox->setFixedWidth(150);
    pageStackedWidget->setFrameShape(QFrame::StyledPanel);
    pageStackedWidget->addWidget(mUiConfigWidget);
    pageStackedWidget->addWidget(aboutWidget);
    connect(settingsListToolBox, SIGNAL(currentChanged(int)), this, SLOT(settingsPageChanged(int)));

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(settingsListToolBox);
    mainLayout->addWidget(pageStackedWidget);
}

void SettingsPanel::settingsPageChanged(int index)
{
    switch(index){
    case 0:
        pageStackedWidget->setCurrentWidget(mUiConfigWidget);
        break;
    case 4:
        pageStackedWidget->setCurrentWidget(aboutWidget);
        break;
    }
}

void SettingsPanel::aboutPageChanged(int index)
{
    switch(index){
    case 0:
        pageStackedWidget->setCurrentWidget(aboutWidget);
        break;
    case 1:
        QMessageBox::aboutQt(this);
        break;
    }
}
