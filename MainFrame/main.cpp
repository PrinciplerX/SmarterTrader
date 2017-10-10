#include "mainwindow.h"
#include <QApplication>
#include <traceplotcanvas.h>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QMenu>
#include <QPushButton>
#include <QStyle>
#include "searchbox.h"
#include "simplescalewidget.h"
#include "restingordertable.h"
#include "qwt_abstract_scale_draw.h"
#include "aboutwidget.h"
#include "simpleplot.h"
#include "splitterplot.h"
#include <QCalendarWidget>
#include <abstractplotlayout.h>
#include "classifyingmanager.h"
#include "historydatamanager.h"
#include <tradedata.h>
#include "datamanager.h"
#include "functionaltradedataitem.h"
#include "datagenetator.h"
#include "plotablecurvefactory.h"
#include "operationwrapperindicator.h"
#include "historydataparser.h"
#include "tradedataanalyser.h"
#include "tdxdata.h"
#include <boll.h>

#include "dockerframe.h"

//http://data.eastmoney.com/DataCenter_V3/stock2016/TradeDetail/pagesize=200,page=1,sortRule=-1,sortType=,startDate=2016-08-26,endDate=2016-08-26,gpfw=0,js=vardata_tab_1.html

using namespace TradingCurveWidgets;

/*!
 * \todo ?????????????????
 */

void realTimePlotTest(){
    RealTimePlot* plot = new RealTimePlot();
    plot->getPlotLayout()->setLayoutOption(AbstractPlotLayout::MinimizeCanvasMargin);
    plot->getAxisWidget(AbstractPlot::xBottom)->setFixedBorderDist(48, 48);
    plot->show();
}

void simpleScaleWidgetTest(){
    SimpleScaleWidget* t = new SimpleScaleWidget(AbstractAxis::BottomScale);
    t->setCanvasValueRange(5.0, 50.0, 5);
    t->updateScaleDiv();
    t->show();
}

void classifyingManagerTest(){
    ClassifyingManager* manager = ClassifyingManager::instance();
    manager->updateIndustryInfo();
    manager->updateIndustryDetail();
}

QNetworkReply* netTest(){
    static QNetworkAccessManager manager;
    return manager.get(QNetworkRequest(QUrl("http://www.baidu.com")));
}

void tdxText(const QString& filePath)
{
    TradeDataSeries data;
    HistoryDataParser::parseTdxFile(filePath, data);

    qDebug() << QString("%1 size: %2").arg(filePath, QString::number(data.size()));

    int inTrendBarCount = 0;
    int insideBarCount = 0;
    int outsideBarCount = 0;

    TradeDataAnalyser::analyseBars(data, inTrendBarCount, insideBarCount, outsideBarCount);
    int theRestBarCount = data.size() - inTrendBarCount - insideBarCount - outsideBarCount - 1;

    qDebug() << QString("In-Trend Bar Count: %1\t(%2%)").arg(QString::number(inTrendBarCount), QString::number(inTrendBarCount * 100.0 / (data.size() - 1), 'f', 1));
    qDebug() << QString("In-Side  Bar Count: %1\t(%2%)").arg(QString::number(insideBarCount),  QString::number(insideBarCount  * 100.0 / (data.size() - 1), 'f', 1));
    qDebug() << QString("Out-Side Bar Count: %1\t(%2%)").arg(QString::number(outsideBarCount), QString::number(outsideBarCount * 100.0 / (data.size() - 1), 'f', 1));
    qDebug() << QString("The Rest Bar Count: %1\t(%2%)").arg(QString::number(theRestBarCount), QString::number(theRestBarCount * 100.0 / (data.size() - 1), 'f', 1));

    TradeDataAnalyser analyser(data);
    QList<Trend> trends = analyser.trends();

    int trendsCount = trends.size();
    int size1Count = 0;
    int size2Count = 0;
    int size3Count = 0;
    int size4Count = 0;
    int sizeElseCount = 0;

    /***************/
    for (const Trend& item : trends) {
        switch (item.length) {
        case 1: size1Count++; break;
        case 2: size2Count++; break;
        case 3: size3Count++; break;
        case 4: size4Count++; break;
        default:sizeElseCount++; break;
        }

        //if (item.length > 2) {
            qDebug() << QString("[%1] [%2] %3 %4").arg(item.startTime, item.endTime, QString::number(item.length), QString(item.type));
        //}
    }

    qDebug() << QString("size1Count: %1(%2%)").arg(QString::number(size1Count), QString::number(size1Count * 100.0 / trendsCount, 'f', 1));
    qDebug() << QString("size2Count: %1(%2%)").arg(QString::number(size2Count), QString::number(size2Count * 100.0 / trendsCount, 'f', 1));
    qDebug() << QString("size3Count: %1(%2%)").arg(QString::number(size3Count), QString::number(size3Count * 100.0 / trendsCount, 'f', 1));
    qDebug() << QString("size4Count: %1(%2%)").arg(QString::number(size4Count), QString::number(size4Count * 100.0 / trendsCount, 'f', 1));
    qDebug() << QString("else Count: %1(%2%)").arg(QString::number(sizeElseCount), QString::number(sizeElseCount * 100.0 / trendsCount, 'f', 1));

    qDebug() << QString("tradeCount: %1(%2%)").arg(QString::number(trendsCount - size1Count - size2Count), QString::number((trendsCount - size1Count - size2Count) * 100.0 / trendsCount, 'f', 1));
    /****************/


}

