#ifndef QBOXSET_P_H
#define QBOXSET_P_H

#include <QBoxSet>
#include <QtCore/QMap>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QFont>

SCHARTS_BEGIN_NAMESPACE

class QBoxPlotSeriesPrivate;

class QBoxSetPrivate : public QObject
{
    Q_OBJECT

public:
    QBoxSetPrivate(const QString label, QBoxSet *parent);
    ~QBoxSetPrivate();

    bool append(qreal value);
    bool append(QList<qreal> values);

    int remove(const int index, const int count);
    void clear();

    void setValue(const int index, const qreal value);

    qreal value(const int index);

    //!\todo 找到更好的解决办法
    void setSeries(QBoxPlotSeriesPrivate* series) { m_series = series; }

Q_SIGNALS:
    void restructuredBox();
    void updatedBox();
    void updatedLayout();

    //!\todo 找到更好的解决办法
//private:
public:
    const QBoxSet *q_ptr;
    QString m_label;
    const int m_valuesCount;
    qreal *m_values;
    int m_appendCount;
    QPen m_pen;
    QBrush m_brush;
    QBrush m_labelBrush;
    QFont m_labelFont;
    QBoxPlotSeriesPrivate *m_series;

    friend class QBoxSet;
    friend class QBoxPlotSeriesPrivate;
};

SCHARTS_END_NAMESPACE

#endif // QBOXSET_P_H
