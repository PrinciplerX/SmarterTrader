#ifndef VOLUME_H
#define VOLUME_H

#include "plotableitem.h"
#include <techniqueindicator.h>
#include <tradedata.h>
#include "operation.h"
#include "tradedataoperations.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT Volume : public PlotableItem, public TechniqueIndicator
{
public:
    Volume(AbstractHistoryData *data);
    virtual ~Volume(){}

    virtual void draw(const QVector<int>& itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int bodyWidth,
                      const QwtScaleMap& yMap,
                      QPainter* painter);

    virtual QPen getRaisePen() const;
    virtual QBrush getRaiseBrush() const;
    virtual QPen getFallPen() const;
    virtual QBrush getFallBrush() const;
    virtual QPen getStillPen() const;
    virtual QBrush getStillBrush() const;

    /*! \sa PlotableSequence::regionalMaxValue() */
    virtual double regionalMaxValue(int minIndex,int maxIndex) const override;

    /*! \sa PlotableSequence::regionalMinValue() */
    virtual double regionalMinValue(int minIndex, int maxIndex) const override;

    virtual void onDataLoaded() override;
    virtual void onDataUpdated() override;
    virtual void onDataAppended() override;

    long at(int index) const { return vol.at(index); }
    virtual int size() const override { return mHistoryData->size(); }

    /*! \sa PlotableSequence::dateString() */
    virtual QString dateString(int index) override;
    virtual QString titleLabelString(int index) override;

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const override {
        return yMap.transform(vol.at(itemIndex));
    }

    virtual void createTitleCache();

    virtual QString category() const override { return mCategory; }

protected:
    /*!
     * \brief 从原始数据计算得出需要保存的数据。
     */
    void initData();

    AbstractHistoryData* historyData();

private:
    Operation<bool>* gain;
    VOL vol;

    AbstractHistoryData* mHistoryData;

    QVector<QString> titleCache;
    static const QString mCategory;
};

#endif // VOLUME_H
