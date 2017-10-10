#ifndef REALTIMEITEMAXIS_H
#define REALTIMEITEMAXIS_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "abstractitemaxis.h"

namespace TradingCurveWidgets{

/*!
 * \brief ����������ʱ��ʾ�̶���Ŀ��PlotIndex��
 *
 * �̶Ƚű������ԣ�����������źͿ̶Ƚű��һ���ֵ䣨map������Ҫʱ��̬���ҡ�
 * �α����ּ�����ԣ�����������ʽ��������ָʾ���֣�vector������ѯʱֱ�ӷ��ء�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimeItemAxis : public AbstractItemAxis
{
    Q_OBJECT
public:
    /*!
     * \brief ���ݸ��µķ���
     *
     * ���ﲻ����ʵ���������֯�����Ǽ��裺����Դ���Ϊ0������Ϊ��ɵ����ݣ�����Դ�����������Ϊ�����ݡ�
     * ������� = ����� + ��ͼ��š�
     */
    enum UpdateDirection{
        //! ���¾����������ƶ�����ʾ��������������ߣ��ұ߻�ͼ���Ϊ0����߻�ͼ�����󣨻�ͼ������ӷ����x�������������෴����
        NewestLeft,

        //! ���¾����������ƶ�����ʾ�������������ұߣ��ұ߻�ͼ��������߻�ͼ���Ϊ0����ͼ������ӷ����x��������������ͬ����
        NewestRight,

        //! ����ʱ�����ƶ������ݣ����Ҿ����ݱ��̶�������ߣ�������ӷ����x��������������ͬ����
        OldestLeft,

        //! ����ʱ�����ƶ������ݣ����Ҿ����ݱ��̶������ұߣ�������ӷ����x�������������෴����
        OldestRight
    };

    explicit RealTimeItemAxis(AbstractAxis::AxisType align = AbstractAxis::LeftScale,
                              UpdateDirection direction = NewestLeft,
                              QWidget *parent = nullptr);

    void setPlotableIndexCount(int count);
    int getPlotableIndexCount() const;

    DELETE_LATER //virtual int caculateItemPos(int index) const;
    DELETE_LATER //virtual int caculateItemIndex(int x) const;
    DELETE_LATER //virtual int caculatePlotIndex(int x) const;
    DELETE_LATER //virtual int caculatePlotPos(int index) const;

    virtual QwtScaleMap caculateCanvasMap(const QWidget *canvas) const override;

    virtual void updateAxisMap();

    virtual void onSelectedItemChanged() override;
    virtual void onPlotableItemAdded() override;

    virtual void updatePlotIndexRange() override;
    virtual int bodyWidth() const override;

protected:
    virtual void updateTickLabels() override;

private:
    /*! \brief canvas����ʾ������������*/
    int plotableIndexCount = 257;

    UpdateDirection updateDirection;

    // ������� - ����
    QMap<int, QString> majorTickMap;
    QVector<QString> tracerLabelVector;
};

}

#endif // REALTIMEITEMAXIS_H
