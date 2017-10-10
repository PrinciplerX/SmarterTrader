#include "candlestickset.h"
#include <charthelpers_p.h>

/*!
    Constructs QBoxSet with optional \a label and parent of \a parent
*/
CandleStickSet::CandleStickSet(const QString label, QObject *parent)
    : QObject(parent),
      d_ptr(new CandleStickSetPrivate(label, this))
{
}

/*!
    Constructs QBoxSet with given ordered values. \a le for lower extreme, \a lq for lower quartile, \a m for median,
    \a uq for upper quartile and \a ue for upper quartile. \a label and \a parent are optional.
 */
CandleStickSet::CandleStickSet(const qreal le, const qreal lq, const qreal uq, const qreal ue, const QString label, QObject *parent)
    : QObject(parent),
      d_ptr(new CandleStickSetPrivate(label, this))
{
    d_ptr->append(le);
    d_ptr->append(lq);
    d_ptr->append(uq);
    d_ptr->append(ue);
}

/*!
    Destroys the boxset
*/
CandleStickSet::~CandleStickSet()
{
}

/*!
    Appends new value \a value to the end of set.
*/
void CandleStickSet::append(const qreal value)
{
    if (d_ptr->append(value))
        emit valueChanged(d_ptr->m_appendCount - 1);
}

/*!
    Appends a list of reals to set. Works like append with single real value. The \a values in list
    are appended to end of boxset.
    \sa append()
*/
void CandleStickSet::append(const QList<qreal> &values)
{
    if (d_ptr->append(values))
        emit valuesChanged();
}

/*!
    Sets new \a label for the category of the set.
*/
void CandleStickSet::setLabel(const QString label)
{
    d_ptr->m_label = label;
}

/*!
    Returns the label of the the category of the set.
*/
QString CandleStickSet::label() const
{
    return d_ptr->m_label;
}

/*!
    Convenience operator. Same as append, with real \a value.
    \sa append()
*/
CandleStickSet &CandleStickSet::operator << (const qreal &value)
{
    append(value);
    return *this;
}

/*!
    Sets a new \a value on the \a index position. For \a index ValuePositions can be used.
*/
void CandleStickSet::setValue(const int index, const qreal value)
{
    d_ptr->setValue(index, value);
    emit valueChanged(index);
}

/*!
    Sets all values on the set to 0.
 */
void CandleStickSet::clear()
{
    d_ptr->clear();
    emit cleared();
}

/*!
    Returns value of set indexed by \a index. For \a index ValuePositions can be used.
    If the index is out of bounds 0.0 is returned.
*/
qreal CandleStickSet::at(const int index) const
{
    if (index < 0 || index >= 4)
        return 0;
    return d_ptr->m_values[index];
}

/*!
    Returns value of set indexed by \a index. For \a index ValuePositions can be used.
    If the index is out of bounds 0.0 is returned.
*/
qreal CandleStickSet::operator [](const int index) const
{
    return at(index);
}

/*!
    Returns count of values appended to the set.
*/
int CandleStickSet::count() const
{
    return d_ptr->m_appendCount;
}

/*!
    Sets pen for set. Boxes of this set are drawn using \a pen
*/
void CandleStickSet::setPen(const QPen &pen)
{
    if (d_ptr->m_pen != pen) {
        d_ptr->m_pen = pen;
        emit d_ptr->updatedBox();
        emit penChanged();
    }
}

/*!
    Returns pen of the set.
*/
QPen CandleStickSet::pen() const
{
    return d_ptr->m_pen;
}

/*!
    Sets brush for the set. Boxes of this set are drawn using \a brush
*/
void CandleStickSet::setBrush(const QBrush &brush)
{
    if (d_ptr->m_brush != brush) {
        d_ptr->m_brush = brush;
        emit d_ptr->updatedBox();
        emit brushChanged();
    }
}

/*!
    Returns brush of the set.
*/
QBrush CandleStickSet::brush() const
{
    return d_ptr->m_brush;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CandleStickSetPrivate::CandleStickSetPrivate(const QString label, CandleStickSet *parent) : QObject(parent),
    q_ptr(parent),
    m_label(label),
    m_valuesCount(4),
    m_appendCount(0),
    m_pen(QPen(Qt::NoPen)),
    m_brush(QBrush(Qt::NoBrush)),
    m_series(0)
{
    m_values = new qreal[m_valuesCount];
}

CandleStickSetPrivate::~CandleStickSetPrivate()
{
    delete[] m_values;
}

bool CandleStickSetPrivate::append(qreal value)
{
    if (isValidValue(value) && m_appendCount < m_valuesCount) {
        m_values[m_appendCount++] = value;
        emit restructuredBox();

        return true;
    }
    return false;
}

bool CandleStickSetPrivate::append(QList<qreal> values)
{
    bool success = false;

    for (int i = 0; i < values.count(); i++) {
        if (isValidValue(values.at(i)) && m_appendCount < m_valuesCount) {
            success = true;
            m_values[m_appendCount++] = values.at(i);
        }
    }

    if (success)
        emit restructuredBox();

    return success;
}

void CandleStickSetPrivate::clear()
{
    m_appendCount = 0;
    for (int i = 0; i < m_valuesCount; i++)
         m_values[i] = 0.0;
    emit restructuredBox();
}

void CandleStickSetPrivate::setValue(const int index, const qreal value)
{
    if (index < m_valuesCount) {
        m_values[index] = value;
        emit updatedLayout();
    }
}

qreal CandleStickSetPrivate::value(const int index)
{
    if (index < 0 || index >= m_valuesCount)
        return 0;
    return m_values[index];
}
