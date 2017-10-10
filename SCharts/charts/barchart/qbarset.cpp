#include <QBarSet>
#include <qbarset_p.h>
#include <charthelpers_p.h>
#include <qchart_p.h>

SCHARTS_BEGIN_NAMESPACE

/*!
 * \class QBarSet
 * \inmodule Qt Charts
 * \brief 不同类型条形图的构建基石。
 *
 * QBarSet 表示了一组条状元素。条状元素组的集合包含了对应每个类别的一个数据值。集合中的第一个数值被假定为第一个类别，
 * 第二个数值对应第二个类别，等等。如果集合的数值数目少于类别数目，则缺少的数值假定位于集合的末尾。对于在集合中间缺失
 * 的数值，则认为其为数值0。没有数值的集合的标签不会显示。
 *
 * \sa QAbstractBarSeries, QBarSeries, QStackedBarSeries, QPercentBarSeries
*/
/*!
 * \qmltype BarSet
 * \instantiates QBarSet
 * \inqmlmodule QtCharts
 * \brief 不同类型条形图的构建基石。
 *
 * QBarSet 表示了一组条状元素。条状元素组的集合包含了对应每个类别的一个数据值。集合中的第一个数值被假定为第一个类别，
 * 第二个数值对应第二个类别，等等。如果集合的数值数目少于类别数目，则缺少的数值假定位于集合的末尾。对于在集合中间缺失
 * 的数值，则认为其为数值0。没有数值的集合的标签不会显示。
 *
 * \sa AbstractBarSeries, BarSeries, StackedBarSeries, PercentBarSeries
 */

/*!
 * \property QBarSet::label
 * 定义了条形元素集合的标签。
 */
/*!
 * \qmlproperty string BarSet::label
 * 定义了条形元素集合的标签。
 */

/*!
 * \property QBarSet::pen
 * \brief 定义了由条形元素集合使用的画笔。
 */

/*!
 * \property QBarSet::brush
 * \brief 定义了由条形元素集合使用的画刷（brush）。
 */

/*!
 * \qmlproperty QString BarSet::brushFilename
 * 用作条形元素集合画刷的文件名称。
 */

/*!
 * \property QBarSet::labelBrush
 * \brief 定义了条形元素集合标签使用的画刷。
 */

/*!
 * \property QBarSet::labelFont
 * \brief 定义了条形元素集合标签使用的字体。
 */

/*!
 * \qmlproperty Font BarSet::labelFont
 * 定义了条形元素集合标签使用的字体。
 */

/*!
 * \property QBarSet::color
 * 条形元素集合的填充（画刷）颜色。
 */
/*!
 * \qmlproperty color BarSet::color
 * 条形元素集合的填充（画刷）颜色。
 */

/*!
 * \property QBarSet::borderColor
 * 条形元素集合边界线（画笔）颜色。
 */
/*!
 * \qmlproperty color BarSet::borderColor
 * 条形元素集合边界线（画笔）颜色。
 */

/*!
 * \qmlproperty real BarSet::borderWidth
 * 边界线的宽度。默认宽度为2.0。
 */

/*!
 * \property QBarSet::labelColor
 * 条形元素集合的文字（标签）颜色。
 */
/*!
 * \qmlproperty color BarSet::labelColor
 * 条形元素集合的文字（标签）颜色。
 */

/*!
 * \fn void QBarSet::clicked(int index)
 *
 * 当用户在条形元素集合上面使用鼠标点击时，发出该信号。被点击的条形元素在集合中的位置由\a index 标识。
 */

/*!
 * \fn void QBarSet::pressed(int index)
 *
 * 当用户在条形元素集合上面按下鼠标时，发出该信号。被按下的条形元素在集合中的位置由\a index 标识。
 */

/*!
 * \fn void QBarSet::released(int index)
 *
 * 当用户在条形元素集合上面松开鼠标时，发出该信号。发生该事件的条形元素在集合中的位置由\a index 标识。
 */

/*!
 * \fn void QBarSet::doubleClicked(int index)
 *
 * 当用户在条形元素集合上面双击鼠标时，发出该信号。发生该事件的条形元素在集合中的位置由\a index 标识。
 */

/*!
 * \fn void QBarSet::hovered(bool status, int index)
 *
 * 当鼠标悬停在条形元素集合的上面时，发出该信号。
 * 当鼠标进入条形元素时，参数\a status 为true，而当鼠标离开则为false。
 * 发生该事件的条形元素在集合中的位置由\a index 标识。
 */

