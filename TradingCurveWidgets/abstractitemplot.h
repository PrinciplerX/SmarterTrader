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
 * \brief �����ݼ�����ÿ��������Ϊ��λ�Ļ�ͼϵͳ��
 *
 * >> ��ȷһЩ���
 * 1. ��ͼ��ţ�PlotIndex����������ʾcanvas�ϻ���������˴˼��ϵ����ţ������� 0 ��ʼ����������С�����Ϊ0����
 * canvas�ײ����������������Ի�ͼ�������ʾ������\a dateLabel() �Լ�\a caculateScaleDiv() ����������
 *
 * 2. ������ţ�ItemIndex����������ʾcanvas�������������������ݼ����е���ţ��ܿ��ܲ��� 0 ��ʼ���ͺ������ϵ��ȷ������
 *
 * >> AbstractItemPlot��AbstractItemAxis�Ĺ�ϵ��
 * 1. itemAxisͨ������indexRangeChanged()�¼���֪ͨitemPlot��Ҫ���Ƶ���ŷ�Χ�����仯��
 * 2. itemAxisͨ������axisMapChanged()�¼���֪ͨitemPlot����canvas map��
 *
 * >> AbstractItemPlot��Ҫ�������¼�������Ӧ��
 * 1. itemAxis��indexRangeChanged()�¼������¼�����ʱ����Ҫ���¼��㻭����ֵ��Χ������ѡ���Ե��ػ滭����
 * 2. itemAxis��axisMapChanged()�¼������¼�����ʱ����Ҫ���¼���canvas map���ػ滭����
 * 3. resizeEvent�������ĸ������Ტ�ػ档
 * 4. keyPressEvent�������û�����ѡ���Ե�������Ӧ��
 *
 * \todo GridLine Settings
 * \todo ֧�ֽ�ItemAxis���õ���������λ���ϡ�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractItemPlot : public TracePlot
{
    Q_OBJECT

public:
    explicit AbstractItemPlot(QWidget* parent = nullptr);

    virtual ~AbstractItemPlot();

    /*!
     * \brief ��������ʾ����ؼ��ϻ������ݣ���PlotCanvas��ͼʱ���á�
     *
     * \sa SimplePlotCanvas::drawCanvas
     * \param painter ��SimplePlotCanvas�ṩ�Ļ��ʡ�
     */
    virtual void drawCanvas(QWidget* canvas, QPainter *painter);

    /*!
     * \brief �ڻ����ϻ�������
     * \sa drawCanvas(QPainter*);
     * \param painter ������canvas�ϻ������ݵĻ���
     */
    virtual void drawCanvasGrid(QPainter* painter);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���������
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief caculateItemPos ����������ż�����Ӧ����canvas����ʾ��λ�á�
     * \param itemIndex Ҫ���������������е���š�
     * \return ������canvas��Ӧ����ʾ��λ�ã����������ӦΪ�������̼ۡ�
     *         ����QPoint(-1,-1)��ʾ�ƶ���item���ڵ�ǰ��ʾ�����С�
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
    // ��ͼԪ�أ�PlotableItem��
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief ���һ���µ�PlotableItem��������ͼ����ʾ��
     */
    virtual void addPlotableItem(PlotableItem* item);
    void clearPlotableItem();
    bool containsItem(PlotableItem* item);

    void attachUpdater(AbstractHistoryData* data);

    virtual double regionalMaxValue(int minIndex, int maxIndex) const;
    virtual double regionalMinValue(int minIndex, int maxIndex) const;

    /*!
     * \brief �趨canvas�����϶����͵ײ������Ŀհ������ʾ���ݷ�Χ�������Ⱥ�
     * ��ͼ����������ֵ��Χ�ı�����
     * \param factor �հ������ʾ���ݷ�Χ�������Ⱥͻ�ͼ����������ֵ��Χ�ı�����
     */
    void setVerticalExtraSpaceFactor(double factor = 0.07);

    /*!
     * \brief ��������������major tick�������ȡ�
     * \param space ����������major tick��������
     */
    void setMaxMajorTickSpace(int space);
    int getMaxMajotTickSpace() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // �������
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setGridLinePen(QPen pen);
    QPen getGridLinePen() const;

    virtual int dataSize() const;

    virtual QString titleLabelString() const;

    virtual void keyPressEvent(QKeyEvent *event) override;
    /*! \brief ȷ������ʾʱ���Ǵ�����ȷ״̬��*/
    virtual void showEvent(QShowEvent* event) override;
    /*! \brief �ı��Сʱ������ӳ���ϵ���ܸı䣬��˱���ʵʱ���¡�*/
    virtual void resizeEvent(QResizeEvent* event) override;

    /*!
     * \brief �趨��ǰѡ�е���������š�
     * \param index ���������
     */
    void setSelectedItemIndex(int index);
    int selectedItemIndex() const { return currentSelectedItemIndex; }

    virtual void updateAxes() override;

    /*! \brief �����������ᡣ */
    virtual void updateVerticalAxes(bool recaculate = false);
    /*! \brief ���º������ᡣ*/
    virtual void updateHorizontialAxes();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ItemAxis���
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief ������������ű�ǩ��item axis�ϵ��α��ƶ�ʱ�����α�����ʾ�ñ�ǩ��
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
     * \brief �������λ�ü���tracerλ�ã����Ҹ��µ�ǰѡ�е�����λ��
     *       ����currentSelectedItemIndex����������֤tracer����ָ��Ӱ��λ�á�
     * \param �����canvas�е�λ�á�
     * \return tracerλ�á�
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
    // ����
    /////////////////////////////////////////////////////////////////////////////////////////////////////

	void createTitleCache();
    bool isCacheEnabled() const;
    void enableCache();

private:
	QVector<QString> titleCache;
    bool cacheEnabled = false;

    int currentSelectedItemIndex;

    //canvas�ڿհ������ʾ���ݷ�Χ�������Ⱥͻ�ͼ����������ֵ��Χ�ı�����
    double verticalExtraSpaceFactor = 0.0;

    //����������major tick�������ȡ������᳤�ȱ仯ʱ�������¼���̶�ֵscaleDiv������
    //major tick��Ŀ��ʹmajor tick֮��Ŀ��ʼ�ղ��������ֵ��
    int maxMajorTickSpace = 60;

    // ��¼��ǰ�����ᱻ�̶ȷֳɵ���������
    mutable int majorIntervalCount;

    PlotTitleWidget* titleLabel; /*! titled plot canvas�ı���ؼ�*/

    // ������ʾ�����ݷ�Χ����ͨ��updateCanvasRange()��������Щ���ݡ�
    double maxValue;
    double minValue;

    //���������ĸ�������
    TitledPlotAxis* tTpa;
    TitledPlotAxis* bTpa;
    TitledPlotAxis* lTpa;
    TitledPlotAxis* rTpa;

    //���ڻ���grid�ߵ�QPen
    QPen gridLinePen;

    QwtScaleMap canvasMapArray[axisCnt];

    QList<AbstractHistoryData*> mUpdater;
};

}
#endif // ABSTRACTITEMPLOT_H
