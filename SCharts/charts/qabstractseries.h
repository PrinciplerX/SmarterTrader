#ifndef QABSTRACTSERIES_H
#define QABSTRACTSERIES_H

#include <SChartGlobal>
#include <QAbstractAxis>
#include <QtCore/QObject>
#include <QtGui/QPen>

SCHARTS_BEGIN_NAMESPACE

class QAbstractSeriesPrivate;
class QChart;

class SCHARTS_EXPORT QAbstractSeries : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(SeriesType type READ type)
    Q_PROPERTY(bool useOpenGL READ useOpenGL WRITE setUseOpenGL NOTIFY useOpenGLChanged)
    Q_ENUMS(SeriesType)

public:
    enum SeriesType {
        SeriesTypeLine,
        SeriesTypeArea,
        SeriesTypeBar,
        SeriesTypeStackedBar,
        SeriesTypePercentBar,
        SeriesTypePie,
        SeriesTypeScatter,
        SeriesTypeSpline,
        SeriesTypeHorizontalBar,
        SeriesTypeHorizontalStackedBar,
        SeriesTypeHorizontalPercentBar,
        SeriesTypeBoxPlot,
        SeriesTypeCandlestick
    };

protected:
    QAbstractSeries(QAbstractSeriesPrivate &d, QObject *parent = Q_NULLPTR);

public:
    ~QAbstractSeries();
    virtual SeriesType type() const = 0;

    void setName(const QString &name);
    QString name() const;
    void setVisible(bool visible = true);
    bool isVisible() const;
    qreal opacity() const;
    void setOpacity(qreal opacity);
    void setUseOpenGL(bool enable = true);
    bool useOpenGL() const;

    QChart *chart() const;

    bool attachAxis(QAbstractAxis *axis);
    bool detachAxis(QAbstractAxis *axis);
    QList<QAbstractAxis*> attachedAxes();

    void show();
    void hide();

Q_SIGNALS:
    void nameChanged();
    void visibleChanged();
    void opacityChanged();
    void useOpenGLChanged();

protected:
    QScopedPointer<QAbstractSeriesPrivate> d_ptr;
    friend class ChartDataSet;
    friend class ChartPresenter;
    friend class ChartThemeManager;
    friend class QLegendPrivate;
    friend class DeclarativeChart;
    friend class QAreaSeries;
};

SCHARTS_END_NAMESPACE

#endif // QABSTRACTSERIES_H
