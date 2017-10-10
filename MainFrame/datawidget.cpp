#include "datawidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

DataWidget::DataWidget(QWidget *parent):
    QWidget(parent),
    dataProviderComboBox(new QComboBox()),
    periodComboBox(new QComboBox()),
    symbolLineEdit(new QLineEdit()),
    downloadButton(new QPushButton(tr("��ѯ"))),
    resultTextEdit(new QPlainTextEdit())
{
    QStringList dataProviderList;
    dataProviderList << "��˲ƾ�" << "���˲ƾ�" << "��Ѷ�ƾ�" << "�Ż��ƾ�";
    dataProviderComboBox->addItems(dataProviderList);

    QStringList periodList;
    periodList << "����" << "����" << "����" << "Сʱ��" << "30������" << "15������" << "5������" << "1������";
    periodComboBox->addItems(periodList);

    resultTextEdit->setReadOnly(true);
    resultTextEdit->setPlainText(QString("��ѯ���"));
    //resultTextEdit->set(10);

    QHBoxLayout* argLayout = new QHBoxLayout();
    argLayout->setMargin(0);
    argLayout->addWidget(dataProviderComboBox);
    argLayout->addWidget(symbolLineEdit);
    argLayout->addWidget(periodComboBox);
    argLayout->addWidget(downloadButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addLayout(argLayout);
    mainLayout->addWidget(resultTextEdit);

    connect(downloadButton, SIGNAL(pressed()), this, SLOT(onDownloadButtonClicked()));
}

void DataWidget::onDownloadButtonClicked()
{
    DataPeriod period;
    switch(periodComboBox->currentIndex()){
    case 0: period = DataPeriod::MONTH;      break;
    case 1: period = DataPeriod::WEEK;       break;
    case 2: period = DataPeriod::DAY;        break;
    case 3: period = DataPeriod::MINUTES_60; break;
    case 4: period = DataPeriod::MINUTES_30; break;
    case 5: period = DataPeriod::MINUTES_15; break;
    case 6: period = DataPeriod::MINUTES_5;  break;
    case 7: period = DataPeriod::MINUTES_1;  break;
    default: return;
    }
    currentData = HistoryDataManager::instance().getHistoryData(symbolLineEdit->text(), period);
    connect(currentData, SIGNAL(dataLoaded()), this, SLOT(reloadData()));
}

void DataWidget::reloadData()
{
    resultTextEdit->clear();
    if(currentData->size() == 0){
        resultTextEdit->setPlainText(QString("[No Data]"));
        return;
    }
    for(int i = 0; i < currentData->size(); i++){
        resultTextEdit->appendPlainText(QString("%1: [%2\t%3\t%4\t%5\t%6]").arg(currentData->at(i).date,
                                                                       QString::number(currentData->at(i).open),
                                                                       QString::number(currentData->at(i).high),
                                                                       QString::number(currentData->at(i).low),
                                                                       QString::number(currentData->at(i).close),
                                                                       QString::number(currentData->at(i).volume)));
    }
}
