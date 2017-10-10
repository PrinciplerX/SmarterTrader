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
 * \brief ͨ�����ߵ����ݴ洢�ṹ��
 *
 * һ������£����԰����߱�ʾΪ�±�-��ֵ�����С����±�Ľ��ͷ�ʽ���������ߵĲ�����ʽ��
 */
typedef QVector<double> PlotableCurveData;

/*!
 * \brief ͨ�����ߡ�
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
     * \brief ���ݼ��������Ϊ��minIndex��maxIndex֮�������Ԫ���е����ֵ
     * \param minIndex ָ��Ԫ�ؼ������ݼ����е���С���
     * \param maxIndex ָ��Ԫ�ؼ������ݼ����е������ż�1
     */
    virtual double regionalMaxValue(int minIndex,int maxIndex) const;

    /*!
     * \brief ���ݼ��������Ϊ��minIndex��maxIndex֮�������Ԫ���е���Сֵ
     * \param minIndex ָ��Ԫ�ؼ������ݼ����е���С���
     * \param maxIndex ָ��Ԫ�ؼ������ݼ����е������ż�1
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
