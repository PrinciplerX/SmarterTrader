#include "tickdatawidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

TickDataWidget::TickDataWidget(QWidget *parent):
    QWidget(parent),
    dataProviderComboBox(new QComboBox()),
    symbolLineEdit(new QLineEdit()),
    downloadButton(new QPushButton(tr("查询"))),
    resultTextEdit(new QPlainTextEdit()),
    dateEdit(new QDateEdit())
{
    QStringList dataProviderList;
    dataProviderList << "新浪财经";
    dataProviderComboBox->addItems(dataProviderList);

    resultTextEdit->setReadOnly(true);
    resultTextEdit->setPlainText(QString("查询结果"));
    //resultTextEdit->setFontPointSize(10);

    QHBoxLayout* argLayout = new QHBoxLayout();
    argLayout->setMargin(0);
    argLayout->addWidget(dataProviderComboBox);
    argLayout->addWidget(symbolLineEdit);
    argLayout->addWidget(dateEdit);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    argLayout->addWidget(downloadButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addLayout(argLayout);
    mainLayout->addWidget(resultTextEdit);

    connect(downloadButton, SIGNAL(pressed()), this, SLOT(onDownloadButtonClicked()));

}

void TickDataWidget::onDownloadButtonClicked()
{
    mData = TickDataManager::instance().getTickData(symbolLineEdit->text(), dateEdit->text());
    connect(mData, SIGNAL(dataLoaded()), this, SLOT(reloadData()));
}

void TickDataWidget::reloadData()
{
    resultTextEdit->clear();
    if(mData->size() == 0){
        resultTextEdit->setPlainText(QString("[No Data]"));
        return;
    }
    for(int i = 0; i < mData->size(); i++){
        resultTextEdit->appendPlainText(QString("%1: [%2\t%3\t%4\t%5\t%6]").arg(mData->at(i).time,
                                                                       QString::number(mData->at(i).price),
                                                                       QString::number(mData->at(i).change),
                                                                       QString::number(mData->at(i).volume),
                                                                       QString::number(mData->at(i).amount),
                                                                       mData->at(i).type));
    }
}
