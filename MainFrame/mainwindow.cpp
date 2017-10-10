#include "mainwindow.h"
#include "searchbox.h"
#include "favoritelistmodel.h"
#include "newswidget.h"
#include "accountpanel.h"

#include <QStringList>
#include "datawidget.h"
#include <QDebug>
#include <QTreeView>
#include <QSqlQueryModel>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) :
    DrawerFrame(parent),
    toolBoxPanel(new ToolBoxPanel()),
    settingsPanel(new SettingsPanel()),
    favoritePanel(new FavoritePanel(new FavoriteListModel())),
    indexQuotePanel(new IndexQuotePanel()),
    optionButton(new DetailButton(48,36,258)),
    bottomButton(new DetailButton(48,36,258)),
    listButton(new DetailButton(48,36,258)),
    systemTestButton(new DetailButton(48,36,258)),
    newsButton(new DetailButton(48,36,258)),
    accountButton(new DetailButton(48,36,258)),
    toolsButton(new DetailButton(48,36,258)),
    searchBox(new SearchBox()),
    statusBar(new QStatusBar())
{
    this->setWindowTitle(tr("SmarterTrader"));
    //this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    marketPanel = new MarketPanel();

    this->windowTitle();

    //Title Bar
    getTitleBar()->setStyleSheet(QString("QFrame#TitleBarWidget{background-color:#9D9D9D;}"));
    getTitleBar()->setObjectName(QString("TitleBarWidget"));

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT code, name, industry FROM ShareTable");
    model->setHeaderData(0, Qt::Horizontal, "code");
    model->setHeaderData(1, Qt::Horizontal, "name");
    model->setHeaderData(2, Qt::Horizontal, "industry");

    QTreeView *view = new QTreeView;
    view->setModel(model);
    view->setHeaderHidden(true);
    view->header()->setSectionResizeMode(QHeaderView::Stretch);
    view->header()->setStretchLastSection(false);
    view->setUniformRowHeights(true);//避免计算行高
    view->setRootIsDecorated(false);

    QCompleter *completer = new QCompleter(model, this);
    completer->setPopup(view);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    // searchBox->setSearchButtonIcon(qApp->style()->standardIcon(QStyle::SP_TitleBarContextHelpButton));
    // searchBox->setClearButtonIcon(qApp->style()->standardIcon(QStyle::SP_LineEditClearButton));

    searchBox->setFixedHeight(48);
    searchBox->setFixedWidth(320);
    searchBox->setCompleter(completer);

    connect(searchBox, SIGNAL(search(QString)), this, SLOT(onSearchBoxActivated(QString)));
    addTitleWidget(searchBox);

    //Title Label
    getTitleLabel()->setStyleSheet(QString("font:bold"));
    //getTitleLabel()->layout()->addWidget(new SearchBox());

    //Home Button
    getHomeButton()->setStyleSheet(QString("QPushButton{background-color:#FF3030;border:0px;}")+
                                   QString("QPushButton:hover{background-color:#FF5050;border:0px;}")+
                                   QString("QPushButton:pressed{background-color:#FF7070;border:0px;}"));

    //Drawer Widget
    getDrawerWidget()->setStyleSheet(QString("QFrame#DrawerWidget{background-color: #242424}"));
    getDrawerWidget()->setObjectName(QString("DrawerWidget"));

    //Back Button
    getBackButton()->setStyleSheet(QString("QPushButton{border:0px;}")+
                                   QString("QPushButton:hover{background-color:#D0D0D0;border:0px;}")+
                                   QString("QPushButton:pressed{background-color:#9D9D9D;border:0px;}"));

    //content panel
    indexQuotePanel->setStyleSheet(QString("border-left: 1px solid white;"));
    addBottomContentWidget(indexQuotePanel);

    //
    accountButton->setIcon(QIcon(":/ic_account_box_white_48dp.png"));
    toolsButton->setIcon(QIcon(":/ic_star_border_white_48dp.png"));
    systemTestButton->setIcon(QIcon(":/ic_history_white_48dp.png"));
    optionButton->setIcon(QIcon(":/ic_trending_up_white_48dp.png"));
    bottomButton->setIcon(QIcon(":/ic_settings_applications_white_48dp.png"));
    listButton->setIcon(QIcon(":/ic_collections_bookmark_white_48dp.png"));
    newsButton->setIcon(QIcon(":/ic_image_white_48dp.png"));

    QLabel* bl=new QLabel(tr("系统设置"));
    QLabel* ol=new QLabel(tr("市场行情"));
    QLabel* ll=new QLabel(tr("我的自选"));
    QLabel* sl=new QLabel(tr("量化投资"));
    QLabel* nl=new QLabel(tr("新闻资讯"));
    QLabel* al=new QLabel(tr("我的信息"));
    QLabel* tl=new QLabel(tr("工具箱"));

    bl->setStyleSheet(QString("color:white"));
    ol->setStyleSheet(QString("color:white"));
    ll->setStyleSheet(QString("color:white"));
    sl->setStyleSheet(QString("color:white"));
    nl->setStyleSheet(QString("color:white"));
    al->setStyleSheet(QString("color:white"));
    tl->setStyleSheet(QString("color:white"));

    newsButton->setDetailWidget(nl);
    bottomButton->setDetailWidget(bl);
    optionButton->setDetailWidget(ol);
    listButton->setDetailWidget(ll);
    systemTestButton->setDetailWidget(sl);
    toolsButton->setDetailWidget(tl);
    accountButton->setDetailWidget(al);

    /**********************************************
    QSplitter* detailPanel=new QSplitter(Qt::Vertical);
    QSplitter* tradingCurveWidget=new QSplitter(Qt::Horizontal);
    detailPanel->addWidget(new QLabel(QString("名称")));
    detailPanel->addWidget(new QLabel(QString("五档")));
    detailPanel->addWidget(new QLabel(QString("分笔")));
    detailPanel->addWidget(new QLabel(QString("基本信息")));
    tradingCurveWidget->addWidget(tradingCurvePlotPanel);
    tradingCurveWidget->addWidget(detailPanel);
    **********************************************/

    addUpperButtonPage(listButton, favoritePanel, tr("我的自选"));
    addUpperButtonPage(optionButton, marketPanel, tr("市场行情"));
    addUpperButtonPage(systemTestButton, new DataPanel(), tr("量化投资"));
    addUpperButtonPage(newsButton, new NewsWidget(), tr("新闻资讯"));
    addUpperButtonPage(toolsButton, toolBoxPanel, tr("工具箱"));

    addBottomButtonPage(accountButton, new AccountPanel(), tr("我的信息"));
    addBottomButtonPage(bottomButton, settingsPanel ,tr("系统设置"));

    setCurrentPage(0);

    connect(favoritePanel, SIGNAL(itemDoubleClicked(QString)), this, SLOT(onFavoritePanelItemSelected(QString)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    DrawerFrame::keyPressEvent(event);
}

void MainWindow::setPageButtonStyleSheet(DetailButton *pageButton) const{
    pageButton->setStyleSheet(QString("QPushButton{background-color:#242424;border:0px;font:white;}")+
                              QString("QPushButton:hover{background-color:#243434;border:0px;font:white;}")+
                              QString("QPushButton:pressed{background-color:#244444;border:0px;font:white;}")+
                              QString("QPushButton:checked{background-color:#AE5050;border:0px;font:white;}"));
}

void MainWindow::onSearchBoxActivated(const QString &keyword)
{
    QMessageBox::information(this, QString("来自MainWindow的消息"), QString("搜索框被触发，开始搜索。关键字为：%1\n\n. . .\n\n好吧其实并没有开始搜索 . . .").arg(keyword));
}

void MainWindow::onFavoritePanelItemSelected(const QString &symbol)
{
    marketPanel->setData(symbol);
    setCurrentPage(1);
}
