/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qcategoryaxis.h>
#include <qcategoryaxis_p.h>
#include <chartcategoryaxisx_p.h>
#include <chartcategoryaxisy_p.h>
#include <polarchartcategoryaxisangular_p.h>
#include <polarchartcategoryaxisradial_p.h>
#include <QChart>
#include <QtCore/QtMath>
#include <QtCore/QDebug>

SCHARTS_BEGIN_NAMESPACE
/*!
    \class QCategoryAxis
    \inmodule Qt Charts
    \brief The QCategoryAxis class allows putting a named ranges on the axis.

    This class can be used when the underlying data needs to be given extra meaning.
    Unlike with the QBarCategoryAxis the QCategoryAxis allows the categories ranges widths to be specified freely.

    Example code on how to use QCategoryAxis:
    \image api_category_axis.png
    \code
    QChartView *chartView = new QChartView;
    QLineSeries *series = new QLineSeries;
    // ...
    chartView->chart()->addSeries(series);

    QCategoryAxis *axisY = new QCategoryAxis;
    axisY->setMin(0);
    axisY->setMax(52);
    axisY->setStartValue(15);
    axisY->append("First", 20);
    axisY->append("Second", 37);
    axisY->append("Third", 52);
    chartView->chart()->setAxisY(axisY, series);
    \endcode
*/
/*!
    \qmltype CategoryAxis
    \instantiates QCategoryAxis
    \inqmlmodule QtCharts

    \inherits AbstractAxis
    \brief CategoryAxis allows putting a named ranges on the axis.

    For example:
    \image examples_qmlaxes3.png
    \snippet qmlaxes/qml/qmlaxes/View3.qml 1
*/

/*!
  \property QCategoryAxis::startValue
  Defines the low end of the first category on the axis.
*/
/*!
  \qmlproperty int CategoryAxis::startValue
  Defines the low end of the first category on the axis.
*/

/*!
  \property QCategoryAxis::count
  The count of categories.
*/
/*!
  \qmlproperty int CategoryAxis::count
  The count of categories.
*/

/*!
  \property QCategoryAxis::categoriesLabels
  The category labels as a string list.
*/
/*!
  \qmlproperty StringList CategoryAxis::categoriesLabels
  The category labels as a list of strings.
*/

/*!
  \fn void QCategoryAxis::categoriesChanged()
  Axis emits signal when the categories of the axis have changed.
*/

/*!
 \enum QCategoryAxis::AxisLabelsPosition

 This enum describes the position of the category labels.

 \value AxisLabelsPositionCenter Labels are centered to category.
 \value AxisLabelsPositionOnValue Labels are positioned to the high end limit of the category.
 */
/*!
  \property QCategoryAxis::labelsPosition
  Defines the position of the category labels. The labels in the beginning and in the end of the
  axes may overlap other axes labels when positioned on value.
*/
/*!
  \qmlproperty AxisLabelsPosition CategoryAxis::labelsPosition
  Defines the position of the category labels. The labels in the beginning and in the end of the
  axes may overlap other axes labels when positioned on value.
*/


/*!
    Constructs an axis object which is a child of \a parent.
*/
QCategoryAxis::QCategoryAxis(QObject *parent):
    QValueAxis(*new QCategoryAxisPrivate(this), parent)
{
}

/*!
    Destroys the object
*/
QCategoryAxis::~QCategoryAxis()
{
    Q_D(QCategoryAxis);
    if (d->m_chart)
        d->m_chart->removeAxis(this);
}

/*!
    \internal
*/
QCategoryAxis::QCategoryAxis(QCategoryAxisPrivate &d, QObject *parent): QValueAxis(d, parent)
{

}

/*!
 * \qmlmethod CategoryAxis::append(string label, real endValue)
 * 为坐标轴增加一个由\a label 指定的新条目。条目标签必须是唯一的。
 * 参数\a endValue 指定了条目表示区域的上限。
 * 它必须大于前一个条目的上限，否则该函数不会添加新条目。
*/
/*!
 * 为坐标轴增加一个由\a categoryLabel 指定的新条目。条目标签必须是唯一的。
 * 参数\a categoryEndValue 指定了条目表示区域的上限。
 * 它必须大于前一个条目的上限，否则该函数不会添加新条目。
 */
void QCategoryAxis::append(const QString &categoryLabel, qreal categoryEndValue)
{
    Q_D(QCategoryAxis);

    if (!d->m_categories.contains(categoryLabel)) {
        if (d->m_categories.isEmpty()) {
            Range range(d->m_categoryMinimum, categoryEndValue);
            d->m_categoriesMap.insert(categoryLabel, range);
            d->m_categories.append(categoryLabel);
            emit categoriesChanged();
        } else if (categoryEndValue > endValue(d->m_categories.last())) {
            Range previousRange = d->m_categoriesMap.value(d->m_categories.last());
            d->m_categoriesMap.insert(categoryLabel, Range(previousRange.second, categoryEndValue));
            d->m_categories.append(categoryLabel);
            emit categoriesChanged();
        }
    }
}

/*!
 * 设置 \a min 作为坐标轴上第一个分类条目表示区域范围的下限数值。
 * 如果已经有一些条目被添加到坐标轴则传递的数值必须小于已定义的第一个条目区域的上限数值。
 * 否则不会设置该数值。
 */
