#ifndef FAVORITEPANEL_H
#define FAVORITEPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QTreeView>
#include "favoritelistmodel.h"
#include "mainframe_global.h"
#include "statisticpanel.h"
#include "realtimecompositeplot.h"

using TradingCurveWidgets::RealTimeCompositePlot;

class FavoritePanel : public QTabWidget
{
    Q_OBJECT
public:
    explicit FavoritePanel(FavoriteListModel* model,QWidget *parent = 0);

signals:
    void itemDoubleClicked(const QString& symbol);

public slots:
    void deleteItemSlot();
    void addItemSlot();
    void editSlot();

protected slots:
    void onTreeViewItemDoubleClicked(QModelIndex index);
    void onTreeViewItemClicked(QModelIndex index);

private:
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* editButton;

    RealTimeCompositePlot* itemPlot;
    RealTimeCompositePlot* indexPlot;

    QTreeView* treeView;
    FavoriteListModel* listModel;
};

#endif // FAVORITEPANEL_H
