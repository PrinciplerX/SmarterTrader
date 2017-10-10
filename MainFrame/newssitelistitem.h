#ifndef NEWSSITELISTITEM_H
#define NEWSSITELISTITEM_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include "mainframe_global.h"
#include <QUrl>

class NewsSiteListItem : public QWidget
{
    Q_OBJECT
public:
    explicit NewsSiteListItem(const QUrl& siteUrl,const QString& siteName,QWidget *parent = 0);

private slots:
    void launchSite();

private:
    QPushButton* launchButton;
    QLabel* siteNameLabel;
    QUrl siteUrl;
    QString siteName;
};

#endif // NEWSSITELISTITEM_H
