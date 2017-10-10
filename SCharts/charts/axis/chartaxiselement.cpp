#include <chartaxiselement_p.h>
#include <qabstractaxis_p.h>
#include <chartpresenter_p.h>
#include <abstractchartlayout_p.h>
#include <../categoryaxis/qcategoryaxis.h>
#include <QtCore/QtMath>
#include <QtCore/QDateTime>
#include <QtGui/QTextDocument>
#include <cmath>

SCHARTS_BEGIN_NAMESPACE

static const char *labelFormatMatchString = "%[\\-\\+#\\s\\d\\.\\'lhjztL]*([dicuoxfegXFEG])";
static const char *labelFormatMatchLocalizedString = "^([^%]*)%\\.(\\d+)([defgiEG])(.*)$";
static QRegExp *labelFormatMatcher = 0;
static QRegExp *labelFormatMatcherLocalized = 0;
class StaticLabelFormatMatcherDeleter
{
public:
    StaticLabelFormatMatcherDeleter() {}
    ~StaticLabelFormatMatcherDeleter() {
        delete labelFormatMatcher;
        delete labelFormatMatcherLocalized;
    }
};
static StaticLabelFormatMatcherDeleter staticLabelFormatMatcherDeleter;

ChartAxisElement::ChartAxisElement(QAbstractAxis *axis, QGraphicsItem *item, bool intervalAxis)
    : ChartElement(item),
      m_axis(axis),
      m_animation(0),
      m_grid(new QGraphicsItemGroup(item)),
      m_arrow(new QGraphicsItemGroup(item)),
      m_minorGrid(new QGraphicsItemGroup(item)),
      m_minorArrow(new QGraphicsItemGroup(item)),
      m_shades(new QGraphicsItemGroup(item)),
      m_labels(new QGraphicsItemGroup(item)),
      m_title(new QGraphicsTextItem(item)),
      m_intervalAxis(intervalAxis)

{
    // 最初的初始化工作
    m_arrow->setHandlesChildEvents(false);
    m_arrow->setZValue(ChartPresenter::AxisZValue);
    m_minorArrow->setHandlesChildEvents(false);
    m_minorArrow->setZValue(ChartPresenter::AxisZValue);
    m_labels->setZValue(ChartPresenter::AxisZValue);
    m_shades->setZValue(ChartPresenter::ShadesZValue);
    m_grid->setZValue(ChartPresenter::GridZValue);
    m_minorGrid->setZValue(ChartPresenter::GridZValue);
    m_title->setZValue(ChartPresenter::GridZValue);
    m_title->document()->setDocumentMargin(ChartPresenter::textMargin());
    handleVisibleChanged(axis->isVisible());
    connectSlots();

    setFlag(QGraphicsItem::ItemHasNoContents, true);
}

ChartAxisElement::~ChartAxisElement()
{
}

