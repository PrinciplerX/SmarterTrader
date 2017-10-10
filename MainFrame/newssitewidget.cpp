#include "newssitewidget.h"

NewsSiteWidget::NewsSiteWidget(QWidget *parent):
    QWidget(parent),
    newsSiteList(new QListWidget()),
    stackedWidget(new QStackedWidget())
{
    initNewsList();

    stackedWidget->addWidget(new QWidget());

    QHBoxLayout* newsSiteLayout=new QHBoxLayout();
    newsSiteLayout->addWidget(newsSiteList);
    newsSiteLayout->addWidget(stackedWidget);

    QVBoxLayout* mainLayout=new QVBoxLayout();
    mainLayout->addWidget(new QLabel(QString("从新闻中寻找潜在的投资机会。")));
    mainLayout->addLayout(newsSiteLayout);

    this->setLayout(mainLayout);
}

void NewsSiteWidget::initNewsList(){
    NewsSiteListItem* itemArray[13];

    itemArray[0]=new NewsSiteListItem(QUrl("http://www.sdpc.gov.cn/"),
                                                  QString("国家发展和改革委员会"));
    itemArray[1]=new NewsSiteListItem(QUrl("http://www.ce.cn/"),
                                                  QString("中国经济网"));
    itemArray[2]=new NewsSiteListItem(QUrl("http://www.cs.com.cn/"),
                                                  QString("中证网"));
    itemArray[3]=new NewsSiteListItem(QUrl("http://www.hexun.com/"),
                                                  QString("和讯网"));
    itemArray[4]=new NewsSiteListItem(QUrl("http://xueqiu.com/"),
                                                  QString("雪球网"));
    itemArray[5]=new NewsSiteListItem(QUrl("http://wallstreetcn.com/"),
                                                  QString("华尔街见闻"));
    itemArray[6]=new NewsSiteListItem(QUrl("http://www.mof.gov.cn/"),
                                                  QString("中华人民共和国财政部"));
    itemArray[7]=new NewsSiteListItem(QUrl("http://www.stats.gov.cn/"),
                                                  QString("中华人民共和国统计局"));
    itemArray[8]=new NewsSiteListItem(QUrl("http://www.stcn.com/"),
                                                  QString("证券时报网"));
    itemArray[9]=new NewsSiteListItem(QUrl("http://www.financialnews.com.cn/"),
                                                  QString("中国金融新闻网"));
    itemArray[10]=new NewsSiteListItem(QUrl("http://www.yicai.com/"),
                                       QString("第一财经"));

    for(int i=0;i<11;i++){
        QListWidgetItem* item=new QListWidgetItem(tr(""),newsSiteList);
        item->setSizeHint(QSize(-1,50));
        newsSiteList->setItemWidget(item,itemArray[i]);
    }
}
