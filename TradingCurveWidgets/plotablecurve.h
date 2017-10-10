#ifndef PLOTABLECURVE_H
#define PLOTABLECURVE_H

#include "tradingcurvewidgets_global.h"
#include <qwt_scale_map.h>
#include "tradingcurveplot.h"
#include "plotableitem.h"
#include <tradedata.h>
#include <QPainter>
#include <QList>
#include <QVector>
#include "operation.h"

/*!
 * \brief 通用曲线的数据存储结构。
 *
 * 一般情况下，可以把曲线表示为下标-数值对序列。对下标的解释方式决定了曲线的采样方式。
 */
typedef QVector<double> PlotableCurveData;

/*!
 * \brief 通用曲线。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT PlotableCurve : public PlotableItem
{
public:
    PlotableCurve(Operation<double>* input = nullptr, const QString& name = QString("Custom"), QPen curvePen = QPen());
    PlotableCurve(const Operation<double>& input, const QString& name = QString("Custom"), QPen curvePen = QPen());

    virtual ~PlotableCurve();

    void setInput(Operation<double>* input);
    void setInput(Operation<double>& input);

    double operator[](int index) const;
    double at(int index) const;
    Operation<double>* operation();

    virtual void draw(const QVector<int>& itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int bodyWidth,
                      const QwtScaleMap& yMap,
                      QPainter* painter) override;

    void setPen(const QPen& pen);
    QPen getPen();

    /*!
     * \brief 数据集合中序号为与minIndex和maxIndex之间的所有元素中的最大值
     * \param minIndex 指定元素集在数据集合中的最小序号
     * \param maxIndex 指定元素集在数据集合中的最大序号加1
     */
    virtual double regionalMaxValue(int minIndex,int maxIndex) const;

    /*!
     * \brief 数据集合中序号为与minIndex和maxIndex之间的所有元素中的最小值
     * \param minIndex 指定元素集在数据集合中的最小序号
     * \param maxIndex 指定元素集在数据集合中的最大序号加1
     */
    virtual double regionalMinValue(int minIndex, int maxIndex) const;

    double regionalSum(int start, int offset) const;

    virtual int size() const;

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const;

    virtual QString titleLabelString(int index) override;

    virtual void createTitleCache();

    virtual QString valueToString(double value);

    virtual QString dateString(int index) { Q_UNUSED(index) return QString("Not Storaged"); }

private:
    Operation<double>* mInput;
    QVector<QString> titleCache;
    QPen pen;
};

#endif // PLOTABLECURVE_H