void ChartAxisElement::connectSlots()
{
    QObject::connect(axis(), SIGNAL(visibleChanged(bool)), this, SLOT(handleVisibleChanged(bool)));
    QObject::connect(axis(), SIGNAL(lineVisibleChanged(bool)), this, SLOT(handleArrowVisibleChanged(bool)));
    QObject::connect(axis(), SIGNAL(gridVisibleChanged(bool)), this, SLOT(handleGridVisibleChanged(bool)));
    QObject::connect(axis(), SIGNAL(labelsVisibleChanged(bool)), this, SLOT(handleLabelsVisibleChanged(bool)));
    QObject::connect(axis(), SIGNAL(shadesVisibleChanged(bool)), this, SLOT(handleShadesVisibleChanged(bool)));
    QObject::connect(axis(), SIGNAL(labelsAngleChanged(int)), this, SLOT(handleLabelsAngleChanged(int)));
    QObject::connect(axis(), SIGNAL(linePenChanged(const QPen&)), this, SLOT(handleArrowPenChanged(const QPen&)));
    QObject::connect(axis(), SIGNAL(labelsBrushChanged(const QBrush&)), this, SLOT(handleLabelsBrushChanged(const QBrush&)));
    QObject::connect(axis(), SIGNAL(labelsFontChanged(const QFont&)), this, SLOT(handleLabelsFontChanged(const QFont&)));
    QObject::connect(axis(), SIGNAL(gridLinePenChanged(const QPen&)), this, SLOT(handleGridPenChanged(const QPen&)));
    QObject::connect(axis(), SIGNAL(shadesPenChanged(const QPen&)), this, SLOT(handleShadesPenChanged(const QPen&)));
    QObject::connect(axis(), SIGNAL(shadesBrushChanged(const QBrush&)), this, SLOT(handleShadesBrushChanged(const QBrush&)));
    QObject::connect(axis(), SIGNAL(titleTextChanged(const QString&)), this, SLOT(handleTitleTextChanged(const QString&)));
    QObject::connect(axis(), SIGNAL(titleFontChanged(const QFont&)), this, SLOT(handleTitleFontChanged(const QFont&)));
    QObject::connect(axis(), SIGNAL(titleBrushChanged(const QBrush&)), this, SLOT(handleTitleBrushChanged(const QBrush&)));
    QObject::connect(axis(), SIGNAL(titleVisibleChanged(bool)), this, SLOT(handleTitleVisibleChanged(bool)));
    QObject::connect(axis()->d_ptr.data(), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(handleRangeChanged(qreal, qreal)));
    QObject::connect(axis(), SIGNAL(reverseChanged(bool)), this, SLOT(handleReverseChanged(bool)));
    QObject::connect(axis(), SIGNAL(lineVisibleChanged(bool)),
                     this, SLOT(handleMinorArrowVisibleChanged(bool)));
    QObject::connect(axis(), SIGNAL(linePenChanged(const QPen&)), this,
                     SLOT(handleMinorArrowPenChanged(const QPen&)));
    QObject::connect(axis(), SIGNAL(minorGridVisibleChanged(bool)),
                     this, SLOT(handleMinorGridVisibleChanged(bool)));
    QObject::connect(axis(), SIGNAL(minorGridLinePenChanged(const QPen&)),
                     this, SLOT(handleMinorGridPenChanged(const QPen&)));
    QObject::connect(axis(), SIGNAL(gridLineColorChanged(const QColor&)),
                     this, SLOT(handleGridLineColorChanged(const QColor&)));
    QObject::connect(axis(), SIGNAL(minorGridLineColorChanged(const QColor&)),
                     this, SLOT(handleMinorGridLineColorChanged(const QColor&)));

    if (axis()->type() == QAbstractAxis::AxisTypeCategory) {
        QCategoryAxis *categoryAxis = static_cast<QCategoryAxis *>(axis());
        QObject::connect(categoryAxis,
                         SIGNAL(labelsPositionChanged(QCategoryAxis::AxisLabelsPosition)),
                         this, SLOT(handleLabelsPositionChanged()));
    }
}

void ChartAxisElement::handleArrowVisibleChanged(bool visible)
{
    m_arrow->setVisible(visible);
}

void ChartAxisElement::handleMinorArrowVisibleChanged(bool visible)
{
    m_minorArrow->setVisible(visible);
}

void ChartAxisElement::handleGridVisibleChanged(bool visible)
{
    m_grid->setVisible(visible);
}

void ChartAxisElement::handleMinorGridVisibleChanged(bool visible)
{
    m_minorGrid->setVisible(visible);
}

void ChartAxisElement::handleLabelsPositionChanged()
{
    QGraphicsLayoutItem::updateGeometry();
    presenter()->layout()->invalidate();
}

void ChartAxisElement::handleLabelsVisibleChanged(bool visible)
{
    QGraphicsLayoutItem::updateGeometry();
    presenter()->layout()->invalidate();
    m_labels->setVisible(visible);
}

