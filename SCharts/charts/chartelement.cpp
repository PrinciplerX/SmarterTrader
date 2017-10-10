#include <chartelement_p.h>
#include <chartpresenter_p.h>
#include <abstractdomain_p.h>
#include <chartdataset_p.h>

SCHARTS_BEGIN_NAMESPACE

ChartElement::ChartElement(QGraphicsItem* item):
      QGraphicsObject(item),
      m_presenter(0),
      m_themeManager(0),
      m_dataSet(0)
{

}

void ChartElement::setPresenter(ChartPresenter *presenter)
{
    m_presenter = presenter;
}

ChartPresenter *ChartElement::presenter() const
{
    return m_presenter;
}

void ChartElement::setThemeManager(ChartThemeManager *manager)
{
    m_themeManager = manager;
}

ChartThemeManager* ChartElement::themeManager() const
{
    return m_themeManager;
}

void ChartElement::setDataSet(ChartDataSet *dataSet)
{
    m_dataSet = dataSet;
}

ChartDataSet *ChartElement::dataSet() const
{
    return m_dataSet;
}

SCHARTS_END_NAMESPACE
