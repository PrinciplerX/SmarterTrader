#ifndef TICKDATAWIDGET_H
#define TICKDATAWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <tradedata.h>
#include "tickdatamanager.h"
#include <QDateEdit>

class TickDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TickDataWidget(QWidget *parent = 0);

signals:

public slots:
    void onDownloadButtonClicked();
    void reloadData();

private:
    QComboBox* dataProviderComboBox;
    QLineEdit* symbolLineEdit;
    QPushButton* downloadButton;
    QPlainTextEdit* resultTextEdit;
    QDateEdit* dateEdit;

    const AbstractTickData* mData = nullptr;
};

#endif // TICKDATAWIDGET_H
