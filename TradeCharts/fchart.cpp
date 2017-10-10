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
    // �滻��ԭ�е� ChartDataSet �� ChartPresenter ����
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
    // �滻��ԭ�е� QChartPrivate ����
    FChartPrivate* d = new FChartPrivate(this);
    d_ptr.reset(d);
    d_ptr->init();

    // �滻��ԭ�е� layout ����
    mPanelLayout = new FPanelLayout(d->presenter());
    mPanelLayout->setContentsMargins(0, 0, 0, 0);
    mPanelLayout->setMargins(QMargins());
    setLayout(mPanelLayout);
    d->m_presenter->setLayout(mPanelLayout);

    // legend Ĭ�ϲ��ɼ���
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
        qWarning() << "�޷��ظ�����Ѿ����ڵ���ͼ�����";
        return;
    }

    // Ϊ��ֹ��ͼ��ԭ�еĲ��ֹ�����������ʾ����Ӱ�죬�����滻ΪVoidChartLayout��
    VoidChartLayout* flayout = new VoidChartLayout(chart->dPtr().data()->m_presenter);
    chart->setLayout(flayout);
    chart->dPtr().data()->m_presenter->setLayout(flayout);

    chart->setMargins(QMargins());
    chart->setContentsMargins(0, 0, 0, 0);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->layout()->setContentsMargins(0, 0, 0, 0);

    // ������ͬ��������ʾ��Χ��
    if (!mCharts.isEmpty()) {
        //! \warning ������Ҫÿ����ͼ��ĺ��ᶼ�� IndexAxis ʵ����
        chart->axisX()->setRange(((IndexAxis*)(mCharts.last()->axisX()))->min(),
                                 ((IndexAxis*)(mCharts.last()->axisX()))->max());
    }

    mCharts.append(chart);
    emit chartAdded(chart);

    // ������󣬷�ֹ����ͼ��ʱ��˸��
    // ������һ��ͼ��ĺ��ᣬ��ʾ�����ͼ��ĺ��ᡣ����ʹ������ͼ����ֻ��ʾһ���������ᡣ
    chart->axisX()->setVisible(true);
    if (mCharts.size() != 1) {
        mCharts[mCharts.size() - 2]->axisX()->setVisible(false);
    }
    chart->setParentItem(this);
}

void FChart::insert(int index, QChart *chart)
{
    if (mCharts.contains(chart)) {
        qWarning() << "�޷��ظ�����Ѿ����ڵ���ͼ�����";
        return;
    }

    // Ϊ��ֹ��ͼ��ԭ�еĲ��ֹ�����������ʾ����Ӱ�죬�����滻ΪVoidChartLayout��
    VoidChartLayout* flayout = new VoidChartLayout(chart->dPtr().data()->m_presenter);
    chart->setLayout(flayout);
    chart->dPtr().data()->m_presenter->setLayout(flayout);

    chart->setMargins(QMargins());
    chart->setContentsMargins(0, 0, 0, 0);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->setContentsMargins(0, 0, 0, 0);
    chart->legend()->layout()->setContentsMargins(0, 0, 0, 0);

    // ������ͬ��������ʾ��Χ��
    if (!mCharts.isEmpty()) {
        //! \warning ������Ҫÿ����ͼ��ĺ��ᶼ�� IndexAxis ʵ����
        chart->axisX()->setRange(((IndexAxis*)(mCharts.last()->axisX()))->min(),
                                 ((IndexAxis*)(mCharts.last()->axisX()))->max());
    }

    mCharts.insert(index, chart);
    emit chartInserted(index, chart);

    // ������󣬷�ֹ����ͼ��ʱ��˸��
    // ������һ��ͼ��ĺ��ᣬ��ʾ�����ͼ��ĺ��ᡣ����ʹ������ͼ����ֻ��ʾһ���������ᡣ
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

    // ����Ƴ����������ͼ����Ҫ���õ����ڶ���ͼ��ĺ�������Ϊ�ɼ���
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
