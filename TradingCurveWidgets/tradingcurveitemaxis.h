#ifndef TRADINGCURVEITEMAXIS_H
#define TRADINGCURVEITEMAXIS_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "abstractitemaxis.h"

#define BASIC_SCALE_LEVEL_COUNT 11

namespace TradingCurveWidgets{

class TRADINGCURVEWIDGETSSHARED_EXPORT TradingCurveItemAxis : public AbstractItemAxis
{
    Q_OBJECT
public:
    struct CandleStickSize{
        int shadowWidth;     //����Ӱ�ߵĿ��
        int bodyWidth;       //���岿�ֿ��
        int space;           //�����������߼������
    };

    explicit TradingCurveItemAxis(AbstractAxis::AxisType align, AbstractItemPlot *parent);

    int caculateItemPos(int index) const;
    int caculateItemIndex(int x) const;
    int caculatePlotPos(int index) const;
    int caculatePlotIndex(int x) const;

    int getScaleLevel() const;

    void resetIndexRange();
    virtual void updatePlotIndexRange() override;

    /*!
     * \brief �Ŵ�ͼ�Ρ��Բ���ָʾλ��Ϊ���ķŴ�ͼ�Ρ�
     *
     * ���ȣ�����currentScaleLevel����ȡ��Ӧ������������Ӧ�ĵ���������ռ�õĿ�ȡ�Ȼ������ڵ�ǰ�Ŀ�
     * ����������ܻ��Ƶ�����Ԫ����Ŀ�������Բ���posΪ���ģ�ȷ����Щ����Ԫ�صľ������䡣��󣬸�����
     * �����ݵ����ڷ�Χ�������Сֵ�����ݺ���������ʾ���䣬���»���ͼ�Ρ�
     *
     * \param pos ���ݼ����е���ţ���Ϊ�Ŵ����������λ�á�
     */
    virtual void zoomIn();

    /*!
     * \brief ��Сͼ�Ρ��Բ���ָʾλ��Ϊ������Сͼ�Ρ�
     * \param pos ���ݼ����е���ţ���Ϊ��С���������λ��
     */
    virtual void zoomOut();

    CandleStickSize currentItemSize();
    virtual int bodyWidth() const override;

    virtual void onSelectedItemChanged() override;
    virtual void onPlotableItemAdded() override;

    virtual void resizeEvent(QResizeEvent *event) override;

    const AbstractItemPlot *getItemPlot() const;
    AbstractItemPlot *getItemPlot();

    /*!
     * \brief indexPaintInterval ������Ż�ͼ����Ĳ�ֵ������scale level�йء�
     */
    double indexPaintInterval() const;
    const CandleStickSize& candleStickSize() const;

    virtual void updateAxisMap() override;
    virtual QwtScaleMap caculateCanvasMap(const QWidget *canvas) const override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Controller
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setScaleLevel(int scaleLevel);

signals:
    void scaleLevelChanged();

protected:
     virtual void updateTickLabels() override;
    /*!
     * \brief ������Ļ����ʾ���������ޣ���С��ţ�����������Χ
     * \param minIndex ��Ļ����ʾ���ݵ�������С��ţ�������һ���Ǽ������е���С��š�
     * \return ���������Χ
     */
    IndexRange caculatePlotRangeWithMinIndex(int minIndex);
    /*!
     * \brief ������Ļ����ʾ���������ޣ������ţ�����������Χ
     * \param maxIndex ��Ļ����ʾ���ݵ����������ţ�������һ���Ǽ������е������š�
     * \return ���������Χ
     */
    IndexRange caculatePlotRangeWithMaxIndex(int maxIndex);
    /*!
     * \brief ������Ļ����ʾ���������ޣ��м���ţ�����������Χ
     * \param maxIndex ��Ļ����ʾ���ݵ������м���ţ�������һ���Ǽ������е��м���š�
     * \return ���������Χ
     */
    IndexRange caculatePlotRangeWithMidIndex(int midIndex);

private:
    /*! ��ǰ�������𡣿ɷŴ󵽵���󼶱�Ϊ0������ֵ���Ӷ�ͼ������С������10ʱ������֮������غ� */
    int currentScaleLevel = 6;

    static const CandleStickSize CandleStickSizeStandard[BASIC_SCALE_LEVEL_COUNT];

    AbstractItemPlot* itemPlot;
};

}

#endif // TRADINGCURVEITEMAXIS_H
