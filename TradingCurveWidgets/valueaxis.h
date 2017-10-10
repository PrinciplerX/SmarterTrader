#ifndef VALUEAXIS_H
#define VALUEAXIS_H

#include "tradingcurvewidgets_global.h"
#include "abstracttraceaxis.h"
#include "rectangularaxisdraw.h"
#include <qwt_scale_engine.h>

/*!
 * \brief The ValueAxis class
 * \todo 正在施工 . . .
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT ValueAxis : public AbstractTraceAxis
{
    Q_OBJECT
    Q_PROPERTY(double min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(double max READ max WRITE setMax NOTIFY maxChanged)

public:
    explicit ValueAxis( QWidget *parent = nullptr );
    explicit ValueAxis( AxisType axisType, QWidget *parent = nullptr );

    // 数值范围相关
    void setMin(double min);
    double min() const;
    void setMax(double max);
    double max() const;
    void setRange(double min, double max);

    static bool isValidValue(double min, double max);
    static bool isValidValue(double value);

    // ticks相关
    void setTickCount(int count);
    int tickCount() const;
    void setMinorTickCount(int count);
    int minorTickCount() const;

    // label格式
    void setLabelFormat(const QString &format);
    QString labelFormat() const;

Q_SIGNALS:
    void minChanged(double min);
    void maxChanged(double max);
    void rangeChanged(double min, double max);
    void tickCountChanged(int tickCount);
    void minorTickCountChanged(int tickCount);
    void labelFormatChanged(const QString &format);

private:
    double minValue;
    double maxValue;

    QwtScaleEngine* scaleEngine = nullptr;
    RectangularAxisDraw* scaleDraw = nullptr;
};

inline double ValueAxis::min() const { return minValue; }
inline double ValueAxis::max() const { return maxValue; }


#endif // VALUEAXIS_H
