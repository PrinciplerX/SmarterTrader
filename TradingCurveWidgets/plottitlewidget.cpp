#include "plottitlewidget.h"
#include <QComboBox>
#include <QPushButton>

namespace TradingCurveWidgets{

PlotTitleWidget::PlotTitleWidget(QWidget *parent):
    QWidget(parent),
    textLabel(new QLabel()),
    mainLayout(new QHBoxLayout())
{
    // 该控件默认包含一个QLabel，用于显示标题文字。
    mainLayout->addWidget(textLabel);
    mainLayout->addStretch();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    /*
    QPushButton* button = new QPushButton(QString("指标说明"));
    button->setContentsMargins(0,0,0,0);

    QComboBox* box = new QComboBox();
    box->addItem(QString("MACD"));
    box->addItem(QString("KDJ"));
    box->addItem(QString("DMI"));
    box->addItem(QString("BOLL"));
    box->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(button);
    mainLayout->addWidget(box);
    */

    this->setLayout(mainLayout);
}

PlotTitleWidget::PlotTitleWidget(const QString &text, QWidget *parent):
    PlotTitleWidget(parent)
{
    textLabel->setText(text);
}

void PlotTitleWidget::addWidget(QWidget *widget){
    mainLayout->addWidget(widget);
}

void PlotTitleWidget::setText(const QString &text){
    textLabel->setText(text);
}

}
