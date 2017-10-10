#include "dockwidget.h"
#include <QVBoxLayout>

DockWidget::DockWidget(const QString &title, QWidget *parent):
    QDockWidget(parent),
    mArea(Qt::NoDockWidgetArea),
    mTitleWidget(new DockWidgetTitle()),
    mState(DockWidgetState::Unknown),
    mLayout(new QVBoxLayout())
{
    setObjectName("DockWidget");
    setWindowTitle(title);
    setTitleBarWidget(mTitleWidget);

    mLayout->setContentsMargins(0, 2, 0, 0);
    QWidget* widget = new QWidget();
    widget->setLayout(mLayout);
    QDockWidget::setWidget(widget);
}

DockWidget::~DockWidget()
{

}
