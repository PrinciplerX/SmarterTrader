#ifndef CHARTPRESENTER_H
#define CHARTPRESENTER_H

#include <SChartGlobal>
#include "qchart.h" //because of QChart::ChartThemeId
#include <glwidget_p.h>
#include <QtCore/QRectF>
#include <QtCore/QMargins>
#include <QtCore/QLocale>
#include <QtCore/QPointer>
#include <QtCore/QEasingCurve>

SCHARTS_BEGIN_NAMESPACE

class ChartItem;
class AxisItem;
class QAbstractSeries;
class ChartDataSet;
class AbstractDomain;
class ChartAxisElement;
class ChartAnimator;
class ChartBackground;
class ChartTitle;
class ChartAnimation;
class AbstractChartLayout;

class SCHARTS_EXPORT ChartPresenter: public QObject
{
    Q_OBJECT
public:
    enum ZValues {
          BackgroundZValue = -1,
          PlotAreaZValue,
          ShadesZValue,
          GridZValue,
          AxisZValue,
          SeriesZValue,
          LineChartZValue = SeriesZValue,
          SplineChartZValue = SeriesZValue,
          BarSeriesZValue = SeriesZValue,
          ScatterSeriesZValue = SeriesZValue,
          PieSeriesZValue = SeriesZValue,
          BoxPlotSeriesZValue = SeriesZValue,
          CandlestickSeriesZValue = SeriesZValue,
          LegendZValue,
          TopMostZValue
      };

    enum State {
        ShowState,
        ScrollUpState,
        ScrollDownState,
        ScrollLeftState,
        ScrollRightState,
        ZoomInState,
        ZoomOutState
    };

    ChartPresenter(QChart *chart, QChart::ChartType type);
    virtual ~ChartPresenter();


    void setGeometry(QRectF rect);
    QRectF geometry() const;

    QGraphicsItem *rootItem(){ return m_chart; }
    ChartBackground *backgroundElement();
    QAbstractGraphicsShapeItem *plotAreaElement();
    ChartTitle *titleElement();
    QList<ChartAxisElement *> axisItems() const;
    QList<ChartItem *> chartItems() const;

    QLegend *legend();

    void setBackgroundBrush(const QBrush &brush);
    QBrush backgroundBrush() const;

    void setBackgroundPen(const QPen &pen);
    QPen backgroundPen() const;

    void setBackgroundRoundness(qreal diameter);
    qreal backgroundRoundness() const;

    void setPlotAreaBackgroundBrush(const QBrush &brush);
    QBrush plotAreaBackgroundBrush() const;

    void setPlotAreaBackgroundPen(const QPen &pen);
    QPen plotAreaBackgroundPen() const;

    void setTitle(const QString &title);
    QString title() const;

    void setTitleFont(const QFont &font);
    QFont titleFont() const;

    void setTitleBrush(const QBrush &brush);
    QBrush titleBrush() const;

    void setBackgroundVisible(bool visible);
    bool isBackgroundVisible() const;

    void setPlotAreaBackgroundVisible(bool visible);
    bool isPlotAreaBackgroundVisible() const;

    void setBackgroundDropShadowEnabled(bool enabled);
    bool isBackgroundDropShadowEnabled() const;

    void setLocalizeNumbers(bool localize);
    inline bool localizeNumbers() const { return m_localizeNumbers; }
    void setLocale(const QLocale &locale);
    inline const QLocale &locale() const { return m_locale; }

    void setVisible(bool visible);

    void setAnimationOptions(QChart::AnimationOptions options);
    QChart::AnimationOptions animationOptions() const;
    void setAnimationDuration(int msecs);
    int animationDuration() const { return m_animationDuration; }
    void setAnimationEasingCurve(const QEasingCurve &curve);
    QEasingCurve animationEasingCurve() const { return m_animationCurve; }

    void startAnimation(ChartAnimation *animation);

    void setState(State state,QPointF point);
    State state() const { return m_state; }
    QPointF statePoint() const { return m_statePoint; }
    AbstractChartLayout *layout();
    void setLayout(AbstractChartLayout* layout);

    QChart::ChartType chartType() const { return m_chart->chartType(); }
    QChart *chart() { return m_chart; }

    static QRectF textBoundingRect(const QFont &font, const QString &text, qreal angle = 0.0);
    static QString truncatedText(const QFont &font, const QString &text, qreal angle,
                                 qreal maxWidth, qreal maxHeight, QRectF &boundingRect);
    inline static qreal textMargin() { return qreal(0.5); }

    QString numberToString(double value, char f = 'g', int prec = 6);
    QString numberToString(int value);

    void updateGLWidget();
    void glSetUseWidget(bool enable) { m_glUseWidget = enable; }

private:
    void createBackgroundItem();
    void createPlotAreaBackgroundItem();
    void createTitleItem();

public Q_SLOTS:
    void handleSeriesAdded(QAbstractSeries *series);
    void handleSeriesRemoved(QAbstractSeries *series);
    void handleAxisAdded(QAbstractAxis *axis);
    void handleAxisRemoved(QAbstractAxis *axis);

Q_SIGNALS:
    void plotAreaChanged(const QRectF &plotArea);

private:
    QChart *m_chart;
    QList<ChartItem *> m_chartItems;
    QList<ChartAxisElement *> m_axisItems;
    QList<QAbstractSeries *> m_series;
    QList<QAbstractAxis *> m_axes;
    QChart::AnimationOptions m_options;
    int m_animationDuration;
    QEasingCurve m_animationCurve;
    State m_state;
    QPointF m_statePoint;
    AbstractChartLayout *m_layout;
    ChartBackground *m_background;
    QAbstractGraphicsShapeItem *m_plotAreaBackground;
    ChartTitle *m_title;
    QRectF m_rect;
    bool m_localizeNumbers;
    QLocale m_locale;
#ifndef QT_NO_OPENGL
    QPointer<GLWidget> m_glWidget;
#endif
    bool m_glUseWidget;
};

SCHARTS_END_NAMESPACE

#endif /* CHARTPRESENTER_H */