void ChartAxisElement::handleShadesVisibleChanged(bool visible)
{
    m_shades->setVisible(visible);
}

void ChartAxisElement::handleTitleVisibleChanged(bool visible)
{
    QGraphicsLayoutItem::updateGeometry();
    presenter()->layout()->invalidate();
    m_title->setVisible(visible);
}

void ChartAxisElement::handleLabelsAngleChanged(int angle)
{
    foreach (QGraphicsItem *item, m_labels->childItems())
        item->setRotation(angle);

    QGraphicsLayoutItem::updateGeometry();
    presenter()->layout()->invalidate();
}

void ChartAxisElement::handleLabelsBrushChanged(const QBrush &brush)
{
    foreach (QGraphicsItem *item, m_labels->childItems())
        static_cast<QGraphicsTextItem *>(item)->setDefaultTextColor(brush.color());
}

void ChartAxisElement::handleLabelsFontChanged(const QFont &font)
{
    foreach (QGraphicsItem *item, m_labels->childItems())
        static_cast<QGraphicsTextItem *>(item)->setFont(font);
    QGraphicsLayoutItem::updateGeometry();
    presenter()->layout()->invalidate();
}

void ChartAxisElement::handleTitleTextChanged(const QString &title)
{
    QGraphicsLayoutItem::updateGeometry();
    presenter()->layout()->invalidate();
    if (title.isEmpty() || !m_title->isVisible())
        m_title->setHtml(title);
}

void ChartAxisElement::handleTitleBrushChanged(const QBrush &brush)
{
    m_title->setDefaultTextColor(brush.color());
}

void ChartAxisElement::handleTitleFontChanged(const QFont &font)
{
    if (m_title->font() != font) {
        m_title->setFont(font);
        QGraphicsLayoutItem::updateGeometry();
        presenter()->layout()->invalidate();
    }
}

/*! \brief 可见性变化处理函数：调整组件的可见性并更新布局。*/
void ChartAxisElement::handleVisibleChanged(bool visible)
{
    setVisible(visible);
    if (!visible) {
        // 当不可见时，隐藏所有组件。
        m_grid->setVisible(visible);
        m_arrow->setVisible(visible);
        m_minorGrid->setVisible(visible);
        m_minorArrow->setVisible(visible);
        m_shades->setVisible(visible);
        m_labels->setVisible(visible);
        m_title->setVisible(visible);
    } else {
        // 当可见时，根据坐标设置显示设置为可见的组件。
        m_grid->setVisible(axis()->isGridLineVisible());
        m_arrow->setVisible(axis()->isLineVisible());
        m_minorGrid->setVisible(axis()->isMinorGridLineVisible());
        m_minorArrow->setVisible(axis()->isLineVisible());
        m_shades->setVisible(axis()->shadesVisible());
        m_labels->setVisible(axis()->labelsVisible());
        m_title->setVisible(axis()->isTitleVisible());
    }

    // 整个图表重新布局。
    if (presenter()) presenter()->layout()->invalidate();
}

/*! \brief 数值范围变化处理函数。*/
void ChartAxisElement::handleRangeChanged(qreal min, qreal max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);

    // 当坐标轴相关部分为空时，不需要处理数值范围变化。
    if (!isEmpty()) {
        QVector<qreal> layout = calculateLayout();
        updateLayout(layout);
        QSizeF before = effectiveSizeHint(Qt::PreferredSize);
        QSizeF after = sizeHint(Qt::PreferredSize);

        //! 如果显示区域不会变化，则不需要更新显示区域。
        if (before != after) {
            QGraphicsLayoutItem::updateGeometry();
            // 这里并不会对布局调用 QGraphicsLayout::invalidate()，因为这会改变组件的最小大小。我们并不希望出现这
            // 种情况，因为在滚动或者放缩时这将导致令人厌烦的跳动（flips）。所以，采用重新计算布局并且使用绘图区域
            // 作为额外的空间。
            presenter()->layout()->setGeometry(presenter()->layout()->geometry());
        }
    }
}

