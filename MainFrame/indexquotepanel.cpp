#include "indexquotepanel.h"
#include "realtimedatamanager.h"
#include <QTextCodec>
#include <QToolButton>

IndexQuotePanel::IndexQuotePanel(QWidget* parent):
    QWidget(parent),
    mainLayout(new QHBoxLayout())
{
    this->setLayout(mainLayout);
    this->setFixedHeight(20);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0); 

    RealTimeDataManager& manager = RealTimeDataManager::instance();
    const AbstractRealTimeData* sseCompIndexData = manager.getRealTimeData(QString("sh000001"));
    const AbstractRealTimeData* szseCompSubIndexData = manager.getRealTimeData(QString("sz399001"));
    const AbstractRealTimeData* gemIndexData = manager.getRealTimeData(QString("sz399006"));

    IndexQuoteButton* sseCompIndexButton = new IndexQuoteButton(QString("沪"), sseCompIndexData);
    IndexQuoteButton* szseCompSubIndexButton = new IndexQuoteButton(QString("深"), szseCompSubIndexData);
    IndexQuoteButton* gemIndexButton = new IndexQuoteButton(QString("创"), gemIndexData);

    addIndexQuoteButton(sseCompIndexButton);
    addIndexQuoteButton(szseCompSubIndexButton);
    addIndexQuoteButton(gemIndexButton);

    // 设置按钮
    QPushButton* settingsButton = new QPushButton();
    settingsButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    QString borderStyleSheet("border:1px solid black; border-left:1px solid gray; border-right:1px solid gray;");
    QString indexLabelStyleSheet = QString("QPushButton{background-color:#242424;font: bold;") + borderStyleSheet + QString("}") +
                                   QString("QPushButton:hover{background-color:#243434;font: bold;") + borderStyleSheet + QString("}") +
                                   QString("QPushButton:pressed{background-color:#244444;font: bold;") + borderStyleSheet + QString("}");
    settingsButton->setStyleSheet(indexLabelStyleSheet);

    settingsButton->setIcon(QIcon(":/cog.png"));
    mainLayout->addWidget(settingsButton);
}

void IndexQuotePanel::addIndexQuoteButton(IndexQuoteButton *indexLabel){
    indexButtonVector.append(indexLabel);
    mainLayout->addWidget(indexLabel);
}

QPushButton *IndexQuotePanel::getIndexButton(int index){
    return indexButtonVector[index];
}

int IndexQuotePanel::size(){
    return indexButtonVector.size();
}