void tdxReaderTest()
{
    TdxDataFrameReader reader;
    reader.readTdxFile("/home/principlerx/Desktop/tdx/20170428-1.txt");
    reader.readTdxFile("/home/principlerx/Desktop/tdx/20170428-2.txt");
    reader.readTdxFile("/home/principlerx/Desktop/tdx/20170428-3.txt");
    reader.readTdxFile("/home/principlerx/Desktop/tdx/20170428-4.txt");
    reader.readTdxFile("/home/principlerx/Desktop/tdx/20170428-5.txt");

    if (!reader.writeStdFile("/home/principlerx/Desktop/StSnapshot/StSnapshot.txt")) {
        qDebug() << "Write failed.";
    } else {
        qDebug() << "Write succeed.";
    }

    QTreeView* view = new QTreeView();
    view->setModel(new SnapshotModel(reader.data()));
    view->header()->setStretchLastSection(false);
    view->setUniformRowHeights(true);//避免计算行高

    view->setRootIsDecorated(false);
    view->header()->setSortIndicatorShown(true);

    view->show();
}

void randomDataTest()
{
    RandomDataGenerator* generator = new RandomDataGenerator(nullptr);

    // TradeDataItem* item = new TradeDataItem("sh000001", DataPeriod::DAY, "上证指数");

    FunctionalTradeDataItem* item = new FunctionalTradeDataItem(generator, "RAND1612", DataPeriod::DAY, "RAND1612");
    QTimer* timer = new QTimer();
    timer->setInterval(500);
    timer->start();
    QObject::connect(timer, SIGNAL(timeout()), item, SLOT(advance()));

    TradingCurvePlot* plot = new TradingCurvePlot();

    /**
    for (int i = 0; i < 5000; i++) {
        item->advance();
    }
    /**/

    CandleStick* cs = new CandleStick(item);
    //PlotableCurve* ma5 = PlotableCurveFactory::createMA(item, 5);//, QPen(Qt::darkBlue));
    //PlotableCurve* ma10 = PlotableCurveFactory::createMA(item, 10);//, QPen(Qt::darkBlue));
    //PlotableCurve* ma20 = PlotableCurveFactory::createMA(item, 20);//, QPen(Qt::darkBlue));

    PlotableCurve* ma5 = new PlotableCurve(MA(LOW(item), 5));
    PlotableCurve* ma10 = new PlotableCurve(MA(LOW(item), 10));
    PlotableCurve* ma20 = new PlotableCurve(MA(LOW(item), 20));

    item->attachIndicator(cs);
    item->attachIndicator(new OperationWrapperIndicator(ma5->operation(), QString("MA"), QString("MA5")));
    item->attachIndicator(new OperationWrapperIndicator(ma10->operation(), QString("MA"), QString("MA10")));
    item->attachIndicator(new OperationWrapperIndicator(ma20->operation(), QString("MA"), QString("MA20")));

    plot->addPlotableItem(cs);
    plot->addPlotableItem(ma5);
    plot->addPlotableItem(ma10);
    plot->addPlotableItem(ma20);
    plot->getItemAxis()->setTracerLabels(TradeDataHelper::getDateSeries(item->data()));

    plot->attachUpdater(item);
    plot->resize(300, 200);

    plot->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Set Icon
    a.setWindowIcon(QIcon(":/AppIcon.ico"));

    // dark fusion
    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;

    /**
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::lightGray);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    /**/
    qApp->setPalette(darkPalette);

    //tdxText("./999999_5min.txt");
    //    tdxText("./999999.txt");
    //    return 0;

    /***************[ QSS ]*********************/
    QFile qssFile(":/GlobalUiSettings.qss");
    qssFile.open(QFile::ReadOnly);

    if(qssFile.isOpen()){
        QString qss = QLatin1String(qssFile.readAll());
        a.setStyleSheet(qss);
        qssFile.close();
    }
    /*******************************************/

    // font
    int index=QFontDatabase::addApplicationFont("C:\\Users\\Princ\\OneDrive\\Documents\\Downloads\\MSYHMONO.ttf");
    qDebug() << "font index =" << index;
    if (index != -1){
        QStringList strList(QFontDatabase::applicationFontFamilies(index));
        if (strList.count() > 0){
            QFont fontThis(strList.at(0));
            //fontThis.setPointSize(10);
            a.setFont(fontThis);
            //QFont f = a.font();
            //f.setPointSize(11);
            //a.setFont(f);
        }
    } else {
        a.setFont(QFont("Fixedsys"));
    }

    //realTimePlotTest();
    classifyingManagerTest();
    // tdxReaderTest();

    /*------------------------------------------------------------------------*/
    MainWindow w;
    w.setMinimumSize(800,480);
    w.resize(800,480);
    w.show();
    /*------------------------------------------------------------------------*/

    // randomDataTest();

    /*
    ChartTestPanel chartPanel1;
    chartPanel1.show();
    ChartTestPanel chartPanel2;
    chartPanel2.show();
    ChartTestPanel chartPanel3;
    chartPanel3.show();
    ChartTestPanel chartPanel4;
    chartPanel4.show();
    */

    /****************************************************************
    TradeChartView chartView;
    chartView.setChart(new FChart()); // 换成QChart则没有问题。
    chartView.show();
    *****************************************************************/

    //QNetworkReply* reply = Downloader::instance().download(QString("http://www.baidu.com"));
    //QTimer timer;
    //timer.setInterval(5000);
    //QObject::connect(&timer, SIGNAL(timeout()), reply, SLOT(deleteLater()));
    //timer.start();

    //QNetworkReply* reply = Downloader::instance().download(QString("http://www.baidu.com"));

    //static QNetworkAccessManager manager;
    //manager.get(QNetworkRequest(QUrl("http://www.baidu.com")));

    // simpleScaleWidgetTest();

    /*------------------------------------------------------------------------*/

    /*
    QPushButton button(QString("Button"));
    QPalette pal = button.palette();
    pal.setBrush(QPalette::Active, QPalette::Shadow, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::WindowText, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Button, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Light, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Midlight, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Dark, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Mid, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Text, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::BrightText, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::ButtonText, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Base, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Window, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Highlight, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::HighlightedText, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::Link, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::LinkVisited, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::AlternateBase, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::NoRole, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::ToolTipBase, QBrush(QColor("yellow")));
    pal.setBrush(QPalette::Active, QPalette::ToolTipText, QBrush(QColor("yellow")));
    button.setPalette(pal);
    button.show();
    */

    return a.exec();
}