/*! \brief Reverse属性变化处理函数。*/
void ChartAxisElement::handleReverseChanged(bool reverse)
{
    Q_UNUSED(reverse);
    // 更新显示区域。
    QGraphicsLayoutItem::updateGeometry();
    // 重新布局。
    presenter()->layout()->invalidate();
}

/*! \return 如果坐标轴大小、坐标轴网格区域大小或数值范围大小近似为空则返回true。*/
bool ChartAxisElement::isEmpty()
{
    return axisGeometry().isEmpty()
           || gridGeometry().isEmpty()
           || qFuzzyCompare(min(), max());
}

/*! \return 坐标轴显示范围的最小值。*/
qreal ChartAxisElement::min() const
{
    return m_axis->d_ptr->min();
}

/*! \return 坐标轴显示范围的最大值。*/
qreal ChartAxisElement::max() const
{
    return m_axis->d_ptr->max();
}

/*! \brief 格式化刻度标签。*/
QString ChartAxisElement::formatLabel(const QString &formatSpec, const QByteArray &array,
                                      qreal value, int precision, const QString &preStr,
                                      const QString &postStr) const
{
    QString retVal;
    if (!formatSpec.isEmpty()) {
        if (formatSpec.at(0) == QLatin1Char('d')
            || formatSpec.at(0) == QLatin1Char('i')
            || formatSpec.at(0) == QLatin1Char('c')) {
            if (presenter()->localizeNumbers())
                retVal = preStr + presenter()->locale().toString(qint64(value)) + postStr;
            else
                retVal = QString().sprintf(array, qint64(value));
        } else if (formatSpec.at(0) == QLatin1Char('u')
                 || formatSpec.at(0) == QLatin1Char('o')
                 || formatSpec.at(0) == QLatin1Char('x')
                 || formatSpec.at(0) == QLatin1Char('X')) {
            // 本地化数字不支持这些格式。
            retVal = QString().sprintf(array, quint64(value));
        } else if (formatSpec.at(0) == QLatin1Char('f')
                   || formatSpec.at(0) == QLatin1Char('F')
                   || formatSpec.at(0) == QLatin1Char('e')
                   || formatSpec.at(0) == QLatin1Char('E')
                   || formatSpec.at(0) == QLatin1Char('g')
                   || formatSpec.at(0) == QLatin1Char('G')) {
            if (presenter()->localizeNumbers()) {
                retVal = preStr
                        + presenter()->locale().toString(value, formatSpec.at(0).toLatin1(),
                                                         precision)
                        + postStr;
            } else {
                retVal = QString().sprintf(array, value);
            }
        }
    }
    return retVal;
}

/*! \brief 根据指定格式创建普通线性数值刻度标签。*/
QStringList ChartAxisElement::createValueLabels(qreal min, qreal max, int ticks,
                                                const QString &format) const
{
    QStringList labels;

    if (max <= min || ticks < 1)
        return labels;

    // 根据指定格式转化数值。
    if (format.isNull()) {
        // 计算所需精度。
        int n = qMax(int(-qFloor(std::log10((max - min) / (ticks - 1)))), 0) + 1;
        for (int i = 0; i < ticks; i++) {
            qreal value = min + (i * (max - min) / (ticks - 1));
            labels << presenter()->numberToString(value, 'f', n);
        }
    } else {
        QByteArray array = format.toLatin1();
        QString formatSpec;
        QString preStr;
        QString postStr;
        int precision = 6; // Six is the default precision in Qt API
        if (presenter()->localizeNumbers()) {
            if (!labelFormatMatcherLocalized)
                labelFormatMatcherLocalized
                        = new QRegExp(QString::fromLatin1(labelFormatMatchLocalizedString));
            if (labelFormatMatcherLocalized->indexIn(format, 0) != -1) {
                preStr = labelFormatMatcherLocalized->cap(1);
                if (!labelFormatMatcherLocalized->cap(2).isEmpty())
                    precision = labelFormatMatcherLocalized->cap(2).toInt();
                formatSpec = labelFormatMatcherLocalized->cap(3);
                postStr = labelFormatMatcherLocalized->cap(4);
            }
        } else {
            if (!labelFormatMatcher)
                labelFormatMatcher = new QRegExp(QString::fromLatin1(labelFormatMatchString));
            if (labelFormatMatcher->indexIn(format, 0) != -1)
                formatSpec = labelFormatMatcher->cap(1);
        }
        for (int i = 0; i < ticks; i++) {
            qreal value = min + (i * (max - min) / (ticks - 1));
            labels << formatLabel(formatSpec, array, value, precision, preStr, postStr);
        }
    }

    return labels;
}