/*!
 * \fn void QBarSet::labelChanged()
 * 当条形元素集合的标签发生改变时，发出该信号。
 * \sa label
 */
/*!
 * \qmlsignal BarSet::onLabelChanged()
 * 当条形元素集合的标签发生改变时，发出该信号。
 */

/*!
 * \fn void QBarSet::penChanged()
 * 当条形元素集合的画笔发生改变时，发出该信号。
 * \sa pen
 */

/*!
 * \fn void QBarSet::brushChanged()
 * 当条形元素集合的画刷发生改变时，发出该信号。
 * \sa brush
 */

/*!
 * \fn void QBarSet::labelBrushChanged()
 * 当条形元素集合的标签的画刷发生改变时，发出该信号。
 * \sa labelBrush
 */

/*!
 * \fn void QBarSet::labelFontChanged()
 * 当条形元素集合的标签的字体发生改变时，发出该信号。
 * \sa labelFont
 */

/*!
 * \fn void QBarSet::colorChanged(QColor)
 * This signal is emitted when the fill (brush) color of the set has changed to \a color.
 */
/*!
 * \qmlsignal BarSet::onColorChanged(color color)
 * This signal is emitted when the fill (brush) color of the set has changed to \a color.
 */

/*!
 * \fn void QBarSet::borderColorChanged(QColor)
 * This signal is emitted when the line (pen) color of the set has changed to \a color.
 */
/*!
    \qmlsignal BarSet::onBorderColorChanged(color color)
    This signal is emitted when the line (pen) color of the set has changed to \a color.
*/

/*!
    \fn void QBarSet::labelColorChanged(QColor)
    This signal is emitted when the text (label) color of the set has changed to \a color.
*/
/*!
    \qmlsignal BarSet::onLabelColorChanged(color color)
    This signal is emitted when the text (label) color of the set has changed to \a color.
*/

/*!
    \fn void QBarSet::valuesAdded(int index, int count)
    This signal is emitted when new values have been added to the set.
    Parameter \a index indicates the position of the first inserted value.
    Parameter \a count is the number of inserted values.
    \sa append(), insert()
*/
/*!
    \qmlsignal BarSet::onValuesAdded(int index, int count)
    This signal is emitted when new values have been added to the set.
    Parameter \a index indicates the position of the first inserted value.
    Parameter \a count is the number of inserted values.
*/

/*!
    \fn void QBarSet::valuesRemoved(int index, int count)
    This signal is emitted values have been removed from the set.
    Parameter \a index indicates the position of the first removed value.
    Parameter \a count is the number of removed values.
    \sa remove()
*/
/*!
    \qmlsignal BarSet::onValuesRemoved(int index, int count)
    This signal is emitted values have been removed from the set.
    Parameter \a index indicates the position of the first removed value.
    Parameter \a count is the number of removed values.
*/

/*!
    \fn void QBarSet::valueChanged(int index)
    This signal is emitted values the value in the set has been modified.
    Parameter \a index indicates the position of the modified value.
    \sa at()
*/
/*!
    \qmlsignal BarSet::onValueChanged(int index)
    This signal is emitted values the value in the set has been modified.
    Parameter \a index indicates the position of the modified value.
*/

/*!
    \qmlproperty int BarSet::count
    The count of values on the bar set
*/

/*!
    \qmlproperty QVariantList BarSet::values
    The values of the bar set. You can set either a list of reals or a list of points as values. If you set a list of
    reals as values, the values are automatically completed to points by using the index of a value as it's
    x-coordinate. For example the following sets have equal values:
    \code
        myBarSet1.values = [0, 5, 1, 5];
        myBarSet2.values = [Qt.point(0, 0), Qt.point(1, 5), Qt.point(2, 1), Qt.point(3, 5)];
    \endcode
*/

/*!
 * Constructs QBarSet with a label of \a label and with parent of \a parent.
 */
QBarSet::QBarSet(const QString label, QObject *parent)
    : QObject(parent),
      d_ptr(new QBarSetPrivate(label, this))
{
}

/*!
    Destroys the bar set.
*/
QBarSet::~QBarSet()
{
    // NOTE: d_ptr destroyed by QObject
}

/*!
    Sets new \a label for set.
*/
void QBarSet::setLabel(const QString label)
{
    d_ptr->m_label = label;
    emit labelChanged();
}

/*!
    Returns label of the set.
*/
QString QBarSet::label() const
{
    return d_ptr->m_label;
}

