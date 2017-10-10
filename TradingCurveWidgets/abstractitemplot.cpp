#include "plotableitem.h"
#include "abstractitemplot.h"
#include "abstractitemaxis.h"
#include "titledplotaxis.h"
#include <qwt_scale_map.h>
#include <cfloat>

namespace TradingCurveWidgets{

AbstractItemPlot::AbstractItemPlot(QWidget *parent) :
    TracePlot(parent),
    titleLabel(new PlotTitleWidget("untitled")),
    tTpa(new TitledPlotAxis(AbstractAxis::AxisType::TopScale, this)),
    bTpa(new TitledPlotAxis(AbstractAxis::AxisType::BottomScale, this)),
    lTpa(new TitledPlotAxis(AbstractAxis::AxisType::LeftScale, this)),
    rTpa(new TitledPlotAxis(AbstractAxis::AxisType::RightScale, this))
{
    //canvas (包括了title widget)
    //setCanvasBackground(QBrush(Qt::black));
    setCanvasBackground(QColor("#D4D0C8"));
    //setCanvasBackground(QColor("#F0F0F0"));

    this->setTitleWidget(titleLabel);

    QFont font = this->font();
    font.setPointSize(10);
    this->setFont(font);

    this->setScaleWidget(SimplePlot::Axis::xTop, tTpa);
    this->setScaleWidget(SimplePlot::Axis::xBottom, bTpa);
    this->setScaleWidget(SimplePlot::Axis::yLeft, lTpa);
    this->setScaleWidget(SimplePlot::Axis::yRight, rTpa);

    //配色
    QPalette p = this->palette();
    p.setColor(QPalette::WindowText, Qt::darkGray);
    tTpa->setPalette(p);
    bTpa->setPalette(p);
    lTpa->setPalette(p);
    rTpa->setPalette(p);
    this->setPalette(p);

    connect(tTpa, SIGNAL(scaleMapChanged()), this, SLOT(onAxisMapChanged()));
    connect(bTpa, SIGNAL(scaleMapChanged()), this, SLOT(onAxisMapChanged()));
    connect(lTpa, SIGNAL(scaleMapChanged()), this, SLOT(onAxisMapChanged()));
    connect(rTpa, SIGNAL(scaleMapChanged()), this, SLOT(onAxisMapChanged()));

    //设置默认绘制水平网格线的画笔
    QPen pen(QColor("#9d0a0f"));
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 1 << space;
    pen.setDashPattern(dashes);
    setGridLinePen(pen);
}

AbstractItemPlot::~AbstractItemPlot(){
    qDeleteAll(plotableItemList);
}

void AbstractItemPlot::drawCanvas(QWidget *canvas, QPainter *painter){
    Q_UNUSED(canvas)
    drawCanvasGrid(painter);

    if (dataSize() == 0) return;

    const QwtScaleMap& scaleMap = getCanvasMap(yRight);

    if(isnan(scaleMap.p1()) || isnan(scaleMap.p2()) ||
       isnan(scaleMap.s1()) || isnan(scaleMap.s2()) ||
       isinf(scaleMap.p1()) || isinf(scaleMap.p2()) ||
       isinf(scaleMap.s1()) || isinf(scaleMap.s2())) return;

    IndexRange range = getIndexRange();

    if (range.minIndex > dataSize() - 1) return;

    if (range.maxIndex() > dataSize() - 1) {
        range.offset = dataSize() - range.minIndex;
    }

    int vectorSize = range.offset;
    if(vectorSize <= 0){
        return;
    }

    // 绘制元素中心所在的位置的横坐标QVector。
    QVector<int> itemPosXVector;
    itemPosXVector.resize(vectorSize);
    for (int i = 0; i < vectorSize; i++) {
        itemPosXVector[i] = caculateItemPos(i + range.minIndex);
    }

    //绘制PlotableItem
    for (auto curve : plotableItemList) {
        curve->draw(itemPosXVector, range, bodyWidth(), scaleMap, painter);
    }

    //更新标题控件信息
    titleLabel->setText(titleLabelString());
}

void AbstractItemPlot::drawCanvasGrid(QPainter *painter){
    painter->save();

    painter->setPen(gridLinePen);

    SimpleScaleWidget* axis = getSimpleScaleWidget(AbstractPlot::yRight);
    const QwtScaleDiv& div = axis->scaleDraw()->scaleDiv();
    QwtScaleMap map = canvasMap(AbstractPlot::yRight);
    const QList<double>& tickList = div.ticks(QwtScaleDiv::MajorTick);

    for(double value : tickList){
        double y = map.transform(value);
        //painter->drawText(0, y - 4, QString::number(value, 'f', 2));
        painter->drawLine(QPointF(0, y), QPointF(canvasWidget()->width() - 1, y));
    }

    painter->restore();
}

int AbstractItemPlot::caculateItemPos(int index) const {
    return getCanvasMap(xBottom).transform(index);
}

int AbstractItemPlot::caculateItemIndex(int x) const {
    return qBound(getItemAxis()->getIndexRange().minIndex,
                  qRound(getCanvasMap(xBottom).invTransform(x)),
                  getItemAxis()->getIndexRange().maxIndex());
}

int AbstractItemPlot::caculatePlotPos(int index) const {
    return getCanvasMap(xBottom).transform(index + getIndexRange().minIndex);
}

int AbstractItemPlot::caculatePlotIndex(int x) const {
    return getCanvasMap(xBottom).invTransform(x) - getIndexRange().minIndex;
}

QPoint AbstractItemPlot::caculateTargetPos(int index) const {
    return QPoint(caculateItemPos(index),
                  plotableItemList.at(0)->tracerTarget(index, canvasMap(AbstractPlot::yRight)));
}

void AbstractItemPlot::caculateScaleDivRange(double &min, double &max) const
{

}

void AbstractItemPlot::addPlotableItem(PlotableItem *item){
    if(!item) return;
    plotableItemList.push_back(item);
    setSelectedItemIndex(dataSize() - 1);
    onPlotableItemAdded(item);//需要放到前面
    updateAxes();

    if(isCacheEnabled())
        createTitleCache();
}

void AbstractItemPlot::clearPlotableItem()
{
    plotableItemList.clear();
}

bool AbstractItemPlot::containsItem(PlotableItem* item)
{
    return plotableItemList.contains(item);
}

void AbstractItemPlot::attachUpdater(AbstractHistoryData *data)
{
    if (!mUpdater.contains(data)) {
        mUpdater.append(data);
        connect(data, SIGNAL(dataAppended()), this, SLOT(onDataAppended()));
        connect(data, SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));
        connect(data, SIGNAL(dataLoaded()), this, SLOT(onDataLoaded()));
    }
}

