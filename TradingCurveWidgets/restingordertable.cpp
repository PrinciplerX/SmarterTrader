#include "restingordertable.h"
#include <QHeaderView>
#include <QDebug>

RestingOrderTable::RestingOrderTable(QWidget *parent):
    QWidget(parent),
    orderTable(new QTableWidget())
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* labelLayout = new QVBoxLayout();
    QFrame* labelFrame = new QFrame();
    labelFrame->setFrameShape(QFrame::StyledPanel);

    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    labelLayout->setMargin(0);
    labelLayout->setSpacing(0);

    QLabel* bidLabel = new QLabel(tr("买\n\n盘"));
    bidLabel->setStyleSheet("color: white; background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #FF3030, stop: 1 #FF3030);");
    QLabel* askLabel = new QLabel(tr("卖\n\n盘"));
    askLabel->setStyleSheet("background-color: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, stop: 0 #00A800, stop: 1 #00A800);");
    labelLayout->addWidget(askLabel);
    labelLayout->addWidget(bidLabel);
    labelFrame->setLayout(labelLayout);

    mainLayout->addWidget(labelFrame);
    mainLayout->addWidget(orderTable);

    bidLabel->setAlignment(Qt::AlignVCenter);
    askLabel->setAlignment(Qt::AlignVCenter);

    orderTable->setRowCount(10);
    orderTable->setColumnCount(2);

    orderTable->setAutoScroll(false);
    orderTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    orderTable->verticalHeader()->setDefaultSectionSize(19);
    orderTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    orderTable->setFixedHeight(orderTable->sizeHint().height());

    orderTable->horizontalHeader()->hide();
    orderTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    orderTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFont font = orderTable->font();
    font.setPointSize(9);
    orderTable->setFont(font);

    orderTable->setShowGrid(false);

    int n = 5;

    QStringList verticalHeaders;
    //ask order
    for(int i = 0; i < n; i++){
        verticalHeaders.append(QString::number(n - i));
    }
    //bid order
    for(int i = 0; i < n; i++){
        verticalHeaders.append(QString::number(i + 1));
    }
    orderTable->setVerticalHeaderLabels(verticalHeaders);

    for(int i = 0; i < n; i++){
        //价格
        QTableWidgetItem* pItem = new QTableWidgetItem("--");
        pItem->setBackground(QColor("#00A800"));
        orderTable->setItem(i, 0, pItem);

        //挂单数
        QTableWidgetItem* nItem = new QTableWidgetItem("--");
        nItem->setBackgroundColor(QColor("#00A800"));
        orderTable->setItem(i, 1, nItem);
    }

    for(int i = n; i < n*2; i++){
        //价格
        QTableWidgetItem* pItem = new QTableWidgetItem("--");
        pItem->setBackground(QColor("#FF3030"));
        pItem->setTextColor(Qt::white);
        orderTable->setItem(i, 0, pItem);

        //挂单数
        QTableWidgetItem* nItem = new QTableWidgetItem("--");
        nItem->setBackgroundColor(QColor("#FF3030"));
        nItem->setTextColor(Qt::white);
        orderTable->setItem(i, 1, nItem);
    }

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

void RestingOrderTable::updateData(const QuoteData *data)
{
    orderTable->item(0, 0)->setText(data->bid1.price);
    orderTable->item(0, 1)->setText(data->bid1.volume);
    orderTable->item(1, 0)->setText(data->bid2.price);
    orderTable->item(1, 1)->setText(data->bid2.volume);
    orderTable->item(2, 0)->setText(data->bid3.price);
    orderTable->item(2, 1)->setText(data->bid3.volume);
    orderTable->item(3, 0)->setText(data->bid4.price);
    orderTable->item(3, 1)->setText(data->bid4.volume);
    orderTable->item(4, 0)->setText(data->bid5.price);
    orderTable->item(4, 1)->setText(data->bid5.volume);

    orderTable->item(5, 0)->setText(data->ask1.price);
    orderTable->item(5, 1)->setText(data->ask1.volume);
    orderTable->item(6, 0)->setText(data->ask2.price);
    orderTable->item(6, 1)->setText(data->ask2.volume);
    orderTable->item(7, 0)->setText(data->ask3.price);
    orderTable->item(7, 1)->setText(data->ask3.volume);
    orderTable->item(8, 0)->setText(data->ask4.price);
    orderTable->item(8, 1)->setText(data->ask4.volume);
    orderTable->item(9, 0)->setText(data->ask5.price);
    orderTable->item(9, 1)->setText(data->ask5.volume);
}