/*!
    Appends new value \a value to the end of set.
*/
void QBarSet::append(const qreal value)
{
    // Convert to QPointF
    int index = d_ptr->m_values.count();
    d_ptr->append(QPointF(d_ptr->m_values.count(), value));
    emit valuesAdded(index, 1);
}

/*!
    Appends a list of reals to set. Works like append with single real value. The \a values in list
    are appended to end of bar set.
    \sa append()
*/
void QBarSet::append(const QList<qreal> &values)
{
    int index = d_ptr->m_values.count();
    d_ptr->append(values);
    emit valuesAdded(index, values.count());
}

/*!
    Convenience operator. Same as append, with real \a value.
    \sa append()
*/
QBarSet &QBarSet::operator << (const qreal &value)
{
    append(value);
    return *this;
}

/*!
    Inserts new \a value on the \a index position.
    The value that is currently at this postion is moved to position index + 1
    \sa remove()
*/
void QBarSet::insert(const int index, const qreal value)
{
    d_ptr->insert(index, value);
    emit valuesAdded(index, 1);
}

/*!
    Removes \a count number of values from the set starting at \a index.
    \sa insert()
*/
void QBarSet::remove(const int index, const int count)
{
    int removedCount = d_ptr->remove(index, count);
    if (removedCount > 0)
        emit valuesRemoved(index, removedCount);
    return;
}

/*!
    Sets a new value \a value to set, indexed by \a index.
*/
void QBarSet::replace(const int index, const qreal value)
{
    if (index >= 0 && index < d_ptr->m_values.count()) {
        d_ptr->replace(index, value);
        emit valueChanged(index);
    }
}


/*!
    Returns value of set indexed by \a index.
    If the index is out of bounds 0.0 is returned.
*/
qreal QBarSet::at(const int index) const
{
    if (index < 0 || index >= d_ptr->m_values.count())
        return 0;
    return d_ptr->m_values.at(index).y();
}

/*!
    Returns value of set indexed by \a index.
    If the index is out of bounds 0.0 is returned.
*/
qreal QBarSet::operator [](const int index) const
{
    return at(index);
}

/*!
    Returns count of values in set.
*/
int QBarSet::count() const
{
    return d_ptr->m_values.count();
}

/*!
    Returns sum of all values in the bar set.
*/
qreal QBarSet::sum() const
{
    qreal total(0);
    for (int i = 0; i < d_ptr->m_values.count(); i++)
        total += d_ptr->m_values.at(i).y();
    return total;
}

/*!
    Sets pen for set. Bars of this set are drawn using \a pen
*/
void QBarSet::setPen(const QPen &pen)
{
    if (d_ptr->m_pen != pen) {
        d_ptr->m_pen = pen;
        emit d_ptr->updatedBars();
        emit penChanged();
    }
}

/*!
    Returns pen of the set.
*/
QPen QBarSet::pen() const
{
    if (d_ptr->m_pen == QChartPrivate::defaultPen())
        return QPen();
    else
        return d_ptr->m_pen;
}

/*!
    Sets brush for the set. Bars of this set are drawn using \a brush.
*/
void QBarSet::setBrush(const QBrush &brush)
{
    if (d_ptr->m_brush != brush) {
        d_ptr->m_brush = brush;
        emit d_ptr->updatedBars();
        emit brushChanged();
    }
}

/*!
    Returns brush of the set.
*/
QBrush QBarSet::brush() const
{
    if (d_ptr->m_brush == QChartPrivate::defaultBrush())
        return QBrush();
    else
        return d_ptr->m_brush;
}

/*!
    Sets \a brush of the values that are drawn on top of this bar set.
*/
void QBarSet::setLabelBrush(const QBrush &brush)
{
    if (d_ptr->m_labelBrush != brush) {
        d_ptr->m_labelBrush = brush;
        emit d_ptr->updatedBars();
        emit labelBrushChanged();
    }
}

/*!
    Returns brush of the values that are drawn on top of this bar set.
*/
QBrush QBarSet::labelBrush() const
{
    if (d_ptr->m_labelBrush == QChartPrivate::defaultBrush())
        return QBrush();
    else
        return d_ptr->m_labelBrush;
}

/*!
    Sets the \a font for values that are drawn on top of this bar set.
*/
void QBarSet::setLabelFont(const QFont &font)
{
    if (d_ptr->m_labelFont != font) {
        d_ptr->m_labelFont = font;
        emit d_ptr->updatedBars();
        emit labelFontChanged();
    }

}

