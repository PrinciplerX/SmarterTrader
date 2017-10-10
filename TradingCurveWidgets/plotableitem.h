#ifndef PLOTABLEITEM_H
#define PLOTABLEITEM_H

#include "tradingcurvewidgets_global.h"
#include <qwt_scale_map.h>
#include "abstractitemplot.h"
#include <tradedata.h>
#include <QPainter>
#include <QList>

class TRADINGCURVEWIDGETSSHARED_EXPORT PlotableItem
{
public:
    /*!
     * \brief ��ԭʼ���ݳ�ʼ����
     *
     * ��ԭʼ�����м��㣨��Ҫ�����ദ���õ�����Ӧ�ñ��棨����Ҫ��TradingCurvePlot����ʾ�������ݡ�
     * \param data ԭʼ�������ݡ�
     */
    PlotableItem();
    virtual ~PlotableItem();

    /*!
     * \brief ��ɫ����
     */
    enum ColorTheme{
        Light,      //ǳɫ��������
        Dark,       //��ɫ��������
        HighContrastBlack,
        HighContrastWhite,
        InvalidTheme //��Ч����
    };

    /*!
     * \brief ���ڻ���Ҫ��ʾ������
     * \param itemPosX ����Ԫ���������ڵ�λ�õ�����QVector
     * \param indexRange Ҫ��������Ԫ���±����
     * \param yMap ����ʵ����ֵ����ͼ�豸�������ӳ���ϵ��ScaleMap
     * \param painter ���ڻ��ƵĻ���
     */
    virtual void draw(const QVector<int>& itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int bodyWidth,
                      const QwtScaleMap& yMap,
                      QPainter* painter) = 0;

    /*!
     * \brief ���ݼ��������Ϊ��minIndex��maxIndex֮�������Ԫ���е����ֵ
     * \param minIndex ָ��Ԫ�ؼ������ݼ����е���С���
     * \param maxIndex ָ��Ԫ�ؼ������ݼ����е������ż�1
     */
    virtual double regionalMaxValue(int minIndex,int maxIndex) const = 0;

    /*!
     * \brief ���ݼ��������Ϊ��minIndex��maxIndex֮�������Ԫ���е���Сֵ
     * \param minIndex ָ��Ԫ�ؼ������ݼ����е���С���
     * \param maxIndex ָ��Ԫ�ؼ������ݼ����е������ż�1
     */
    virtual double regionalMinValue(int minIndex, int maxIndex) const = 0;

    double maxValueAt(int index) const;
    double minValueAt(int index) const;

    static double regionalMaxValue(const QVector<TradeData>& data, int minIndex, int maxIndex);
    static double regionalMinValue(const QVector<TradeData>& data, int minIndex, int maxIndex);

    virtual int size() const = 0;

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const = 0;

    /*!
     * \brief ��ȡ���ݼ������±�indexλ���ϵ�ʱ��ֵ��
     * \param index ���ݼ����±�
     * \return ʱ��ֵ��
     */
    virtual QString dateString(int index) = 0;

    /*!
     * \brief ������ʾ�ڱ���QLabel����TitledPlotCanvas�ı����У��ϵ��������ݡ�
     *
     * ͨ��������ʾ��ǰָ�����ֵ��������html��ǩ��������ʾ��ͬ��ɫ�����⣬����ʹ��ʵ�ִ���õĻ��棬����ʱֱ�Ӷ�ȡ��
     * \param index ��ǰѡ�е��������±�
     * \return Ҫ�ڱ���QLabel����ʾ������
     */
    virtual QString titleLabelString(int index) = 0;

    /*!
     * \brief �����������ݡ�
     *
     * Ϊ����������ʱ�û������ƶ���꣨tracer�����д���������ֿ������󣬿�����ǰ���컺�沢������ʱֱ�Ӷ�ȡ��
     */
    //virtual void createTitleCache() {}

    //����
    virtual QString name(){ return nameString; }
    void setName(QString name){ nameString = name;}
    void resetName(){ nameString =QString(); }

    //�������⣬������ɸ�������ֵ��ȷ����ͼ��ʽ
    ColorTheme getTheme() const { return theme; }
    void setTheme(const ColorTheme &value) { theme = value; }

private:
    QString nameString;
    ColorTheme theme;
};

inline double PlotableItem::maxValueAt(int index) const { return regionalMaxValue(index, index); }

inline double PlotableItem::minValueAt(int index) const { return regionalMinValue(index, index); }

#endif // PLOTABLEITEM_H
