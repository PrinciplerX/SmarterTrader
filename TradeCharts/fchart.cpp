#include "fchart.h"
#include "fchartdataset.h"
#include "fchartpresenter.h"
#include "fpanellayout.h"
#include "voidchartlayout.h"
#include "fchartlayout.h"
#include <QAbstractAxisPrivate>
#include "indexaxis.h"

SCHARTS_BEGIN_NAMESPACE

FChartPrivate::FChartPrivate(FChart *q) : QChartPrivate(q, QChart::ChartTypeCartesian)
{
    // 替换掉原有的 ChartDataSet 与 ChartPresenter 对象。
    this->m_dataset = new FChartDataSet(q);
    this->m_presenter = new FChartPresenter(q);
}

FChartPresenter *FChartPrivate::presenter()
{
    return (FChartPresenter*)m_presenter;
}

FChartDataSet *FChartPrivate::dataset()
{
    return (FChartDataSet*)m_dataset;
}

FChart::FChart(QGraphicsItem *parent, Qt::WindowFlags wFlags) : QChart(parent, wFlags)
{
    // 替换掉原有的 QChartPrivate 对象。
    FChartPrivate* d = new FChartPrivate(this);
    d_ptr.reset(d);
    d_ptr->init();

    // 替换掉原有的 layout 对象。
    mPanelLayout = new FPanelLayout(d->presenter());
    mPanelLayout->setContentsMargins(0, 0, 0, 0);
    mPanelLayout->setMargins(QMargins());
    setLayout(mPanelLayout);
    d->m_presenter->setLayout(mPanelLayout);

    // legend 默认不可见。
    legend()->setVisible(false);

    FChartPresenter* presenter = d->presenter();
    QObject::connect(this, SIGNAL(chartAdded(QChart*)), presenter, SLOT(handleChartAdded(QChart*)));
    QObject::connect(this, SIGNAL(chartRemoved(int)), presenter, SLOT(handleChartRemoved(int)));
    QObject::connect(this, SIGNAL(chartInserted(int,QChart*)), presenter, SLOT(handleChartInserted(int,QChart*)));
}

FChart::~FChart()
{

}

void FChart::append(QChart *chart)
{
    if (mCharts.contains(chart)) {
        qWarning() << "无法重复添加已经存在的子图表对象";
        return;
    }

    // 为防止子图表原有的布局管理器对其显示产生影响，将其替换为VoidChartLayout。
    VoidChartLayout* flayout = new VoidChartLayout(chart->dPtr().data()->m_presenter);
    chart->setLayout(flayout);
    chart->dPtr().data()->m_presenter->setLayout(flayout);

    chart->setMargins(QMargins());
    chart->setContentsMargins(0, 0, 0, 0);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->layout()->setContentsMargins(0, 0, 0, 0);

    // 设置相同的序列显示范围。
    if (!mCharts.isEmpty()) {
        //! \warning 这里需要每个子图表的横轴都是 IndexAxis 实例。
        chart->axisX()->setRange(((IndexAxis*)(mCharts.last()->axisX()))->min(),
                                 ((IndexAxis*)(mCharts.last()->axisX()))->max());
    }

    mCharts.append(chart);
    emit chartAdded(chart);

    // 放在最后，防止加载图表时闪烁。
    // 隐藏上一个图表的横轴，显示新添加图表的横轴。这样使得整个图表中只显示一个横坐标轴。
    chart->axisX()->setVisible(true);
    if (mCharts.size() != 1) {
        mCharts[mCharts.size() - 2]->axisX()->setVisible(false);
    }
    chart->setParentItem(this);
}

void FChart::insert(int index, QChart *chart)
{
    if (mCharts.contains(chart)) {
        qWarning() << "无法重复添加已经存在的子图表对象";
        return;
    }

    // 为防止子图表原有的布局管理器对其显示产生影响，将其替换为VoidChartLayout。
    VoidChartLayout* flayout = new VoidChartLayout(chart->dPtr().data()->m_presenter);
    chart->setLayout(flayout);
    chart->dPtr().data()->m_presenter->setLayout(flayout);

    chart->setMargins(QMargins());
    chart->setContentsMargins(0, 0, 0, 0);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->layout()->setContentsMargins(0, 0, 0, 0);

    // 设置相同的序列显示范围。
    if (!mCharts.isEmpty()) {
        //! \warning 这里需要每个子图表的横轴都是 IndexAxis 实例。
        chart->axisX()->setRange(((IndexAxis*)(mCharts.last()->axisX()))->min(),
                                 ((IndexAxis*)(mCharts.last()->axisX()))->max());
    }

    mCharts.insert(index, chart);
    emit chartInserted(index, chart);

    // 放在最后，防止加载图表时闪烁。
    // 隐藏上一个图表的横轴，显示新添加图表的横轴。这样使得整个图表中只显示一个横坐标轴。
    if (mCharts.size() != index) {
        chart->axisX()->setVisible(true);
        if (index != 0) {
            mCharts.last()->axisX()->setVisible(false);
        }
    } else {
        chart->axisX()->setVisible(false);
    }
    chart->setParentItem(this);
}

void FChart::remove(int index)
{
    takeAt(index)->deleteLater();
}

QChart *FChart::takeAt(int index)
{
    QChart* chartToRemove = mCharts[index];
    chartToRemove->setParent(nullptr);
    chartToRemove->setParentItem(nullptr);

    // 如果移除了最下面的图表，需要设置倒数第二个图表的横坐标轴为可见。
    if (mCharts.size() - 1 == index && index != 0) {
        mCharts[mCharts.size() - 2]->axisX()->setVisible(true);
    }

    mCharts.remove(index);

    emit chartRemoved(index);
    return chartToRemove;
}

int FChart::count()
{
    return mCharts.count();
}

FPanelLayout *FChart::panelLayout()
{
    return mPanelLayout;
}

void FChart::setIndexRange(int start, int offset)
{
    for (QChart* item : mCharts) {
        item->axisX()->setRange(start - 0.5, start + offset + 0.5);
    }
}

QChart *FChart::chart(int index)
{
    return mCharts[index];
}

SCHARTS_END_NAMESPACE
