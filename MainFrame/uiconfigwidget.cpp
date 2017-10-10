#include "uiconfigwidget.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QStyleFactory>
#include <QApplication>

UiConfigWidget::UiConfigWidget(QWidget *parent):
    QWidget(parent),
    mStyleComboBox(new QComboBox())
{
    mStyleComboBox->addItem("NorwegianWood");
    mStyleComboBox->addItems(QStyleFactory::keys());
    connect(mStyleComboBox, SIGNAL(activated(QString)), this, SLOT(changeStyle(QString)));

    QFormLayout* mainLayout = new QFormLayout(this);
    mainLayout->addRow(QString("界面风格"), mStyleComboBox);
}

void UiConfigWidget::changeStyle(const QString &styleName)
{
    if (styleName == "NorwegianWood") {
        QApplication::setStyle(new NorwegianWoodStyle());
    } else {
        QApplication::setStyle(QStyleFactory::create(styleName));
    }
}