bool AbstractItemPlot::scaleDivNeedUpdate() const
{
    int count = getCanvas()->height() / maxMajorTickSpace;
    count = qMax(2, count);

    return majorIntervalCount != count;
}

double AbstractItemPlot::regionalMaxValue(int minIndex, int maxIndex) const {
    int data_size = dataSize();

    if (data_size == 0) {
        return 100.0;
    }

    maxIndex = qMin(data_size, maxIndex);
    minIndex = qMax(0,         minIndex);

    double maxValue = DBL_MIN;
    for (PlotableItem* item : plotableItemList) {
        maxValue = qMax(maxValue, item->regionalMaxValue(minIndex, maxIndex));
    }
    return maxValue;
}

double AbstractItemPlot::regionalMinValue(int minIndex, int maxIndex) const {
    int data_size = dataSize();

    if (data_size == 0) {
        return 0.0;
    }

    maxIndex = qMin(data_size, maxIndex);
    minIndex = qMax(0,         minIndex);

    double minValue = DBL_MAX;
    for (PlotableItem* item : plotableItemList) {
        minValue = qMin(minValue, item->regionalMinValue(minIndex, maxIndex));
    }
    return minValue;
}

void AbstractItemPlot::setVerticalExtraSpaceFactor(double factor){
    verticalExtraSpaceFactor = factor;
}

void AbstractItemPlot::setMaxMajorTickSpace(int space){
    maxMajorTickSpace = space;
}

int AbstractItemPlot::getMaxMajotTickSpace() const{
    return maxMajorTickSpace;
}

void AbstractItemPlot::setGridLinePen(QPen pen){
    gridLinePen = pen;
}

QPen AbstractItemPlot::getGridLinePen() const
{
    return gridLinePen;
}

int AbstractItemPlot::dataSize() const {
    return plotableItemList.isEmpty() ? 0 : plotableItemList.at(0)->size();
}

QString AbstractItemPlot::titleLabelString() const {
    if (selectedItemIndex() < 0 || dataSize() <= 0){
        return QString("没有数据");
    }
    if(selectedItemIndex() >= dataSize()){
        return QString("越界");
    }

    if(isCacheEnabled())
        return titleCache[selectedItemIndex()];

    QString result;
    for(PlotableItem* item : plotableItemList){
        result += item->titleLabelString(selectedItemIndex()) + QString(" ");
    }
    return result;
}

