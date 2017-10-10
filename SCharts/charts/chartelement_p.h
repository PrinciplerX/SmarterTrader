#ifndef CHARTELEMENT_H
#define CHARTELEMENT_H

#include <SChartGlobal>
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QGraphicsObject>

SCHARTS_BEGIN_NAMESPACE

class ChartAnimator;
class ChartPresenter;
class ChartAnimation;
class ChartThemeManager;
class AbstractDomain;
class ChartDataSet;

class SCHARTS_EXPORT ChartElement: public QGraphicsObject
{
    // ©¥£º£º©¥©¥£º Q_OBJECT
public:
    explicit ChartElement(QGraphicsItem* item = 0);

    virtual ChartAnimation *animation() const { return 0; }
    virtual void setPresenter(ChartPresenter *presenter);
    ChartPresenter *presenter() const;
    virtual void setThemeManager(ChartThemeManager *manager);
    ChartThemeManager* themeManager() const;
    virtual void setDataSet(ChartDataSet *dataSet);
    ChartDataSet *dataSet() const;

private:
    ChartPresenter *m_presenter;
    ChartThemeManager *m_themeManager;
    ChartDataSet *m_dataSet;
};

SCHARTS_END_NAMESPACE

#endif
