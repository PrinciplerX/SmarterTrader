#include "fchartpresenter.h"
#include "fchart.h"
#include <AbstractChartLayout>

SCHARTS_BEGIN_NAMESPACE

FChartPresenter::FChartPresenter(FChart *chart) : ChartPresenter(chart, QChart::ChartTypeCartesian)
{

}

QVector<ChartPresenter *> FChartPresenter::presenters()
{
    return mPresenters;
}

void FChartPresenter::handleChartAdded(QChart *chart)
{
    mPresenters.append(chart->dPtr().data()->m_presenter);
    layout()->invalidate();
}

void FChartPresenter::handleChartRemoved(int index)
{
    mPresenters.remove(index);
    layout()->invalidate();
}

void FChartPresenter::handleChartInserted(int index, QChart *chart)
{
    mPresenters.insert(index, chart->dPtr().data()->m_presenter);
    layout()->invalidate();
}

SCHARTS_END_NAMESPACE