void AbstractItemPlot::keyPressEvent(QKeyEvent *event){
    TracePlot::keyPressEvent(event);
    if (dataSize() == 0) return;
    switch (event->key()) {
    case Qt::Key_Home:
        if(dataSize() > 0)
            setSelectedItemIndex(getIndexRange().minIndex);
        setTracerCanvasPosDirectly(caculateTargetPos(selectedItemIndex()));
        break;
    case Qt::Key_End:
        if(dataSize() > 0)
            setSelectedItemIndex(getIndexRange().maxIndex() - 1);
        setTracerCanvasPosDirectly(caculateTargetPos(selectedItemIndex()));
        break;
    case Qt::Key_Left:{
        setSelectedItemIndex( qMax(selectedItemIndex() - 1, 0) );
        setTracerCanvasPosDirectly(caculateTargetPos(selectedItemIndex()));
        if(!getItemAxis()->getIndexRange().contains(selectedItemIndex())){
            IndexRange range = getItemAxis()->getIndexRange();
            range.moveForward();
            getItemAxis()->setIndexRange(range);
        }
    }
        break;
    case Qt::Key_Right:
        setSelectedItemIndex( qMin(selectedItemIndex() + 1, dataSize() - 1) );
        setTracerCanvasPosDirectly(caculateTargetPos(selectedItemIndex()));
        if(!getItemAxis()->getIndexRange().contains(selectedItemIndex())){
            IndexRange range = getItemAxis()->getIndexRange();
            range.moveBackward();
            getItemAxis()->setIndexRange(range);
        }
        break;
    }
}

void AbstractItemPlot::showEvent(QShowEvent *event){
    TracePlot::showEvent(event);
    updateHorizontialAxes();
    updateVerticalAxes();
    updateHorizontialAxes();
    replot();
}

void AbstractItemPlot::resizeEvent(QResizeEvent *event)
{
    TracePlot::resizeEvent(event);
    // 这里需要确保首先更新水平的itemAxis。
    //! \todo 把控制方式改为itemAxis接收自身的resizeEvent事件，重新计算indexRange后通知itemPlot的方式。这
    //! 种方式更加明确了itemAxis和itemPlot的映射关系，将resizeEvent处理转化为对两个核心事件的处理。
    if(event->oldSize().width() != event->size().width()){
        getItemAxis()->updatePlotIndexRange();
        updateHorizontialAxes();
    }
    if(event->oldSize().height() != event->size().height()){
        updateVerticalAxes();
    }
    replot();
}

QPoint AbstractItemPlot::caculateAndUpdateTracerPos(QPoint mousePosInCanvas){
    if(dataSize() == 0) return mousePosInCanvas;
    int selectedItemIndex = caculateItemIndex(mousePosInCanvas.x());
    setSelectedItemIndex(selectedItemIndex);
    return QPoint(caculateItemPos(selectedItemIndex), mousePosInCanvas.y());
}

void AbstractItemPlot::setSelectedItemIndex(int index){
    currentSelectedItemIndex = index;
    updateTitleLabel();
    onSelectedItemChanged();
}

void AbstractItemPlot::updateTitleLabel(){
    titleLabel->setText(titleLabelString());
}

void AbstractItemPlot::updateCanvasRange()
{
    IndexRange range = getIndexRange();
    if(range.isEmpty()) return;

    maxValue = regionalMaxValue(range.minIndex, range.end());
    minValue = regionalMinValue(range.minIndex, range.end());

    if(isAxisEnabled(yRight)){
        setAxisScale(yRight, minValue, maxValue);
        getTraceScaleWidget(yRight)->updateScaleDiv();
        getTraceScaleWidget(yRight)->update();
    }
    if(isAxisEnabled(yLeft)){
        setAxisScale(yLeft, minValue, maxValue);
        getTraceScaleWidget(yLeft)->updateScaleDiv();
        getTraceScaleWidget(yLeft)->update();
    }
}

void AbstractItemPlot::updateAxes(){
    updateAxesRange();
    updateAxesDiv();
    setCanvasMap(getItemAxis()->caculateCanvasMap(getCanvas()), axisType2Id(getItemAxis()->getType()));
}

void AbstractItemPlot::updateVerticalAxes(bool recaculate)
{
    if(dataSize() == 0) return;

    if(recaculate){
        updateCanvasRange();
    }

    if(isAxisEnabled(yRight)){
        getTraceScaleWidget(yRight)->updateScaleDiv();
    }
    if(isAxisEnabled(yLeft)){
        getTraceScaleWidget(yLeft)->updateScaleDiv();
    }
}

void AbstractItemPlot::updateHorizontialAxes()
{
}

void AbstractItemPlot::setItemLabelSeries(const QVector<QString> &series){
    getItemAxis()->setTracerLabels(series);
}

