#include <polarchartlayout_p.h>
#include <chartpresenter_p.h>
#include <polarchartaxis_p.h>
#include <QtCore/QDebug>

SCHARTS_BEGIN_NAMESPACE

static const qreal golden_ratio = 0.4;

PolarChartLayout::PolarChartLayout(ChartPresenter *presenter)
    : AbstractChartLayout(presenter)
{
}

PolarChartLayout::~PolarChartLayout()
{
}

QRectF PolarChartLayout::calculateAxisGeometry(const QRectF &geometry, const QList<ChartAxisElement *> &axes) const
{
    // How to handle multiple angular/radial axes?
    qreal axisRadius = geometry.height() / 2.0;
    if (geometry.width() < geometry.height())
        axisRadius = geometry.width() / 2.0;

    int titleHeight = 0;
    foreach (ChartAxisElement *chartAxis, axes) {
        if (!chartAxis->isVisible())
            continue;

        PolarChartAxis *polarChartAxis = static_cast<PolarChartAxis *>(chartAxis);
        qreal radius = polarChartAxis->preferredAxisRadius(geometry.size());
        if (radius < axisRadius)
            axisRadius = radius;

        if (chartAxis->axis()->orientation() == Qt::Horizontal
            && chartAxis->axis()->isTitleVisible()
            && !chartAxis->axis()->titleText().isEmpty()) {
            // If axis has angular title, adjust geometry down by the space title takes
            QRectF dummyRect = ChartPresenter::textBoundingRect(chartAxis->axis()->titleFont(), chartAxis->axis()->titleText());
            titleHeight = (dummyRect.height() / 2.0) + chartAxis->titlePadding();
        }
    }

    QRectF axisRect;
    axisRect.setSize(QSizeF(axisRadius * 2.0, axisRadius * 2.0));
    axisRect.moveCenter(geometry.center());
    axisRect.adjust(0, titleHeight, 0, titleHeight);

    foreach (ChartAxisElement *chartAxis, axes)
        chartAxis->setGeometry(axisRect, QRectF());

    return axisRect;
}

QRectF PolarChartLayout::calculateAxisMinimum(const QRectF &minimum, const QList<ChartAxisElement *> &axes) const
{
    Q_UNUSED(axes);
    return minimum;
}

SCHARTS_END_NAMESPACE
