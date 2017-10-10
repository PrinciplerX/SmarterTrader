#ifndef CHARTVALUEAXISX_H
#define CHARTVALUEAXISX_H

#include <horizontalaxis_p.h>

SCHARTS_BEGIN_NAMESPACE

class QValueAxis;

class ChartValueAxisX : public HorizontalAxis
{
    Q_OBJECT
public:
    ChartValueAxisX(QValueAxis *axis, QGraphicsItem *item = 0);
    ~ChartValueAxisX();

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
protected:
    QVector<qreal> calculateLayout() const;
    void updateGeometry();
private Q_SLOTS:
    void handleTickCountChanged(int tick);
    void handleMinorTickCountChanged(int tick);
    void handleLabelFormatChanged(const QString &format);

private:
    QValueAxis *m_axis;
};

SCHARTS_END_NAMESPACE

#endif /* CHARTVALUEAXISX_H */
