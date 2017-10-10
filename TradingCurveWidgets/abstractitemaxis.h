#ifndef ABSTRACTITEMAXIS_H
#define ABSTRACTITEMAXIS_H

#include "tradingcurvewidgets_global.h"
#include "tracescalewidget.h"
#include "abstractitemplot.h"

namespace TradingCurveWidgets{

/*!
 * \brief 基于离散序列数据集合的坐标轴控件。
 *
 * 该坐标轴控件以数据序列的序号为定位方式，并且提供了刻度脚标和游标文字的保存。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractItemAxis : public TraceScaleWidget
{
    Q_OBJECT
public:
    explicit AbstractItemAxis(AbstractAxis::AxisType align = AbstractAxis::LeftScale,
                              QWidget *parent = nullptr);

    /*!
     * \brief 计算一个数据项对应坐标轴上的坐标。
     * \param index 数据序号。
     * \return 坐标轴上表示该数据项的位置。
     */
    virtual int caculateItemLocalPos(int index) const;

    /*!
     * \brief 计算一个绘图序号对应坐标轴上的坐标。
     * \param index 绘图序号。
     * \return 坐标轴上表示该绘图序号的位置。
     */
    virtual int caculatePlotLocalPos(int index) const;

    /*!
     * \brief 更新PlotIndexRange。子类应根据画布尺寸重新计算画布上可以显示的数据项范围。
     */
    virtual void updatePlotIndexRange() = 0;
    /*!
     * \brief 当前选中项发生变化。
     *
     * 对于有些坐标轴（如TradingCurveItemAxis），最新选中项可能会超过当前显示范围，需要更新显示范围。
     */
    virtual void onSelectedItemChanged() = 0;
    virtual void onPlotableItemAdded() = 0;

    virtual int bodyWidth() const = 0;

    /*!
     * \brief 获取游标文字。该函数使用频率较高，所以性能要求较高。
     * \param itemIndex 数据集合中的数据项序号。
     * \return 游标上显示的文字。
     */
    virtual QString getTracerLabel(int itemIndex) const;

    /*!
     * \param itemIndex 数据集合中的数据项序号。
     * \return 刻度上的脚标文字。
     */
    virtual QString getTickLabel(int itemIndex) const;

    /*!
     * \brief 重新计算刻度系统。
     *
     * 默认的计算方法是，使用ScaleMap把tick代表的数据项序号转化为屏幕坐标，再转化为刻度数值（没
     * 有实际意义，只为定位），然后将这些转化后的刻度值保存起来，绘制时再转化为刻度位置。
     *
     * \todo 对于item axis，不用scale map是不是效率更高而且更精确一点？
     */
    virtual void updateScaleDiv();

    /*!
     * \brief 设置 major tick 的脚标文字映射表。
     * \param tickLabels 绘图项序号-刻度脚标映射表（map）。坐标轴在主刻度（major tick）上显示脚标，该脚标的文字由该
     * 映射表决定，文字的内容可以是任意的。
     * \warning tick位置不是数据项位置，而是绘图序号位置。
     */
    void setTickLabels(const QMap<int, QString>& tickLabels);

    /*!
     * \brief 设置游标文字存储集合。
     * \param tracerLabel 游标文字向量，数据集合的每一个数据项对应一个游标文字。当游标移动到这个数据项
     * 所在位置上时，显示对应的游标文字。文字内容可以是任意的。
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
    /*! \brief 坐标轴的映射关系发送变化，原来的序号位置可能已经变化，需要重新绘制画布。*/
    void axisMapChanged();

    /*! \brief 数据项范围发送变化，可能需要重新计算数据项的数值范围。*/
    void indexRangeChanged(IndexRange oldRange, IndexRange newRange);

protected:
    virtual void drawTracer(QPainter* painter, int tracerPos) const override;

    /*!
     * \brief 更新刻度脚标信息。
     */
    virtual void updateTickLabels() = 0;
    virtual void updateLayout(bool update = true) override;

private:
    // 数据序号 - 日期
    QMap<int, QString> majorTickMap;
    QVector<QString> tracerLabelVector;
    QwtScaleMap axisMap;

    IndexRange indexRange;
};

inline void AbstractItemAxis::setIndexRange(int minIndex, int offset) { setIndexRange(IndexRange(minIndex, offset)); }
inline const IndexRange& AbstractItemAxis::getIndexRange() const { return indexRange; }

}

#endif // ABSTRACTITEMAXIS_H
