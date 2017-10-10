#include "testpanel.h"
#include <QPushButton>
#include <QMovie>
#include <QGridLayout>
#include <QLabel>

TestPanel::TestPanel(QWidget *parent):
    QWidget(parent),
    maskWidget(new MaskWidget(this)),
    roundProgressBar(new RoundProgressBar(this))
{
    QPushButton* showMaskButton = new QPushButton("Hide");
    QPalette palette;
    palette.setBrush(QPalette::Base, Qt::lightGray);
    palette.setColor(QPalette::Text, QColor("#FF3030"));
    palette.setColor(QPalette::Shadow, Qt::lightGray);
    palette.setBrush(QPalette::Highlight, QColor("#FF3030"));

    roundProgressBar->setPalette(palette);
    roundProgressBar->setFixedSize(300, 300);
    //roundProgressBar->setBarStyle(RoundProgressBar::StyleLine);
    //roundProgressBar->setOutlinePenWidth(48);
    //roundProgressBar->setDataPenWidth(48);
    roundProgressBar->setValue(61.9);

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setRowStretch(0, 1);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->setRowStretch(3, 1);
    mainLayout->setRowStretch(5, 1);

    mainLayout->addWidget(roundProgressBar, 1, 1, 2, 2);
    mainLayout->addWidget(showMaskButton, 4, 1, 1, 2);

    QPushButton* hideMaskButton = new QPushButton("Show");
    maskWidget->hide();

    QLabel* lbTitle = new QLabel(tr("MaskWidget"));
    lbTitle->setStyleSheet("font-size: 28px; font-weight: bold; color: white");
    QLabel* lbProgress = new QLabel;
    QMovie* progressMovie = new QMovie(":/loader.gif");
    lbProgress->setMovie(progressMovie);
    progressMovie->start();
    QLabel* lbDescription = new QLabel(tr("-----------------------------------\n"
                                          "不好，我们找不到要给你看的东西了！\n"
                                          "加载中。。。\n"
                                          "没有可用的网络连接\n"
                                          "加载失败->刷新\n"
                                          "现在，MaskWidget已经占据了显示区域。\n"
                                          "点击按钮将其隐藏，然后回到之前的状态。\n"
                                          "-----------------------------------\n"));
    lbDescription->setStyleSheet("color: white");

    QGridLayout* lbLayout = new QGridLayout(maskWidget);
    lbLayout->setRowStretch(0, 1);
    lbLayout->setColumnStretch(0, 1);
    lbLayout->addWidget(lbTitle, 1, 1);
    lbLayout->addWidget(lbProgress, 1, 2, Qt::AlignRight);
    lbLayout->setColumnStretch(3, 1);
    lbLayout->addWidget(lbDescription, 2, 1, 1, 2);
    lbLayout->addWidget(hideMaskButton, 3, 2);
    lbLayout->setRowStretch(4, 1);

    connect(showMaskButton, SIGNAL(clicked()), maskWidget, SLOT(show()));
    connect(hideMaskButton, SIGNAL(clicked()), maskWidget, SLOT(hide()));
}
