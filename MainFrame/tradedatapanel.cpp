#include "tradedatapanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

TradeDataPanel::TradeDataPanel(QWidget *parent):
    QWidget(parent),
    mNavigationList(new QListWidget()),
    mContentWidget(new QStackedWidget()),
    dataWidget(new DataWidget()),
    tickDataWidget(new TickDataWidget())
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(mNavigationList);
    mainLayout->addWidget(mContentWidget);

    mNavigationList->addItem(QString("历史行情"));
    mNavigationList->addItem(QString("复权数据"));
    mNavigationList->addItem(QString("历史分笔"));
    mNavigationList->addItem(QString("实时分笔"));
    mNavigationList->addItem(QString("大单数据"));
    mNavigationList->addItem(QString("每周快照"));
    mNavigationList->setFixedWidth(120);

    mContentWidget->addWidget(dataWidget);
    mContentWidget->addWidget(new QWidget());
    mContentWidget->addWidget(tickDataWidget);
    mContentWidget->addWidget(new QWidget());
    mContentWidget->addWidget(new QWidget());
    mContentWidget->addWidget(new QWidget());

    connect(mNavigationList, SIGNAL(currentRowChanged(int)), this, SLOT(onNavigationListActivated(int)));
}

void TradeDataPanel::onNavigationListActivated(int index)
{
    mContentWidget->setCurrentIndex(index);
}
