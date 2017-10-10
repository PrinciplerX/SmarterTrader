#include "ticktable.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QStyledItemDelegate>

TickTable::TickTable(QWidget *parent):
    QWidget(parent),
    tickTable(new QTableView())
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tickTable);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    tickTable->verticalHeader()->hide();
    tickTable->horizontalHeader()->show();

    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::Highlight, QColor("#FF3030"));
    tickTable->setPalette(palette);
    tickTable->verticalHeader()->setDefaultSectionSize(20);
    tickTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tickTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tickTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tickTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFont f = tickTable->font();
    f.setPointSize(10);
    tickTable->setFont(f);

    tickTable->horizontalHeader()->setStyleSheet(QString("QHeaderView {font: 13px;}"));
}

void TickTable::setModel(TickModel *model){
    tickTable->setModel(model);
}
