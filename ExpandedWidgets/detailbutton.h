#ifndef DETAILBUTTON_H
#define DETAILBUTTON_H

#include "expandedwidgets_global.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>

class EXPANDEDWIDGETSSHARED_EXPORT DetailButton : public QPushButton
{
    Q_OBJECT

public:
    DetailButton(int fixedHeight, int iconWidth, int maxButtonWidth, QWidget *parent=0);

    virtual ~DetailButton();

    void setIcon(const QIcon& icon);
    void setDetailWidget(QWidget *widget);

    virtual void resizeEvent(QResizeEvent *) override;
private:
    QLabel* iconLabel;
    int maxWidth;
    int fixedButtonHeight;
    QIcon icon;
    QHBoxLayout* mainLayout;
    QSize iconSize;
};

#endif // DETAILBUTTON_H
