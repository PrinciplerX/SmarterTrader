#ifndef BAR_H
#define BAR_H

#include "plotableitem.h"
#include <techniqueindicator.h>
#include <tradedata.h>
#include "operation.h"
#include "tradedataoperations.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT Bar : public PlotableItem, public TechniqueIndicator
{
public:
    explicit Bar(AbstractHistoryData* data);

    virtual void draw(const QVector<int>& itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int bodyWidth,
                      const QwtScaleMap& yMap,
                      QPainter* painter);

    /*!
     * \brief drawItem 用于绘制单个美国柱线
     * \param itemPosX 绘制元素中心所在的位置的横坐标
     * \param bodyWidth 绘制图形的最大宽度（以itemPosX位置为中心）。
     *                  超过此宽度可能会造成元素间的重合。
     * \param data 要绘制数据元素在数据集合中的序号
     * \param painter 用于绘制的画笔
     */
    void drawItem(int itemPosX, int bodyWidth, int itemIndex, const QwtScaleMap& yMap, QPainter* painter);

    virtual double regionalMaxValue(int minIndex,int maxIndex) const override;
    virtual double regionalMinValue(int minIndex, int maxIndex) const override;

    virtual void onDataLoaded() override;
    virtual void onDataUpdated() override;
    virtual void onDataAppended() override;

    virtual int size() const override {return mHistoryData->size();}

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

#endif // BAR_H
