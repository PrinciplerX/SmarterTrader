#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QDebug>

class AboutWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWidget(QWidget *parent = 0);

signals:

public slots:

private:
    static void initTextEdit(QTextEdit* edit);

    QLabel* logoLabel;
    QLabel* appLabel;
    QTextEdit* textEdit;
};

#endif // ABOUTWIDGET_H
