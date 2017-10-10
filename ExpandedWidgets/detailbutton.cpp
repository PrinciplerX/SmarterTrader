#include "detailbutton.h"
#include <QResizeEvent>
#include <QDebug>

DetailButton::DetailButton(int fixedHeight, int iconWidth, int maxButtonWidth, QWidget* parent):
    QPushButton(parent),
    fixedButtonHeight(fixedHeight),
    iconSize(QSize(iconWidth,iconWidth))
{
    this->maxWidth=maxButtonWidth;
    mainLayout=new QHBoxLayout();
    iconLabel=new QLabel();
    //使图标居中显示。
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedSize(fixedHeight,fixedHeight);

    mainLayout->addWidget(iconLabel);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setAlignment(iconLabel,Qt::AlignLeft);
    this->setMinimumSize(QSize(fixedHeight,fixedHeight));
    this->setLayout(mainLayout);
}

DetailButton::~DetailButton()
{

}

void DetailButton::setIcon(const QIcon &icon){
    iconLabel->setPixmap(icon.pixmap(iconSize));
    update();
}

void DetailButton::setDetailWidget(QWidget *widget){
    mainLayout->addSpacing(iconLabel->width()-iconSize.width());
    mainLayout->addWidget(widget,0,Qt::AlignVCenter);
    mainLayout->addStretch();
    update();
}

void DetailButton::resizeEvent(QResizeEvent *event){
    QPushButton::resizeEvent(event);
    this->setGeometry(x(),y(),maxWidth,fixedButtonHeight);
}

