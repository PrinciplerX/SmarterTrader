#ifndef ABSTRACTITEMPLOT_H
#define ABSTRACTITEMPLOT_H

#include "tradingcurvewidgets_global.h"
#include "traceplot.h"
#include <QWidget>
#include <QLabel>
#include "plottitlewidget.h"
#include "tradedata.h"

class PlotableItem;

namespace TradingCurveWidgets{

class TitledPlotAxis;
class AbstractItemAxis;

struct TRADINGCURVEWIDGETSSHARED_EXPORT IndexRange{
    int minIndex = 0;
    int offset = 0;

    IndexRange() { this->minIndex = 0; this->offset = 0; }
    IndexRange(int minIndex, int offset) { this->minIndex = minIndex; this->offset = offset; }
    bool operator==(const IndexRange& range) const{
        return (minIndex == range.minIndex) && (offset == range.offset);
    }

    bool operator!=(const IndexRange& range) const{
        return (minIndex != range.minIndex) || (offset != range.offset);
    }

    bool contains(int index) const { return index >= minIndex && index < (minIndex + offset); }

    int size() const { return offset; }
    void moveBackward(int n = 1) { minIndex -= n; }
    void moveForward(int n = 1) { minIndex += n; }
    int maxIndex() const { return minIndex + offset - 1; }
    int start() const { return minIndex; }
    int end() const { return minIndex + offset; }
    bool isEmpty() const { return offset <= 0; }
    QString toString() const { return QString("IndexRange { start = %1, offset = %2 }").arg(QString::number(minIndex), QString::number(offset)); }

    // end is the index after the last index of this range
    static IndexRange fromIndexInterval(int start, int end) { return {start, end - start}; }
};

/*!
 * \brief 以数据集合中每个数据项为单位的绘图系统。
 *
 * >> 明确一些概念：
 * 1. 绘图序号（PlotIndex）：用来表示canvas上绘制数据项彼此间关系的序号，总是以 0 开始（横坐标最小者序号为0）。
 * canvas底部的纵坐标轴总是以绘图序号来表示，例如\a dateLabel() 以及\a caculateScaleDiv() 就是这样。
 *
 * 2. 数据序号（ItemIndex）：用来表示canvas上所绘制数据项在数据集合中的序号，很可能不以 0 开始（和横坐标关系不确定）。
 *
 * >> AbstractItemPlot和AbstractItemAxis的关系：
 * 1. itemAxis通过发送indexRangeChanged()事件来通知itemPlot需要绘制的序号范围发生变化。
 * 2. itemAxis通过发送axisMapChanged()事件来通知itemPlot更新canvas map。
 *
 * >> AbstractItemPlot主要对以下事件做出反应：
 * 1. itemAxis的indexRangeChanged()事件。该事件出现时，需要重新计算画布数值范围，并且选择性地重绘画布。
 * 2. itemAxis的axisMapChanged()事件。该事件出现时，需要重新计算canvas map并重绘画布。
 * 3. resizeEvent。更新四个坐标轴并重绘。
 * 4. keyPressEvent。根据用户按键选择性地做出反应。
 *
 * \todo GridLine Settings
 * \todo 支持将ItemAxis放置到纵坐标轴位置上。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractItemPlot : public TracePlot
{
    Q_OBJECT

public:
    explicit AbstractItemPlot(QWidget* parent = nullptr);

    virtual ~AbstractItemPlot();

    /*!
     * \brief 在中心显示区域控件上绘制内容，由PlotCanvas绘图时调用。
     *
     * \sa SimplePlotCanvas::drawCanvas
     * \param painter 由SimplePlotCanvas提供的画笔。
     */
    virtual void drawCanvas(QWidget* canvas, QPainter *painter);

    /*!
     * \brief 在画布上绘制网格。
     * \sa drawCanvas(QPainter*);
     * \param painter 用来在canvas上绘制内容的画笔
     */
    virtual void drawCanvasGrid(QPainter* painter);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 坐标轴相关
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief caculateItemPos 根据数据序号计算它应该在canvas上显示的位置。
     * \param itemIndex 要求数据项在数组中的序号。
     * \return 返回在canvas上应该显示的位置，纵轴坐标对应为当日收盘价。
     *         返回QPoint(-1,-1)表示制定的item不在当前显示区域中。
     */
    int caculateItemPos(int index) const;
    int caculateItemIndex(int x) const;
    int caculatePlotPos(int index) const;
    int caculatePlotIndex(int x) const;
    QPoint caculateTargetPos(int index) const;

    const QwtScaleMap& getCanvasMap(int axisId) const;
    QwtScaleMap& getCanvasMap(int axisId);

    virtual AbstractItemAxis* getItemAxis() = 0;
    virtual const AbstractItemAxis* getItemAxis() const = 0;

    virtual void caculateScaleDivRange(double& min, double& max) const;
    virtual bool scaleDivNeedUpdate() const;