/*! \brief 根据指定格式创建对数数值刻度标签。*/
QStringList ChartAxisElement::createLogValueLabels(qreal min, qreal max, qreal base, int ticks,
                                                   const QString &format) const
{
    QStringList labels;

    if (max <= min || ticks < 1)
        return labels;

    int firstTick;
    if (base > 1)
        firstTick = qCeil(std::log10(min) / std::log10(base));
    else
        firstTick = qCeil(std::log10(max) / std::log10(base));

    if (format.isNull()) {
        int n = 0;
        if (ticks > 1)
            n = qMax(int(-qFloor(std::log10((max - min) / (ticks - 1)))), 0);
        n++;
        for (int i = firstTick; i < ticks + firstTick; i++) {
            qreal value = qPow(base, i);
            labels << presenter()->numberToString(value, 'f', n);
        }
    } else {
        QByteArray array = format.toLatin1();
        QString formatSpec;
        QString preStr;
        QString postStr;
        int precision = 6; // Six is the default precision in Qt API
        if (presenter()->localizeNumbers()) {
            if (!labelFormatMatcherLocalized)
                labelFormatMatcherLocalized =
                        new QRegExp(QString::fromLatin1(labelFormatMatchLocalizedString));
            if (labelFormatMatcherLocalized->indexIn(format, 0) != -1) {
                preStr = labelFormatMatcherLocalized->cap(1);
                if (!labelFormatMatcherLocalized->cap(2).isEmpty())
                    precision = labelFormatMatcherLocalized->cap(2).toInt();
                formatSpec = labelFormatMatcherLocalized->cap(3);
                postStr = labelFormatMatcherLocalized->cap(4);
            }
        } else {
            if (!labelFormatMatcher)
                labelFormatMatcher = new QRegExp(QString::fromLatin1(labelFormatMatchString));
            if (labelFormatMatcher->indexIn(format, 0) != -1)
                formatSpec = labelFormatMatcher->cap(1);
        }
        for (int i = firstTick; i < ticks + firstTick; i++) {
            qreal value = qPow(base, i);
            labels << formatLabel(formatSpec, array, value, precision, preStr, postStr);
        }
    }

    return labels;
}

/*! \brief 根据指定格式创建时间数值刻度标签。*/
QStringList ChartAxisElement::createDateTimeLabels(qreal min, qreal max,int ticks,
                                                   const QString &format) const
{
    QStringList labels;

    if (max <= min || ticks < 1)
        return labels;

    int n = qMax(int(-qFloor(std::log10((max - min) / (ticks - 1)))), 0);
    n++;
    for (int i = 0; i < ticks; i++) {
        qreal value = min + (i * (max - min) / (ticks - 1));
        labels << presenter()->locale().toString(QDateTime::fromMSecsSinceEpoch(value), format);
    }
    return labels;
}

void ChartAxisElement::axisSelected()
{
    emit clicked();
}

#include "moc_chartaxiselement_p.cpp"

SCHARTS_END_NAMESPACE
