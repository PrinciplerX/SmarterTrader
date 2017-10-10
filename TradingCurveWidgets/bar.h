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
     * \brief drawItem ���ڻ��Ƶ�����������
     * \param itemPosX ����Ԫ���������ڵ�λ�õĺ�����
     * \param bodyWidth ����ͼ�ε�����ȣ���itemPosXλ��Ϊ���ģ���
     *                  �����˿�ȿ��ܻ����Ԫ�ؼ���غϡ�
     * \param data Ҫ��������Ԫ�������ݼ����е����
     * \param painter ���ڻ��ƵĻ���
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

#endif // BAR_H
