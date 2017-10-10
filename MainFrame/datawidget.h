#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>
#include "tradedata.h"
#include "historydatamanager.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>

/*!
 * \brief ���ڲ��Ի�ȡģ�飬��ʾ����������Ϣ�Ŀؼ���
 */
class DataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataWidget(QWidget *parent = 0);

signals:

public slots:
    void onDownloadButtonClicked();
    void reloadData();

private:
    QComboBox* dataProviderComboBox;
    QComboBox* periodComboBox;
    QLineEdit* symbolLineEdit;
    QPushButton* downloadButton;
    QPlainTextEdit* resultTextEdit;

    const AbstractHistoryData* currentData = nullptr;
};

#endif // DATAWIDGET_H
