#ifndef QCHART_H
#define QCHART_H

#include <QAbstractSeries>
#include <QLegend>
#include <QtWidgets/QGraphicsWidget>
#include <QtCore/QMargins>

QT_BEGIN_NAMESPACE
class QGraphicsSceneResizeEvent;
QT_END_NAMESPACE

SCHARTS_BEGIN_NAMESPACE

class QAbstractSeries;
class QAbstractAxis;
class QLegend;
class QChartPrivate;
class QBoxPlotSeries;

class SCHARTS_EXPORT QChart : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(QChart::ChartTheme theme READ theme WRITE setTheme)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(bool backgroundVisible READ isBackgroundVisible WRITE setBackgroundVisible)
    Q_PROPERTY(bool dropShadowEnabled READ isDropShadowEnabled WRITE setDropShadowEnabled)
    Q_PROPERTY(qreal backgroundRoundness READ backgroundRoundness WRITE setBackgroundRoundness)
    Q_PROPERTY(QChart::AnimationOptions animationOptions READ animationOptions WRITE setAnimationOptions)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(QEasingCurve animationEasingCurve READ animationEasingCurve WRITE setAnimationEasingCurve)
    Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
    Q_PROPERTY(QChart::ChartType chartType READ chartType)
    Q_PROPERTY(bool plotAreaBackgroundVisible READ isPlotAreaBackgroundVisible WRITE setPlotAreaBackgroundVisible)
    Q_PROPERTY(bool localizeNumbers READ localizeNumbers WRITE setLocalizeNumbers)
    Q_PROPERTY(QLocale locale READ locale WRITE setLocale)
    Q_PROPERTY(QRectF plotArea READ plotArea NOTIFY plotAreaChanged)
    Q_ENUMS(ChartTheme)
    Q_ENUMS(AnimationOption)
    Q_ENUMS(ChartType)

public:
    enum ChartType {
        ChartTypeUndefined = 0,
        ChartTypeCartesian,
        ChartTypePolar
    };

    enum ChartTheme {
        ChartThemeLight = 0,
        ChartThemeBlueCerulean,
        ChartThemeDark,
        ChartThemeBrownSand,
        ChartThemeBlueNcs,
        ChartThemeHighContrast,
        ChartThemeBlueIcy,
        ChartThemeQt
    };

    enum AnimationOption {
        NoAnimation = 0x0,
        GridAxisAnimations = 0x1,
        SeriesAnimations = 0x2,
        AllAnimations = 0x3
    };

    Q_DECLARE_FLAGS(AnimationOptions, AnimationOption)

public:
    explicit QChart(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
    ~QChart();

    void addSeries(QAbstractSeries *series);
    void removeSeries(QAbstractSeries *series);
    void removeAllSeries();
    QList<QAbstractSeries *> series() const;

    // *** deprecated ***
    void setAxisX(QAbstractAxis *axis, QAbstractSeries *series = Q_NULLPTR);
    void setAxisY(QAbstractAxis *axis, QAbstractSeries *series = Q_NULLPTR);
    QAbstractAxis *axisX(QAbstractSeries *series = Q_NULLPTR) const;
    QAbstractAxis *axisY(QAbstractSeries *series = Q_NULLPTR) const;
    // ******************

    void addAxis(QAbstractAxis *axis, Qt::Alignment alignment);
    void removeAxis(QAbstractAxis *axis);
    QList<QAbstractAxis*> axes(Qt::Orientations orientation = Qt::Horizontal|Qt::Vertical, QAbstractSeries *series = Q_NULLPTR) const;

    void createDefaultAxes();

    void setTheme(QChart::ChartTheme theme);
    QChart::ChartTheme theme() const;

    void setTitle(const QString &title);
    QString title() const;
    void setTitleFont(const QFont &font);
    QFont titleFont() const;
    void setTitleBrush(const QBrush &brush);
    QBrush titleBrush() const;

    void setBackgroundBrush(const QBrush &brush);
    QBrush backgroundBrush() const;
    void setBackgroundPen(const QPen &pen);
    QPen backgroundPen() const;
    void setBackgroundVisible(bool visible = true);
    bool isBackgroundVisible() const;

    void setDropShadowEnabled(bool enabled = true);
    bool isDropShadowEnabled() const;
    void setBackgroundRoundness(qreal diameter);
    qreal backgroundRoundness() const;

    void setAnimationOptions(AnimationOptions options);
    AnimationOptions animationOptions() const;
    void setAnimationDuration(int msecs);
    int animationDuration() const;
    void setAnimationEasingCurve(const QEasingCurve &curve);
    QEasingCurve animationEasingCurve() const;

    void zoomIn();
    void zoomOut();

    void zoomIn(const QRectF &rect);
    void zoom(qreal factor);
    void zoomReset();
    bool isZoomed();

    void scroll(qreal dx, qreal dy);

    QLegend *legend() const;

    void setMargins(const QMargins &margins);
    QMargins margins() const;

    QRectF plotArea() const;
    void setPlotAreaBackgroundBrush(const QBrush &brush);
    QBrush plotAreaBackgroundBrush() const;
    void setPlotAreaBackgroundPen(const QPen &pen);
    QPen plotAreaBackgroundPen() const;
    void setPlotAreaBackgroundVisible(bool visible = true);
    bool isPlotAreaBackgroundVisible() const;
    void setLocalizeNumbers(bool localize);
    bool localizeNumbers() const;
    void setLocale(const QLocale &locale);
    QLocale locale() const;

    QPointF mapToValue(const QPointF &position, QAbstractSeries *series = Q_NULLPTR);
    QPointF mapToPosition(const QPointF &value, QAbstractSeries *series = Q_NULLPTR);

    ChartType chartType() const;

    //!\todo �ҵ����õĽ���취
    QScopedPointer<QChartPrivate>& dPtr() { return d_ptr; }

Q_SIGNALS:
    void plotAreaChanged(const QRectF &plotArea);

protected:
    explicit QChart(QChart::ChartType type, QGraphicsItem *parent, Qt::WindowFlags wFlags);
    QScopedPointer<QChartPrivate> d_ptr;
    friend class QLegend;
    friend class DeclarativeChart;
    friend class ChartDataSet;
    friend class ChartPresenter;
    friend class ChartThemeManager;
    friend class QAbstractSeries;
    friend class QBoxPlotSeriesPrivate;
    friend class QCandlestickSeriesPrivate;

private:
    Q_DISABLE_COPY(QChart)
};

SCHARTS_END_NAMESPACE

Q_DECLARE_OPERATORS_FOR_FLAGS(SCHARTS_NAMESPACE::QChart::AnimationOptions)

#endif // QCHART_H
