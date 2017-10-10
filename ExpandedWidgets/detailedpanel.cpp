#include "detailedpanel.h"

DetailedPanel::DetailedPanel(PanelSide side, QWidget *mainPanel, QWidget *parent):
    QSplitter(Qt::Horizontal, parent),
    mSidePanel(new QTabWidget()),
    mMainPanel(mainPanel),
    mPanelSide(side)
{
    if(!mMainPanel) mMainPanel = new QWidget();

    if(side == Left){
        addWidget(mSidePanel);
        addWidget(mMainPanel);
    }else{
        addWidget(mSidePanel);
        addWidget(mMainPanel);
    }

    setHandleWidth(1);
    setChildrenCollapsible(false);
    mSidePanel->tabBar()->setObjectName(QString("ChoiceTabBar"));
}

void DetailedPanel::setMainPanel(QWidget *mainPanel)
{
    if(mMainPanel)
        delete mMainPanel;
    mMainPanel = mainPanel;
    insertWidget(mPanelSide == Left ? 1 : 0, mMainPanel);
}

QTabWidget *DetailedPanel::sidePanel()
{
    return mSidePanel;
}

void DetailedPanel::showSidePanel()
{
    mSidePanel->show();
}

void DetailedPanel::hideSidePanel()
{
    mSidePanel->hide();
}
