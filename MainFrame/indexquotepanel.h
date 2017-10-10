#ifndef INDEXQUOTEPANEL_H
#define INDEXQUOTEPANEL_H

#include <titledlabel.h>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QVector>
#include <tradedata.h>
#include <QTimer>
#include "mainframe_global.h"

class IndexQuoteButton : public QPushButton
{
    Q_OBJECT
public:
    IndexQuoteButton(const QString& title, const AbstractRealTimeData* rtData, QWidget* parent = nullptr):
        QPushButton(parent),
        name(title),
        data(rtData)
    {
        QString borderStyleSheet("border:1px solid black; border-left:1px solid gray; border-right:1px solid gray;");
        QString indexLabelStyleSheet = QString("QPushButton{background-color:#242424;font: bold;") + borderStyleSheet + QString("}") +
                                       QString("QPushButton:hover{background-color:#243434;font: bold;") + borderStyleSheet + QString("}") +
                                       QString("QPushButton:pressed{background-color:#244444;font: bold;") + borderStyleSheet + QString("}");
        setStyleSheet(indexLabelStyleSheet);
        updateText();
        connect(data, SIGNAL(dataUpdated()), this, SLOT(updateText()));
    }

    void setName(const QString& name){
        this->name = name;
        updateText();
    }

public slots:
    void updateText(){
        if(!data->getQuoteData()->changeRate.isEmpty()){
            if(data->getQuoteData()->changeRate.at(0) == '+'){
                QPalette p = palette();
                p.setColor(QPalette::ButtonText, QColor("#FF5050"));
                setPalette(p);
            }else{
                QPalette p = palette();
                p.setColor(QPalette::ButtonText, QColor("#00A800"));
                setPalette(p);
            }
            setText(name + " " +
                    QString::number(data->getQuoteData()->current.toDouble(), 'f', 2) + " " +
                    data->getQuoteData()->change + " " +
                    data->getQuoteData()->changeRate);
        }else{
            QPalette p = palette();
            p.setColor(QPalette::ButtonText, Qt::white);
            setPalette(p);
            setText(name + " ---- --- ---");
        }

    }

private:
    const AbstractRealTimeData* data;
    QString name;
};

class IndexQuotePanel : public QWidget
{
    Q_OBJECT
public:
    IndexQuotePanel(QWidget *parent = nullptr);

    /*!
     * \brief 添加一个报价按钮
     * \param 按钮控件
     * \param 按钮上显示的报价项名称
     */
    void addIndexQuoteButton(IndexQuoteButton* indexLabel);
    QPushButton *getIndexButton(int index);

    int size();

private:
    QVector<IndexQuoteButton*> indexButtonVector;
    QVector<AbstractRealTimeData*> dataVector;

    QHBoxLayout* mainLayout;
};

#endif // INDEXQUOTEPANEL_H
