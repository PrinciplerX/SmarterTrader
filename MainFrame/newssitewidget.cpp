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
    mainLayout->addWidget(new QLabel(QString("��������Ѱ��Ǳ�ڵ�Ͷ�ʻ��ᡣ")));
    mainLayout->addLayout(newsSiteLayout);

    this->setLayout(mainLayout);
}

void NewsSiteWidget::initNewsList(){
    NewsSiteListItem* itemArray[13];

    itemArray[0]=new NewsSiteListItem(QUrl("http://www.sdpc.gov.cn/"),
                                                  QString("���ҷ�չ�͸ĸ�ίԱ��"));
    itemArray[1]=new NewsSiteListItem(QUrl("http://www.ce.cn/"),
                                                  QString("�й�������"));
    itemArray[2]=new NewsSiteListItem(QUrl("http://www.cs.com.cn/"),
                                                  QString("��֤��"));
    itemArray[3]=new NewsSiteListItem(QUrl("http://www.hexun.com/"),
                                                  QString("��Ѷ��"));
    itemArray[4]=new NewsSiteListItem(QUrl("http://xueqiu.com/"),
                                                  QString("ѩ����"));
    itemArray[5]=new NewsSiteListItem(QUrl("http://wallstreetcn.com/"),
                                                  QString("�����ּ���"));
    itemArray[6]=new NewsSiteListItem(QUrl("http://www.mof.gov.cn/"),
                                                  QString("�л����񹲺͹�������"));
    itemArray[7]=new NewsSiteListItem(QUrl("http://www.stats.gov.cn/"),
                                                  QString("�л����񹲺͹�ͳ�ƾ�"));
    itemArray[8]=new NewsSiteListItem(QUrl("http://www.stcn.com/"),
                                                  QString("֤ȯʱ����"));
    itemArray[9]=new NewsSiteListItem(QUrl("http://www.financialnews.com.cn/"),
                                                  QString("�й�����������"));
    itemArray[10]=new NewsSiteListItem(QUrl("http://www.yicai.com/"),
                                       QString("��һ�ƾ�"));

    for(int i=0;i<11;i++){
        QListWidgetItem* item=new QListWidgetItem(tr(""),newsSiteList);
        item->setSizeHint(QSize(-1,50));
        newsSiteList->setItemWidget(item,itemArray[i]);
    }
}