void AbstractItemPlot::setMajorTickSeries(const QMap<int, QString> &series){
    getItemAxis()->setTickLabels(series);
}

void AbstractItemPlot::addTitleWidget(QWidget *widget)
{
    titleLabel->addWidget(widget);
}

void AbstractItemPlot::onAxisMapChanged()
{
    AbstractAxis* sender = (AbstractAxis*)QObject::sender();
    Axis axisId = axisType2Id(sender->getType());
    if (axisId == xBottom) {
        int i = 0;
    }
    setCanvasMap(getTraceScaleWidget(axisId)->caculateCanvasMap(getCanvas()), axisId);
}

void AbstractItemPlot::onIndexRangeChanged(IndexRange oldRange, IndexRange newRange)
{
    //! \todo 这里有优化空间。可以通过缓存以前的计算结果来优化性能。维基百科：memoization
    Q_UNUSED(oldRange);
    Q_UNUSED(newRange);

    updateCanvasRange();
}

void AbstractItemPlot::onDataLoaded()
{
    qDebug() << "[AbstractItemPlot] [onDataLoaded]";
    replot();
}

void AbstractItemPlot::onDataAppended()
{
    IndexRange range = getIndexRange();
    qDebug() << "[AbstractItemPlot] [onDataAppended] range(" << range.minIndex << "," <<  range.maxIndex() << "), dataSize() =" << dataSize();
    if (range.maxIndex() + 2 == dataSize()) {
        range.moveForward();
        qDebug() << "[AbstractItemPlot] [onDataAppended] set range(" << range.minIndex << "," <<  range.maxIndex() << ")";
        getItemAxis()->setIndexRange(range);
        updateVerticalAxes();
    } else if (range.maxIndex() + 1 >= dataSize()) {
        updateVerticalAxes(true);
    }
    replot();
}

void AbstractItemPlot::onDataUpdated()
{
    qDebug() << "[AbstractItemPlot] [onDataUpdated]";
    replot();
}

void AbstractItemPlot::updateAxesRange(){
    if(dataSize() == 0) return;
    updateCanvasRange();
}

void AbstractItemPlot::updateAxesDiv(){
    if(dataSize() == 0) return;

    //! \todo 合并这两个部分。
    getItemAxis()->updateScaleDiv();
    getItemAxis()->updateAxisDiv();

    if(getItemAxis()->scaleDraw()->orientation() == Qt::Horizontal){
        //! \todo 不必每次都重新计算QwtScaleDiv，可以等到当tick数发生变化时再重新计算。
        if(!scaleDivNeedUpdate()) return;

        getTraceScaleWidget(yRight)->updateScaleDiv();
        getTraceScaleWidget(yLeft)->updateScaleDiv();

    }else{
        if(!scaleDivNeedUpdate()) return;

        getTraceScaleWidget(xTop)->updateScaleDiv();
        getTraceScaleWidget(xBottom)->updateScaleDiv();
    }
}

void AbstractItemPlot::onPlotableItemAdded(PlotableItem *item){
    Q_UNUSED(item);
    getItemAxis()->onPlotableItemAdded();
}

void AbstractItemPlot::onSelectedItemChanged(){
    getItemAxis()->onSelectedItemChanged();
}

void AbstractItemPlot::createTitleCache()
{
    titleCache.resize(dataSize());
    for (int i = 0; i < titleCache.size(); i++){
        titleCache[i].clear();
        for(PlotableItem* item : plotableItemList){
            titleCache[i] += item->titleLabelString(i) + QString(" ");
        }
    }
}

bool AbstractItemPlot::isCacheEnabled() const {
    return cacheEnabled;
}

void AbstractItemPlot::enableCache()
{
    createTitleCache();
    cacheEnabled = true;
}

IndexRange AbstractItemPlot::getIndexRange() const { return getItemAxis()->getIndexRange(); }
void AbstractItemPlot::setIndexRange(IndexRange range) { getItemAxis()->setIndexRange(range); }

void AbstractItemPlot::setCanvasMap(const QwtScaleMap &map, int axisId) {
    canvasMapArray[axisId] = map;
    if (axisId == xBottom) {
        /*
        qDebug() << QString("CanvasMap: s(%1, %2), p(%3, %4)").arg(QString::number(map.s1()),
                                                                       QString::number(map.s2()),
                                                                       QString::number(map.p1()),
                                                                       QString::number(map.p2()));
                                                                       */
    }
    emit canvasMapChanged(axisId);
}

const QwtScaleMap &AbstractItemPlot::getCanvasMap(int axisId) const { return canvasMapArray[axisId]; }

QwtScaleMap &AbstractItemPlot::getCanvasMap(int axisId) { return canvasMapArray[axisId]; }

}
