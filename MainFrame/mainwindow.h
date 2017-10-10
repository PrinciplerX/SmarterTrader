#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include "marketpanel.h"
#include "detailbutton.h"
#include "yahooquotefactory.h"
#include "drawerframe.h"
#include "newssitewidget.h"
#include "toolboxpanel.h"
#include "indexquotepanel.h"
#include "mainframe_global.h"
#include "settingspanel.h"
#include "searchbox.h"
#include "favoritepanel.h"
#include "datapanel.h"

/*!
 * \brief The MainWindow class
 *
 * \todo 需要完成不同page之间的跳转函数。比如，点击自选列表显示行情信息。
 */
class MainWindow : public DrawerFrame
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void setPageButtonStyleSheet(DetailButton *pageButton) const override;

protected slots:
    void onSearchBoxActivated(const QString& keyword);
    void onFavoritePanelItemSelected(const QString& symbol);

private:
    MarketPanel* marketPanel;
    ToolBoxPanel* toolBoxPanel;
    SettingsPanel* settingsPanel;
    FavoritePanel* favoritePanel;

    YahooQuoteFactory dataFactory;
    QStatusBar* statusBar;

    DetailButton* optionButton;
    DetailButton* bottomButton;
    DetailButton* listButton;
    DetailButton* systemTestButton;
    DetailButton* newsButton;
    DetailButton* accountButton;
    DetailButton* toolsButton;

    IndexQuotePanel* indexQuotePanel;

    SearchBox* searchBox;

    HistoryDataManager* historyDataManager;
};

#endif // MAINWINDOW_H
