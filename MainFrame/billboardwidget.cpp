#include "billboardwidget.h"
#include <QTreeView>
#include <QHeaderView>

BillboardWidget::BillboardWidget(QWidget *parent) : QTreeView(parent)
{
    //QTreeView* this = new QTreeView();
    this->header()->setMinimumSectionSize(40);
    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->setUniformRowHeights(true);//

    this->setRootIsDecorated(false);
    this->header()->setSortIndicatorShown(true);
    this->setModel(new BillboardModel(BillboardManger::instance().billboardData(QString("2016-09-01"), QString("2016-09-01"))));
}