/*!
    Returns the pen for values that are drawn on top of this bar set.
*/
QFont QBarSet::labelFont() const
{
    return d_ptr->m_labelFont;
}

/*!
    Returns the color of the brush of bar set.
*/
QColor QBarSet::color()
{
    return brush().color();
}

/*!
    Sets the \a color of brush for this bar set.
*/
void QBarSet::setColor(QColor color)
{
    QBrush b = brush();
    if ((b.color() != color) || (b.style() == Qt::NoBrush)) {
        b.setColor(color);
        if (b.style() == Qt::NoBrush) {
            // Set tyle to Qt::SolidPattern. (Default is Qt::NoBrush)
            // This prevents theme to override color defined in QML side:
            // BarSet { label: "Bob"; color:"red"; values: [1,2,3] }
            // The color must be obeyed, since user wanted it.
            b.setStyle(Qt::SolidPattern);
        }
        setBrush(b);
        emit colorChanged(color);
    }
}

/*!
    Returns the color of pen of this bar set.
*/
QColor QBarSet::borderColor()
{
    return pen().color();
}

/*!
    Sets the \a color of pen for this bar set.
*/
void QBarSet::setBorderColor(QColor color)
{
    QPen p = pen();
    if (p.color() != color) {
        p.setColor(color);
        setPen(p);
        emit borderColorChanged(color);
    }
}

/*!
    Returns the color of labels of this bar set.
*/
QColor QBarSet::labelColor()
{
    return labelBrush().color();
}

/*!
    Sets the \a color of labels for this bar set.
*/
void QBarSet::setLabelColor(QColor color)
{
    QBrush b = labelBrush();
    if (b == QBrush())
        b.setStyle(Qt::SolidPattern);

    if (d_ptr->m_labelBrush.color() != color) {
        b.setColor(color);
        setLabelBrush(b);
        emit labelColorChanged(color);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QBarSetPrivate::QBarSetPrivate(const QString label, QBarSet *parent) : QObject(parent),
    q_ptr(parent),
    m_label(label),
    m_pen(QChartPrivate::defaultPen()),
    m_brush(QChartPrivate::defaultBrush()),
    m_labelBrush(QChartPrivate::defaultBrush())
{
}

QBarSetPrivate::~QBarSetPrivate()
{
}

void QBarSetPrivate::append(QPointF value)
{
    if (isValidValue(value)) {
        m_values.append(value);
        emit restructuredBars();
    }
}

void QBarSetPrivate::append(QList<QPointF> values)
{
    for (int i = 0; i < values.count(); i++) {
        if (isValidValue(values.at(i)))
            m_values.append(values.at(i));
    }
    emit restructuredBars();
}

void QBarSetPrivate::append(QList<qreal> values)
{
    int index = m_values.count();
    for (int i = 0; i < values.count(); i++) {
        if (isValidValue(values.at(i))) {
            m_values.append(QPointF(index, values.at(i)));
            index++;
        }
    }
    emit restructuredBars();
}

void QBarSetPrivate::insert(const int index, const qreal value)
{
    m_values.insert(index, QPointF(index, value));
    emit restructuredBars();
}

void QBarSetPrivate::insert(const int index, const QPointF value)
{
    m_values.insert(index, value);
    emit restructuredBars();
}

int QBarSetPrivate::remove(const int index, const int count)
{
    int removeCount = count;

    if ((index < 0) || (m_values.count() == 0))
        return 0; // Invalid index or not values in list, remove nothing.
    else if ((index + count) > m_values.count())
        removeCount = m_values.count() - index; // Trying to remove more items than list has. Limit amount to be removed.

    int c = 0;
    while (c < removeCount) {
        m_values.removeAt(index);
        c++;
    }
    emit restructuredBars();
    return removeCount;
}

void QBarSetPrivate::replace(const int index, const qreal value)
{
    m_values.replace(index, QPointF(index, value));
    emit updatedLayout();
}

void QBarSetPrivate::replace(const int index, const QPointF value)
{
    m_values.replace(index, value);
    emit updatedLayout();
}

qreal QBarSetPrivate::pos(const int index)
{
    if (index < 0 || index >= m_values.count())
        return 0;
    return m_values.at(index).x();
}

qreal QBarSetPrivate::value(const int index)
{
    if (index < 0 || index >= m_values.count())
        return 0;
    return m_values.at(index).y();
}

#include "moc_qbarset.cpp"
#include "moc_qbarset_p.cpp"

SCHARTS_END_NAMESPACE
