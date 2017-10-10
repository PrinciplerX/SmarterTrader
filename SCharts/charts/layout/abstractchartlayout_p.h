#ifndef ABSTRACTCHARTLAYOUT_H
#define ABSTRACTCHARTLAYOUT_H

#include <QtWidgets/QGraphicsLayout>
#include <QtCore/QMargins>
#include <SChartGlobal>

SCHARTS_BEGIN_NAMESPACE

class ChartTitle;
class ChartAxisElement;
class ChartPresenter;
class QLegend;
class ChartBackground;

class SCHARTS_EXPORT AbstractChartLayout : public QGraphicsLayout
{
public:
    AbstractChartLayout(ChartPresenter *presenter);
    virtual ~AbstractChartLayout();

    virtual void setMargins(const QMargins &margins);
    virtual QMargins margins() const;
    virtual void setGeometry(const QRectF &rect);

protected:
    virtual QRectF calculateBackgroundGeometry(const QRectF &geometry, ChartBackground *background) const;
    virtual QRectF calculateBackgroundMinimum(const QRectF &minimum) const;
    virtual QRectF calculateContentGeometry(const QRectF &geometry) const;
    virtual QRectF calculateContentMinimum(const QRectF &minimum) const;
    virtual QRectF calculateTitleGeometry(const QRectF &geometry, ChartTitle *title) const;
    virtual QRectF calculateTitleMinimum(const QRectF &minimum, ChartTitle *title) const;
    virtual QRectF calculateLegendGeometry(const QRectF &geometry, QLegend *legend) const;
    virtual QRectF calculateLegendMinimum(const QRectF &minimum, QLegend *legend) const;

    virtual QRectF calculateAxisGeometry(const QRectF &geometry, const QList<ChartAxisElement *>& axes) const = 0;
    virtual QRectF calculateAxisMinimum(const QRectF &minimum, const QList<ChartAxisElement *>& axes) const = 0;

    // from QGraphicsLayout
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    int count() const { return 0; }
    QGraphicsLayoutItem *itemAt(int) const { return 0; }
    void removeAt(int) {}

    ChartPresenter *m_presenter;

    //! 背景和内容之间的margin。
    //! \note 注意，该margin和布局的margin（使用getContentMargins()获得）的含义不同。
    QMargins m_margins;
    QRectF m_minChartRect;
    QRectF m_minAxisRect;
};

SCHARTS_END_NAMESPACE

#endif // ABSTRACTCHARTLAYOUT_H
