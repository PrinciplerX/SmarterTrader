#ifndef UICONFIGWIDGET_H
#define UICONFIGWIDGET_H

#include <QWidget>
#include <QComboBox>
#include "mainframe_global.h"
#include "norwegianwoodstyle.h"

class UiConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UiConfigWidget(QWidget *parent = 0);

signals:

private slots:
    void changeStyle(const QString &styleName);

private:
    QComboBox* mStyleComboBox;
};

#endif // UICONFIGWIDGET_H
