#ifndef CANDLESTICK_H
#define CANDLESTICK_H

#include <tradedata.h>
#include <qwt_scale_map.h>
#include <techniqueindicator.h>
#include "plotableitem.h"
#include "operation.h"
#include "tradedataoperations.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT CandleStick : public PlotableItem, public TechniqueIndicator
{
public:
    explicit CandleStick(AbstractHistoryData* data);

    virtual ~CandleStick(){}

    virtual void draw(const QVector<int>& itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int bodyWidth,
                      const QwtScaleMap& yMap,
                      QPainter* painter) override;

    /*!
     * \brief ���ݼ��������Ϊ��minIndex��maxIndex֮�������Ԫ���е����ֵ
     * \param minIndex ָ��Ԫ�ؼ������ݼ����е���С���
     * \param maxIndex ָ��Ԫ�ؼ������ݼ����е������ż�1
     */
    virtual double regionalMaxValue(int minIndex,int maxIndex) const override;
    /*!
     * \brief ���ݼ��������Ϊ��minIndex��maxIndex֮�������Ԫ���е���Сֵ
     * \param minIndex ָ��Ԫ�ؼ������ݼ����е���С���
     * \param maxIndex ָ��Ԫ�ؼ������ݼ����е������ż�1
     */
    virtual double regionalMinValue(int minIndex, int maxIndex) const override;

    virtual void onDataLoaded() override;
    virtual void onDataUpdated() override;
    virtual void onDataAppended() override;

    virtual int size() const override {return mHistoryData->size();}

    /*! \sa PlotableSequence::dateString() */
    virtual QString dateString(int index) override;
    virtual QString titleLabelString(int index) override;

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const override {
        return yMap.transform(close(itemIndex));
    }

    virtual QString category() const override { return mCategory; }

protected:
    /*!
     * \brief ��ԭʼ���ݼ���ó���Ҫ��������ݡ�
     */
    void initData();

private:
    static const QString mCategory;

    OPEN open;
    HIGH high;
    LOW low;
    CLOSE close;

    //! ���ݱ仯ʱ��Ҫ���µ����ݡ�
    Operation<bool>* gain;

    AbstractHistoryData* mHistoryData;
};

#endif // CANDLESTICK_H
