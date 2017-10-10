#ifndef CHARTTESTPANEL_H
#define CHARTTESTPANEL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QtWidgets>
#include <tradechartview.h>
#include <fchart.h>
#include <historydatamanager.h>
#include <tradedatahelper.h>
#include <QMainWindow>
#include <QPushButton>
#include <tradedata.h>
#include <historydatamanager.h>
#include <QLegend>
#include <QBarCategoryAxis>
#include "candlestick.h"
#include "candlestickset.h"
#include "csplotseries.h"
#include <QGraphicsLayout>
#include <QValueAxis>
#include "indexaxis.h"
#include <HorizontalAxis>
#include <tradedatahelper.h>
#include <QGraphicsLinearLayout>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QAbstractBarSeries>
#include <QBarSeries>
#include <AbstractChartLayout>
#include <CartesianChartLayout>
#include <ChartPresenter>
#include <qchart_p.h>
#include "fchartlayout.h"
#include <QLineSeries>
#include "fchart.h"
#include <operation.h>
#include <tradedataoperations.h>
#include <QLogValueAxis>
#include <QChartView>
#include <fpanellayout.h>
#include <QCandlestickSeries>
#include <QCandlestickSet>

SCHARTS_USE_NAMESPACE

class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    ControlPanel(TradeChartView* chartView, FChart* chart, QWidget* parent = nullptr):
        QWidget(parent),
        mChartView(chartView),
        mChart(chart),
        mLoadDataButton(new QPushButton("加载数据")),
        mAddChartButton(new QPushButton("添加图表")),
        mRemoveChartButton(new QPushButton("移除图表")),
        mMaxIndexBox(new QSpinBox()),
        mMinIndexBox(new QSpinBox())
    {
        setStyleSheet(QString("font-size: 12px;"));

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(mLoadDataButton);

        QGroupBox* databox = new QGroupBox("数据");
        QVBoxLayout* databoxLayout = new QVBoxLayout(databox);
        databoxLayout->addWidget(mLoadDataButton);
        databoxLayout->addWidget(mAddChartButton);
        databoxLayout->addWidget(mRemoveChartButton);

        connect(mLoadDataButton, SIGNAL(clicked()), this, SLOT(loadData()));
        connect(mAddChartButton, SIGNAL(clicked()), this, SLOT(addChart()));
        connect(mRemoveChartButton, SIGNAL(clicked()), this, SLOT(removeChart()));

        QPushButton* formalChartButton = new QPushButton("官方图表");
        connect(formalChartButton, SIGNAL(clicked()), this, SLOT(formalChart()));
        databoxLayout->addWidget(formalChartButton);
        mainLayout->addWidget(databox);

        QGroupBox* axisxbox = new QGroupBox("时间轴");
        QFormLayout* axisxboxLayout = new QFormLayout(axisxbox);
        axisxboxLayout->addRow(QString("左端"), mMinIndexBox);
        axisxboxLayout->addRow(QString("右端"), mMaxIndexBox);
        connect(mMinIndexBox, SIGNAL(valueChanged(int)), this, SLOT(onIndexRangeChanged()));
        connect(mMaxIndexBox, SIGNAL(valueChanged(int)), this, SLOT(onIndexRangeChanged()));
        mainLayout->addWidget(axisxbox);

        QGroupBox* itemChartBox = new QGroupBox("子图表");
        QFormLayout* itemChartLayout = new QFormLayout(itemChartBox);
        mChartIndexEdit = new QLineEdit();
        itemChartLayout->addRow(QString("序号"), mChartIndexEdit);
        itemChartLayout->addRow(QString("主题"), new QComboBox());
        itemChartLayout->addRow(QString("动画选项"), new QComboBox());
        itemChartLayout->addRow(QString("坐标类型"), new QComboBox());
        itemChartLayout->addRow(QString("纵轴最大值"), new QLineEdit());
        itemChartLayout->addRow(QString("纵轴最小值"), new QLineEdit());
        QPushButton* logButton = new QPushButton("点击输出");
        QPushButton* detachButton = new QPushButton("点击分离");
        connect(logButton, SIGNAL(clicked()), this, SLOT(showLog()));
        itemChartLayout->addRow(QString("日志"), logButton);
        itemChartLayout->addRow(QString("分离"), detachButton);
        mainLayout->addWidget(itemChartBox);

        mainLayout->addStretch();
    }

