#include "dockerframe.h"

DockerFrame::DockerFrame(QWidget *centerWidget, QWidget *parent):
    QWidget(parent),
    mTopTabBar(new QTabBar(this)),
    mBottomTabBar(new QTabBar(this)),
    mLeftTabBar(new QTabBar(this)),
    mRightTabBar(new QTabBar(this)),
    mCenterWidget(centerWidget)
{
    if (!mCenterWidget) mCenterWidget = new QWidget(this);

    mCenterWidget->setStyleSheet("background: green;");

    mTopTabBar->setShape(QTabBar::RoundedNorth);
    mBottomTabBar->setShape(QTabBar::RoundedSouth);
    mLeftTabBar->setShape(QTabBar::RoundedWest);
    mRightTabBar->setShape(QTabBar::RoundedEast);

    mTopTabBar->setObjectName("DockerTabBar");
    mBottomTabBar->setObjectName("DockerTabBar");
    mLeftTabBar->setObjectName("DockerTabBar");
    mRightTabBar->setObjectName("DockerTabBar");

    mBottomTabBar->addTab(QString("Tab1"));
    mBottomTabBar->addTab(QString("Tab2"));
    mBottomTabBar->addTab(QString("Tab3"));
    mBottomTabBar->addTab(QString("Tab4"));

    mLeftTabBar->addTab(QString("Tab1"));
    mLeftTabBar->addTab(QString("Tab2"));

    mRightTabBar->addTab(QString("Tab1"));
    mRightTabBar->addTab(QString("Tab2"));
}

DockerFrame::~DockerFrame()
{

}

void DockerFrame::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QRect rect = this->contentsRect();

    // 中心控件的位置和尺寸。
    int x = rect.x() + mLeftTabBar->sizeHint().width();
    int y = rect.y() + mTopTabBar->sizeHint().height();
    int w = rect.width() - x - mRightTabBar->sizeHint().width();
    int h = rect.height() - y - mBottomTabBar->sizeHint().height();

    mTopTabBar->setGeometry(x, rect.y(), w, y - rect.y());
    mBottomTabBar->setGeometry(x, y + h, w, rect.height() - y - h);
    mLeftTabBar->setGeometry(rect.x(), y, x - rect.x(), h);
    mRightTabBar->setGeometry(x + w, y, width() - x - w, h);
    if (mCenterWidget) mCenterWidget->setGeometry(x, y, w, h);
}

void DockerFrame::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}
