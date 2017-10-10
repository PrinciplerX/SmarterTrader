#ifndef ABSTRACTITEMAXIS_H
#define ABSTRACTITEMAXIS_H

#include "tradingcurvewidgets_global.h"
#include "tracescalewidget.h"
#include "abstractitemplot.h"

namespace TradingCurveWidgets{

/*!
 * \brief ������ɢ�������ݼ��ϵ�������ؼ���
 *
 * ��������ؼ����������е����Ϊ��λ��ʽ�������ṩ�˿̶Ƚű���α����ֵı��档
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractItemAxis : public TraceScaleWidget
{
    Q_OBJECT
public:
    explicit AbstractItemAxis(AbstractAxis::AxisType align = AbstractAxis::LeftScale,
                              QWidget *parent = nullptr);

    /*!
     * \brief ����һ���������Ӧ�������ϵ����ꡣ
     * \param index ������š�
     * \return �������ϱ�ʾ���������λ�á�
     */
    virtual int caculateItemLocalPos(int index) const;

    /*!
     * \brief ����һ����ͼ��Ŷ�Ӧ�������ϵ����ꡣ
     * \param index ��ͼ��š�
     * \return �������ϱ�ʾ�û�ͼ��ŵ�λ�á�
     */
    virtual int caculatePlotLocalPos(int index) const;

    /*!
     * \brief ����PlotIndexRange������Ӧ���ݻ����ߴ����¼��㻭���Ͽ�����ʾ�������Χ��
     */
    virtual void updatePlotIndexRange() = 0;
    /*!
     * \brief ��ǰѡ������仯��
     *
     * ������Щ�����ᣨ��TradingCurveItemAxis��������ѡ������ܻᳬ����ǰ��ʾ��Χ����Ҫ������ʾ��Χ��
     */
    virtual void onSelectedItemChanged() = 0;
    virtual void onPlotableItemAdded() = 0;

    virtual int bodyWidth() const = 0;

    /*!
     * \brief ��ȡ�α����֡��ú���ʹ��Ƶ�ʽϸߣ���������Ҫ��ϸߡ�
     * \param itemIndex ���ݼ����е���������š�
     * \return �α�����ʾ�����֡�
     */
    virtual QString getTracerLabel(int itemIndex) const;

    /*!
     * \param itemIndex ���ݼ����е���������š�
     * \return �̶��ϵĽű����֡�
     */
    virtual QString getTickLabel(int itemIndex) const;

    /*!
     * \brief ���¼���̶�ϵͳ��
     *
     * Ĭ�ϵļ��㷽���ǣ�ʹ��ScaleMap��tick��������������ת��Ϊ��Ļ���꣬��ת��Ϊ�̶���ֵ��û
     * ��ʵ�����壬ֻΪ��λ����Ȼ����Щת����Ŀ̶�ֵ��������������ʱ��ת��Ϊ�̶�λ�á�
     *
     * \todo ����item axis������scale map�ǲ���Ч�ʸ��߶��Ҹ���ȷһ�㣿
     */
    virtual void updateScaleDiv();

    /*!
     * \brief ���� major tick �Ľű�����ӳ���
     * \param tickLabels ��ͼ�����-�̶Ƚű�ӳ���map���������������̶ȣ�major tick������ʾ�ű꣬�ýű�������ɸ�
     * ӳ�����������ֵ����ݿ���������ġ�
     * \warning tickλ�ò���������λ�ã����ǻ�ͼ���λ�á�
     */
    void setTickLabels(const QMap<int, QString>& tickLabels);

    /*!
     * \brief �����α����ִ洢���ϡ�
     * \param tracerLabel �α��������������ݼ��ϵ�ÿһ���������Ӧһ���α����֡����α��ƶ������������
     * ����λ����ʱ����ʾ��Ӧ���α����֡��������ݿ���������ġ�
     */
    void setTracerLabels(const QVector<QString>& tracerLabels);

    const QMap<int, QString> &getMajorTickSeries() const;
    const QVector<QString> &getItemLabelSeries() const;

    virtual void updateAxisMap();
    virtual void updateAxisDiv();
    virtual QwtScaleMap caculateCanvasMap(const QWidget* canvas) const override;
    void setIndexRange(int minItem, int offset);
    void setIndexRange(IndexRange newRange);
    const IndexRange& getIndexRange() const;

signals:
    /*! \brief �������ӳ���ϵ���ͱ仯��ԭ�������λ�ÿ����Ѿ��仯����Ҫ���»��ƻ�����*/
    void axisMapChanged();

    /*! \brief �����Χ���ͱ仯��������Ҫ���¼������������ֵ��Χ��*/
    void indexRangeChanged(IndexRange oldRange, IndexRange newRange);

protected:
    virtual void drawTracer(QPainter* painter, int tracerPos) const override;

    /*!
     * \brief ���¿̶Ƚű���Ϣ��
     */
    virtual void updateTickLabels() = 0;
    virtual void updateLayout(bool update = true) override;

private:
    // ������� - ����
    QMap<int, QString> majorTickMap;
    QVector<QString> tracerLabelVector;
    QwtScaleMap axisMap;

    IndexRange indexRange;
};

inline void AbstractItemAxis::setIndexRange(int minIndex, int offset) { setIndexRange(IndexRange(minIndex, offset)); }
inline const IndexRange& AbstractItemAxis::getIndexRange() const { return indexRange; }

}

#endif // ABSTRACTITEMAXIS_H
