#include "newssitelistitem.h"
#include <QDesktopServices>

NewsSiteListItem::NewsSiteListItem(const QUrl& siteUrl,const QString& siteName,QWidget *parent)
    :QWidget(parent)
    ,siteName(siteName)
    ,siteUrl(siteUrl)
    ,launchButton(new QPushButton(tr("")))
    ,siteNameLabel(new QLabel(siteName))
{
    QHBoxLayout* mainLayout=new QHBoxLayout();
    mainLayout->addWidget(siteNameLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(launchButton);

    this->setLayout(mainLayout);
    connect(launchButton,SIGNAL(clicked()),this,SLOT(launchSite()));
}

void NewsSiteListItem::launchSite(){
    QDesktopServices::openUrl(siteUrl);
}
