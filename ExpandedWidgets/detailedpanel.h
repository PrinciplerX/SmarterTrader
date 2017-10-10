#ifndef DETAILEDPANEL_H
#define DETAILEDPANEL_H

#include <QWidget>
#include <QSplitter>
#include <QTableWidget>
#include "expandedwidgets_global.h"

class EXPANDEDWIDGETSSHARED_EXPORT DetailedPanel : public QSplitter
{
    Q_OBJECT
public:
    enum PanelSide{
        Left,
        Right
    };

    explicit DetailedPanel(PanelSide side = Right, QWidget* mainPanel = nullptr, QWidget *parent = nullptr);

    void setMainPanel(QWidget* mainPanel);

    QTabWidget* sidePanel();

signals:

public slots:
    void showSidePanel();
    void hideSidePanel();

private:
    QTabWidget* mSidePanel;
    QWidget* mMainPanel;
    PanelSide mPanelSide;
};

#endif // DETAILEDPANEL_H