protected slots:
    void loadData()
    {
        QVector<TradeData> d;
        if (TradeDataHelper::loadFromCsv(QString("./sh000001.csv"), d))
        {
            TradeDataItem* item = new TradeDataItem(QString("sh000001"), DataPeriod::DAY, QString("上证指数"), this);
            item->setData(d);
            mData = item;
            mLoadDataButton->setEnabled(false);
        } else {
            QMessageBox::warning(this, QString("数据加载错误"), QString("读取文件时出现错误。"));
        }

    }

    void formalChart()
    {
        if (!mData) {
            QMessageBox::warning(this, QString("没有加载数据"), QString("请加载数据后重试。"));
            return;
        }

        QCandlestickSeries *acmeSeries = new QCandlestickSeries();
        acmeSeries->setName("上证指数");
        acmeSeries->setIncreasingColor(QColor(Qt::red));
        acmeSeries->setDecreasingColor(QColor(Qt::green));

        //int startIndex = 0;
        int startIndex = qMax(0, mData->size() - 60);

        QStringList categories;
        for (int i = startIndex; i < mData->size(); i++) {
            const TradeData& data = mData->at(i);
            QCandlestickSet *set = new QCandlestickSet(data.open,
                                                       data.high,
                                                       data.low,
                                                       data.close,
                                                       QDateTime::fromString(data.date, Qt::ISODate).toMSecsSinceEpoch());
            set->setPen(QPen());
            if (set) {
                acmeSeries->append(set);
                categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("yyyy-MM-dd");
            }
        }

        QChart *chart = new QChart();
        chart->addSeries(acmeSeries);
        chart->setTitle("Acme Ltd Historical Data (July 2015)");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        chart->createDefaultAxes();

        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
        axisX->setCategories(categories);

        QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
        axisY->setMax(axisY->max() * 1.01);
        axisY->setMin(axisY->min() * 0.99);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QChartView *chartView = new QChartView(chart);
        //chartView->setRenderHint(QPainter::Antialiasing);
        chartView->show();

        QMainWindow* window = new QMainWindow(this);
        window->setCentralWidget(chartView);
        window->resize(800, 600);
        window->show();
    }

    void addChart()
    {
        if (!mData) {
            QMessageBox::warning(this, QString("没有加载数据"), QString("请加载数据后重试。"));
            return;
        }

        QTime time = QTime::currentTime();
        qsrand(time.msec() + time.second() * 1000);
        int index = qrand() % 2;

        switch(index) {
        case 0: mChart->append(createCandleStickChart()); break;
        case 1: mChart->append(createVolumeChart()); break;
        }
    }

    void removeChart()
    {
        if (mChart->count() == 0) {
            QMessageBox::information(this, QString("错误"), QString("没有可移除的图表"));
            return;
        }

        mChart->remove(mChart->count() - 1);
    }

    void showLog()
    {
        int index = mChartIndexEdit->text().toInt();
        if (index >= mChart->count()) {
            QMessageBox::warning(this, QString("错误"), QString("子图表序号越界。"));
            return;
        }

        QString rect("%1(x=%2, y=%3, w=%4, h=%5)");
        QString content = QString("###[SHOW LOG %1]###\n").arg(QString::number(index));
        QRectF r = mChart->chart(index)->plotArea();
        content.append(rect.arg("plot area",
                                QString::number(r.x()),
                                QString::number(r.y()),
                                QString::number(r.width()),
                                QString::number(r.height())));

        QMessageBox::information(this, QString("详细"), content);
    }

    void onIndexRangeChanged()
    {
        int min = mMinIndexBox->value();
        int max = mMaxIndexBox->value();
        mChart->setIndexRange(min, max - min + 1);
    }

protected:
    void addCandleStickChart();

    QChart* createCandleStickChart();

    QChart* createVolumeChart();
    QAbstractSeries* createVolumeSeries();

private:
    AbstractHistoryData* mData = nullptr;
    TradeChartView* mChartView;
    FChart* mChart;

    QLineEdit* mChartIndexEdit;
    QPushButton* mLoadDataButton;
    QPushButton* mAddChartButton;
    QPushButton* mRemoveChartButton;

    QSpinBox* mMaxIndexBox;
    QSpinBox* mMinIndexBox;
};

class ChartTestPanel : public QWidget
{
    Q_OBJECT
public:
    ChartTestPanel(QWidget* parent = nullptr);

private:
    TradeChartView* chartView;
    FChart* chart;

    ControlPanel* controller;
};

#endif // CHARTTESTPANEL_H
