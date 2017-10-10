#ifndef CHARTDATASET_P_H
#define CHARTDATASET_P_H

#include <QAbstractSeries>
#include <AbstractDomain>
#include <QAbstractAxis>
#include <QtCore/QVector>

SCHARTS_BEGIN_NAMESPACE

class QAbstractAxis;
class ChartPresenter;
class GLXYSeriesDataManager;

class SCHARTS_EXPORT ChartDataSet : public QObject
{
    Q_OBJECT
public:
    ChartDataSet(QChart *chart);
    virtual ~ChartDataSet();

    void addSeries(QAbstractSeries *series);
    void removeSeries(QAbstractSeries *series);
    QList<QAbstractSeries *> series() const;

    void addAxis(QAbstractAxis *axis,Qt::Alignment aligment);
    void removeAxis(QAbstractAxis *axis);
    QList<QAbstractAxis*> axes() const;

    bool attachAxis(QAbstractSeries* series,QAbstractAxis *axis);
    bool detachAxis(QAbstractSeries* series,QAbstractAxis *axis);

    void createDefaultAxes();

    void zoomInDomain(const QRectF &rect);
    void zoomOutDomain(const QRectF &rect);
    void zoomResetDomain();
    bool isZoomedDomain();
    void scrollDomain(qreal dx, qreal dy);

    QPointF mapToValue(const QPointF &position, QAbstractSeries *series = 0);
    QPointF mapToPosition(const QPointF &value, QAbstractSeries *series = 0);

    GLXYSeriesDataManager *glXYSeriesDataManager() { return m_glXYSeriesDataManager; }

Q_SIGNALS:
    void axisAdded(QAbstractAxis* axis);
    void axisRemoved(QAbstractAxis* axis);
    void seriesAdded(QAbstractSeries* series);
    void seriesRemoved(QAbstractSeries* series);
public Q_SLOTS:
    void reverseChanged();
private:
    void createAxes(QAbstractAxis::AxisTypes type, Qt::Orientation orientation);
    QAbstractAxis *createAxis(QAbstractAxis::AxisType type, Qt::Orientation orientation);
    AbstractDomain::DomainType selectDomain(QList<QAbstractAxis* > axes);
    AbstractDomain* createDomain(AbstractDomain::DomainType type);
    void deleteAllAxes();
    void deleteAllSeries();
    void findMinMaxForSeries(QList<QAbstractSeries *> series,Qt::Orientations orientation, qreal &min, qreal &max);
private:
    QList<QAbstractSeries *> m_seriesList;
    QList<QAbstractAxis *> m_axisList;
    QChart* m_chart;
    GLXYSeriesDataManager *m_glXYSeriesDataManager;
};

SCHARTS_END_NAMESPACE

#endif /* CHARTENGINE_P_H */