void QCategoryAxis::setStartValue(qreal min)
{
    Q_D(QCategoryAxis);
    if (d->m_categories.isEmpty()) {
        d->m_categoryMinimum = min;
        emit categoriesChanged();
    } else {
        Range range = d->m_categoriesMap.value(d->m_categories.first());
        if (min < range.second) {
            d->m_categoriesMap.insert(d->m_categories.first(), Range(min, range.second));
            emit categoriesChanged();
        }
    }
}

/*!
 * 返回由\a categoryLabel 指定的条目所表示区域的下限数值（最小数值）。
 */
qreal QCategoryAxis::startValue(const QString &categoryLabel) const
{
    Q_D(const QCategoryAxis);
    if (categoryLabel.isEmpty())
        return d->m_categoryMinimum;
    return d->m_categoriesMap.value(categoryLabel).first;
}

/*!
 * 返回由\a categoryLabel 指定的条目所表示区域的上限数值（最大数值）。
 */
qreal QCategoryAxis::endValue(const QString &categoryLabel) const
{
    Q_D(const QCategoryAxis);
    return d->m_categoriesMap.value(categoryLabel).second;
}

/*!
    \qmlmethod CategoryAxis::remove(string label)
    Removes a category specified by the \a label from the axis
*/
/*!
    Removes an interval specified by the \a categoryLabel from the axis
*/
void QCategoryAxis::remove(const QString &categoryLabel)
{
    Q_D(QCategoryAxis);
    int labelIndex = d->m_categories.indexOf(categoryLabel);

    // check if such label exists
    if (labelIndex != -1) {
        d->m_categories.removeAt(labelIndex);
        d->m_categoriesMap.remove(categoryLabel);

        // the range of the interval that follows (if exists) needs to be updated
        if (labelIndex < d->m_categories.count()) {
            QString label = d->m_categories.at(labelIndex);
            Range range = d->m_categoriesMap.value(label);

            // set the range
            if (labelIndex == 0) {
                range.first = d->m_categoryMinimum;
                d->m_categoriesMap.insert(label, range);
            } else {
                range.first = d->m_categoriesMap.value(d->m_categories.at(labelIndex - 1)).second;
                d->m_categoriesMap.insert(label, range);
            }
        }
        emit categoriesChanged();
    }
}

/*!
  \qmlmethod CategoryAxis::replace(string oldLabel, string newLabel)
  Replaces \a oldLabel of an existing category with a \a newLabel.
  If the old label does not exist the method returns without making any changes.
*/
/*!
  Replaces \a oldLabel of an existing category with a \a newLabel
  If the old label does not exist the method returns without making any changes.
 */
void QCategoryAxis::replaceLabel(const QString &oldLabel, const QString &newLabel)
{
    Q_D(QCategoryAxis);
    int labelIndex = d->m_categories.indexOf(oldLabel);

    // check if such label exists
    if (labelIndex != -1) {
        d->m_categories.replace(labelIndex, newLabel);
        Range range = d->m_categoriesMap.value(oldLabel);
        d->m_categoriesMap.remove(oldLabel);
        d->m_categoriesMap.insert(newLabel, range);
        emit categoriesChanged();
    }
}

/*!
  Returns the list of the intervals labels
 */
QStringList QCategoryAxis::categoriesLabels()
{
    Q_D(QCategoryAxis);
    return d->m_categories;
}

/*!
  Returns number of intervals.
 */
int QCategoryAxis::count() const
{
    Q_D(const QCategoryAxis);
    return d->m_categories.count();
}

/*!
  Returns the type of the axis
*/
QAbstractAxis::AxisType QCategoryAxis::type() const
{
    return QAbstractAxis::AxisTypeCategory;
}

void QCategoryAxis::setLabelsPosition(QCategoryAxis::AxisLabelsPosition position)
{
    Q_D(QCategoryAxis);
    if (d->m_labelsPosition != position) {
        d->m_labelsPosition = position;
        emit labelsPositionChanged(position);
    }
}

QCategoryAxis::AxisLabelsPosition QCategoryAxis::labelsPosition() const
{
    Q_D(const QCategoryAxis);
    return d->m_labelsPosition;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QCategoryAxisPrivate::QCategoryAxisPrivate(QCategoryAxis *q)
    : QValueAxisPrivate(q),
      m_categoryMinimum(0),
      m_labelsPosition(QCategoryAxis::AxisLabelsPositionCenter)
{

}

QCategoryAxisPrivate::~QCategoryAxisPrivate()
{

}

int QCategoryAxisPrivate::ticksCount() const
{
    return m_categories.count() + 1;
}

void QCategoryAxisPrivate::initializeGraphics(QGraphicsItem *parent)
{
    Q_Q(QCategoryAxis);
    ChartAxisElement *axis(0);
    if (m_chart->chartType() == QChart::ChartTypeCartesian) {
        if (orientation() == Qt::Vertical)
            axis = new ChartCategoryAxisY(q,parent);
        else if (orientation() == Qt::Horizontal)
            axis = new ChartCategoryAxisX(q,parent);
    }

    if (m_chart->chartType() == QChart::ChartTypePolar) {
        if (orientation() == Qt::Vertical)
            axis = new PolarChartCategoryAxisRadial(q, parent);
        if (orientation() == Qt::Horizontal)
            axis = new PolarChartCategoryAxisAngular(q, parent);
    }

    m_item.reset(axis);
    QAbstractAxisPrivate::initializeGraphics(parent);
}

#include "moc_qcategoryaxis.cpp"
#include "moc_qcategoryaxis_p.cpp"

SCHARTS_END_NAMESPACE
