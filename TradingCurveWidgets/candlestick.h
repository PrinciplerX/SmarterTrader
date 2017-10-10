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
     * \brief 数据集合中序号为与minIndex和maxIndex之间的所有元素中的最大值
     * \param minIndex 指定元素集在数据集合中的最小序号
     * \param maxIndex 指定元素集在数据集合中的最大序号加1
     */
    virtual double regionalMaxValue(int minIndex,int maxIndex) const override;
    /*!
     * \brief 数据集合中序号为与minIndex和maxIndex之间的所有元素中的最小值
     * \param minIndex 指定元素集在数据集合中的最小序号
     * \param maxIndex 指定元素集在数据集合中的最大序号加1
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
     * \brief 从原始数据计算得出需要保存的数据。
     */
    void initData();

private:
    static const QString mCategory;

    OPEN open;
    HIGH high;
    LOW low;
    CLOSE close;

    //! 数据变化时需要更新的数据。
    Operation<bool>* gain;

    AbstractHistoryData* mHistoryData;
};

#endif // CANDLESTICK_H
