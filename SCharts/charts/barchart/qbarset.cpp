#include <QBarSet>
#include <qbarset_p.h>
#include <charthelpers_p.h>
#include <qchart_p.h>

SCHARTS_BEGIN_NAMESPACE

/*!
 * \class QBarSet
 * \inmodule Qt Charts
 * \brief ��ͬ��������ͼ�Ĺ�����ʯ��
 *
 * QBarSet ��ʾ��һ����״Ԫ�ء���״Ԫ����ļ��ϰ����˶�Ӧÿ������һ������ֵ�������еĵ�һ����ֵ���ٶ�Ϊ��һ�����
 * �ڶ�����ֵ��Ӧ�ڶ�����𣬵ȵȡ�������ϵ���ֵ��Ŀ���������Ŀ����ȱ�ٵ���ֵ�ٶ�λ�ڼ��ϵ�ĩβ�������ڼ����м�ȱʧ
 * ����ֵ������Ϊ��Ϊ��ֵ0��û����ֵ�ļ��ϵı�ǩ������ʾ��
 *
 * \sa QAbstractBarSeries, QBarSeries, QStackedBarSeries, QPercentBarSeries
*/
/*!
 * \qmltype BarSet
 * \instantiates QBarSet
 * \inqmlmodule QtCharts
 * \brief ��ͬ��������ͼ�Ĺ�����ʯ��
 *
 * QBarSet ��ʾ��һ����״Ԫ�ء���״Ԫ����ļ��ϰ����˶�Ӧÿ������һ������ֵ�������еĵ�һ����ֵ���ٶ�Ϊ��һ�����
 * �ڶ�����ֵ��Ӧ�ڶ�����𣬵ȵȡ�������ϵ���ֵ��Ŀ���������Ŀ����ȱ�ٵ���ֵ�ٶ�λ�ڼ��ϵ�ĩβ�������ڼ����м�ȱʧ
 * ����ֵ������Ϊ��Ϊ��ֵ0��û����ֵ�ļ��ϵı�ǩ������ʾ��
 *
 * \sa AbstractBarSeries, BarSeries, StackedBarSeries, PercentBarSeries
 */

/*!
 * \property QBarSet::label
 * ����������Ԫ�ؼ��ϵı�ǩ��
 */
/*!
 * \qmlproperty string BarSet::label
 * ����������Ԫ�ؼ��ϵı�ǩ��
 */

/*!
 * \property QBarSet::pen
 * \brief ������������Ԫ�ؼ���ʹ�õĻ��ʡ�
 */

/*!
 * \property QBarSet::brush
 * \brief ������������Ԫ�ؼ���ʹ�õĻ�ˢ��brush����
 */

/*!
 * \qmlproperty QString BarSet::brushFilename
 * ��������Ԫ�ؼ��ϻ�ˢ���ļ����ơ�
 */

/*!
 * \property QBarSet::labelBrush
 * \brief ����������Ԫ�ؼ��ϱ�ǩʹ�õĻ�ˢ��
 */

/*!
 * \property QBarSet::labelFont
 * \brief ����������Ԫ�ؼ��ϱ�ǩʹ�õ����塣
 */

/*!
 * \qmlproperty Font BarSet::labelFont
 * ����������Ԫ�ؼ��ϱ�ǩʹ�õ����塣
 */

/*!
 * \property QBarSet::color
 * ����Ԫ�ؼ��ϵ���䣨��ˢ����ɫ��
 */
/*!
 * \qmlproperty color BarSet::color
 * ����Ԫ�ؼ��ϵ���䣨��ˢ����ɫ��
 */

/*!
 * \property QBarSet::borderColor
 * ����Ԫ�ؼ��ϱ߽��ߣ����ʣ���ɫ��
 */
/*!
 * \qmlproperty color BarSet::borderColor
 * ����Ԫ�ؼ��ϱ߽��ߣ����ʣ���ɫ��
 */

/*!
 * \qmlproperty real BarSet::borderWidth
 * �߽��ߵĿ�ȡ�Ĭ�Ͽ��Ϊ2.0��
 */

/*!
 * \property QBarSet::labelColor
 * ����Ԫ�ؼ��ϵ����֣���ǩ����ɫ��
 */
/*!
 * \qmlproperty color BarSet::labelColor
 * ����Ԫ�ؼ��ϵ����֣���ǩ����ɫ��
 */

/*!
 * \fn void QBarSet::clicked(int index)
 *
 * ���û�������Ԫ�ؼ�������ʹ�������ʱ���������źš������������Ԫ���ڼ����е�λ����\a index ��ʶ��
 */

/*!
 * \fn void QBarSet::pressed(int index)
 *
 * ���û�������Ԫ�ؼ������水�����ʱ���������źš������µ�����Ԫ���ڼ����е�λ����\a index ��ʶ��
 */

/*!
 * \fn void QBarSet::released(int index)
 *
 * ���û�������Ԫ�ؼ��������ɿ����ʱ���������źš��������¼�������Ԫ���ڼ����е�λ����\a index ��ʶ��
 */

/*!
 * \fn void QBarSet::doubleClicked(int index)
 *
 * ���û�������Ԫ�ؼ�������˫�����ʱ���������źš��������¼�������Ԫ���ڼ����е�λ����\a index ��ʶ��
 */

/*!
 * \fn void QBarSet::hovered(bool status, int index)
 *
 * �������ͣ������Ԫ�ؼ��ϵ�����ʱ���������źš�
 * ������������Ԫ��ʱ������\a status Ϊtrue����������뿪��Ϊfalse��
 * �������¼�������Ԫ���ڼ����е�λ����\a index ��ʶ��
 */

/*!
 * \fn void QBarSet::labelChanged()
 * ������Ԫ�ؼ��ϵı�ǩ�����ı�ʱ���������źš�
 * \sa label
 */
/*!
 * \qmlsignal BarSet::onLabelChanged()
 * ������Ԫ�ؼ��ϵı�ǩ�����ı�ʱ���������źš�
 */

/*!
 * \fn void QBarSet::penChanged()
 * ������Ԫ�ؼ��ϵĻ��ʷ����ı�ʱ���������źš�
 * \sa pen
 */

/*!
 * \fn void QBarSet::brushChanged()
 * ������Ԫ�ؼ��ϵĻ�ˢ�����ı�ʱ���������źš�
 * \sa brush
 */

/*!
 * \fn void QBarSet::labelBrushChanged()
 * ������Ԫ�ؼ��ϵı�ǩ�Ļ�ˢ�����ı�ʱ���������źš�
 * \sa labelBrush
 */

/*!
 * \fn void QBarSet::labelFontChanged()
 * ������Ԫ�ؼ��ϵı�ǩ�����巢���ı�ʱ���������źš�
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
