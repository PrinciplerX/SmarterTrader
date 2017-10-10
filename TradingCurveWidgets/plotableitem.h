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
     * \brief 从原始数据初始化。
     *
     * 从原始数据中计算（需要在子类处理）得到本类应该保存（或需要在TradingCurvePlot中显示）的数据。
     * \param data 原始交易数据。
     */
    PlotableItem();
    virtual ~PlotableItem();

    /*!
     * \brief 颜色主题
     */
    enum ColorTheme{
        Light,      //浅色背景主题
        Dark,       //深色背景主题
        HighContrastBlack,
        HighContrastWhite,
        InvalidTheme //无效主题
    };

    /*!
     * \brief 用于绘制要显示的内容
     * \param itemPosX 绘制元素中心所在的位置的坐标QVector
     * \param indexRange 要绘制数据元素下标序号
     * \param yMap 保存实际数值到绘图设备纵坐标的映射关系的ScaleMap
     * \param painter 用于绘制的画笔
     */
    virtual void draw(const QVector<int>& itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int bodyWidth,
                      const QwtScaleMap& yMap,
                      QPainter* painter) = 0;

    /*!
     * \brief 数据集合中序号为与minIndex和maxIndex之间的所有元素中的最大值
     * \param minIndex 指定元素集在数据集合中的最小序号
     * \param maxIndex 指定元素集在数据集合中的最大序号加1
     */
    virtual double regionalMaxValue(int minIndex,int maxIndex) const = 0;

    /*!
     * \brief 数据集合中序号为与minIndex和maxIndex之间的所有元素中的最小值
     * \param minIndex 指定元素集在数据集合中的最小序号
     * \param maxIndex 指定元素集在数据集合中的最大序号加1
     */
    virtual double regionalMinValue(int minIndex, int maxIndex) const = 0;

    double maxValueAt(int index) const;
    double minValueAt(int index) const;

    static double regionalMaxValue(const QVector<TradeData>& data, int minIndex, int maxIndex);
    static double regionalMinValue(const QVector<TradeData>& data, int minIndex, int maxIndex);

    virtual int size() const = 0;

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const = 0;

    /*!
     * \brief 获取数据集合中下标index位置上的时间值。
     * \param index 数据集合下标
     * \return 时间值。
     */
    virtual QString dateString(int index) = 0;

    /*!
     * \brief 用于显示在标题QLabel（如TitledPlotCanvas的标题中）上的文字内容。
     *
     * 通常用来显示当前指标的数值，可以用html标签修饰以显示不同颜色。另外，可以使用实现创造好的缓存，运行时直接读取。
     * \param index 当前选中的数据项下标
     * \return 要在标题QLabel中显示的内容
     */
    virtual QString titleLabelString(int index) = 0;

    /*!
     * \brief 构建缓存数据。
     *
     * 为避免在运行时用户快速移动光标（tracer）进行大量计算出现卡顿现象，可以提前创造缓存并在运行时直接读取。
     */
    //virtual void createTitleCache() {}

    //名称
    virtual QString name(){ return nameString; }
    void setName(QString name){ nameString = name;}
    void resetName(){ nameString =QString(); }

    //设置主题，派生类可根据主题值来确定绘图方式
    ColorTheme getTheme() const { return theme; }
    void setTheme(const ColorTheme &value) { theme = value; }

private:
    QString nameString;
    ColorTheme theme;
};

inline double PlotableItem::maxValueAt(int index) const { return regionalMaxValue(index, index); }

inline double PlotableItem::minValueAt(int index) const { return regionalMinValue(index, index); }

#endif // PLOTABLEITEM_H