    IndexRange getIndexRange() const;
    void setIndexRange(IndexRange range);
    void setCanvasMap(const QwtScaleMap& map, int axisId);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 绘图元素（PlotableItem）
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief 添加一个新的PlotableItem，在坐标图中显示。
     */
    virtual void addPlotableItem(PlotableItem* item);
    void clearPlotableItem();
    bool containsItem(PlotableItem* item);

    void attachUpdater(AbstractHistoryData* data);

    virtual double regionalMaxValue(int minIndex, int maxIndex) const;
    virtual double regionalMinValue(int minIndex, int maxIndex) const;

    /*!
     * \brief 设定canvas纵向上顶部和底部留出的空白区域表示数据范围和纵向宽度和
     * 绘图区域纵向数值范围的比例。
     * \param factor 空白区域表示数据范围和纵向宽度和绘图区域纵向数值范围的比例。
     */
    void setVerticalExtraSpaceFactor(double factor = 0.07);

    /*!
     * \brief 设置纵轴上两个major tick间的最大宽度。
     * \param space 纵轴上两个major tick间的最大宽度
     */
    void setMaxMajorTickSpace(int space);
    int getMaxMajotTickSpace() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 网格相关
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setGridLinePen(QPen pen);
    QPen getGridLinePen() const;

    virtual int dataSize() const;

    virtual QString titleLabelString() const;

    virtual void keyPressEvent(QKeyEvent *event) override;
    /*! \brief 确保在显示时总是处于正确状态。*/
    virtual void showEvent(QShowEvent* event) override;
    /*! \brief 改变大小时，坐标映射关系可能改变，因此必须实时更新。*/
    virtual void resizeEvent(QResizeEvent* event) override;

    /*!
     * \brief 设定当前选中的数据项序号。
     * \param index 数据项序号
     */
    void setSelectedItemIndex(int index);
    int selectedItemIndex() const { return currentSelectedItemIndex; }

    virtual void updateAxes() override;

    /*! \brief 更新纵坐标轴。 */
    virtual void updateVerticalAxes(bool recaculate = false);
    /*! \brief 更新横坐标轴。*/
    virtual void updateHorizontialAxes();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ItemAxis相关
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief 设置数据项序号标签，item axis上的游标移动时，在游标上显示该标签。
     */
    void setItemLabelSeries(const QVector<QString>& series);
    void setMajorTickSeries(const QMap<int, QString>& series);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Title Widget
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void addTitleWidget(QWidget* widget);

signals:
    void canvasMapChanged(int axisId);

public slots:
    void onAxisMapChanged();
    void onIndexRangeChanged(IndexRange oldRange, IndexRange newRange);

protected slots:
    void onDataLoaded();
    void onDataAppended();
    void onDataUpdated();

protected:
    /*!
     * \brief 根据鼠标位置计算tracer位置，并且更新当前选中的数据位置
     *       （即currentSelectedItemIndex）。函数保证tracer总是指在影线位置。
     * \param 鼠标在canvas中的位置。
     * \return tracer位置。
     */
    virtual QPoint caculateAndUpdateTracerPos(QPoint mousePosInCanvas) override;

    virtual void updateTitleLabel();

    virtual void updateCanvasRange();
    virtual void updateAxesRange();
    virtual void updateAxesDiv();

    void onPlotableItemAdded(PlotableItem* item);
    void onSelectedItemChanged();

    QList<PlotableItem*> plotableItemList;

    virtual int bodyWidth() const = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 缓存
    /////////////////////////////////////////////////////////////////////////////////////////////////////

	void createTitleCache();
    bool isCacheEnabled() const;
    void enableCache();

private:
	QVector<QString> titleCache;
    bool cacheEnabled = false;

    int currentSelectedItemIndex;

    //canvas内空白区域表示数据范围和纵向宽度和绘图区域纵向数值范围的比例。
    double verticalExtraSpaceFactor = 0.0;

    //纵轴上两个major tick间的最大宽度。当纵轴长度变化时，会重新计算刻度值scaleDiv，调整
    //major tick数目，使major tick之间的宽度始终不超过这个值。
    int maxMajorTickSpace = 60;

    // 记录当前坐标轴被刻度分成的区间数。
    mutable int majorIntervalCount;

    PlotTitleWidget* titleLabel; /*! titled plot canvas的标题控件*/

    // 画布显示的数据范围。仅通过updateCanvasRange()来更新这些数据。
    double maxValue;
    double minValue;

    //上下左右四个坐标轴
    TitledPlotAxis* tTpa;
    TitledPlotAxis* bTpa;
    TitledPlotAxis* lTpa;
    TitledPlotAxis* rTpa;

    //用于绘制grid线的QPen
    QPen gridLinePen;

    QwtScaleMap canvasMapArray[axisCnt];

    QList<AbstractHistoryData*> mUpdater;
};

}
#endif // ABSTRACTITEMPLOT_H
