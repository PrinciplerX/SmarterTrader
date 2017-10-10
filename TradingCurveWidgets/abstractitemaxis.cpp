#include "abstractitemaxis.h"
#include "abstractitemplot.h"
#include "itemscaledraw.h"

namespace TradingCurveWidgets{

AbstractItemAxis::AbstractItemAxis(AxisType align, QWidget *parent):
    TraceScaleWidget(align, parent)
{
    ItemScaleDraw* scaleDraw = new ItemScaleDraw(this);
    setScaleDraw(scaleDraw);
}

int AbstractItemAxis::caculateItemLocalPos(int index) const
{
    return scaleDraw()->scaleMap().transform(index);
}

int AbstractItemAxis::caculatePlotLocalPos(int index) const
{
    return scaleDraw()->scaleMap().transform(index + indexRange.minIndex);
}

QString AbstractItemAxis::getTracerLabel(int itemIndex) const
{
    if(itemIndex >= 0 && itemIndex < tracerLabelVector.size())
        return tracerLabelVector.at(itemIndex);
    return QString::number(itemIndex);
}

QString AbstractItemAxis::getTickLabel(int itemIndex) const
{
    QMap<int, QString>::const_iterator i = majorTickMap.find(itemIndex);
    if(i == majorTickMap.end()){
        return QString::number(itemIndex);
    }
    return i.value();
}

void AbstractItemAxis::updateScaleDiv()
{
    updateTickLabels();

    double min = scaleDraw()->scaleMap().s1();
    double max = scaleDraw()->scaleMap().s2();

    QList<double> ticks[QwtScaleDiv::NTickTypes];

    //这里是否可以提前计算直接返回？不行！
    QList<int> keys = majorTickMap.keys();
    for(int i : keys){
        ticks[QwtScaleDiv::MajorTick].append(i);
    }

    setScaleDiv(QwtScaleDiv(min, max, ticks));
}

void AbstractItemAxis::setTickLabels(const QMap<int, QString>& tickLabels) {
    majorTickMap = tickLabels;
}

void AbstractItemAxis::setTracerLabels(const QVector<QString> &tracerLabels){
    tracerLabelVector = tracerLabels;
}

const QMap<int, QString> &AbstractItemAxis::getMajorTickSeries() const {
    return majorTickMap;
}

const QVector<QString> &AbstractItemAxis::getItemLabelSeries() const {
    return tracerLabelVector;
}

void AbstractItemAxis::updateAxisMap()
{
    scaleDraw()->updateMap();
    QwtScaleMap& map = scaleDraw()->scaleMap();

    // sInterval表示对应于1个序号（index）差的位置的数值（value）差。
    double sInterval = (scaleDraw()->length() - 1.0) / (indexRange.offset - 1);
    // canvas两端表示的数值。
    double lowerBound = getIndexRange().minIndex - 0.5 / sInterval;
    double upperBound = scaleDraw()->length() / sInterval + lowerBound;
    map.setScaleInterval(lowerBound, upperBound);
    emit axisMapChanged();
}

void AbstractItemAxis::updateAxisDiv()
{
    updateTickLabels();
    scaleDraw()->scaleDiv().setInterval(scaleDraw()->scaleMap().s1(), scaleDraw()->scaleMap().s2());
}

QwtScaleMap AbstractItemAxis::caculateCanvasMap(const QWidget *canvas) const
{
    QwtScaleMap map = scaleDraw()->scaleMap();

    if(getType() == LeftScale || getType() == RightScale){
        // sInterval表示对应于1个序号（index）差的位置的数值（value）差。
        double pInterval = (canvas->height() - 1.0) / (indexRange.offset - 1);
        double sInterval = 1 / pInterval;
        // canvas两端表示的数值。
        double lowerBound = 0.5 * sInterval - 0.5 / sInterval;
        double upperBound = canvas->height() / sInterval + lowerBound;
        map.setScaleInterval(lowerBound, upperBound);
        map.setPaintInterval(0.0, canvas->height());
    }else{
        double pInterval = (canvas->width() - 1.0) / (indexRange.offset - 1);
        double sPixel = 1 / pInterval;

        double lowerBound = indexRange.minIndex - 0.5 * sPixel - 0.5;
        double upperBound = indexRange.minIndex + indexRange.offset - 1 + 0.5 * sPixel + 0.5;
        map.setScaleInterval(lowerBound, upperBound);
        map.setPaintInterval(0.0, canvas->width());
    }

    return map;
}

void AbstractItemAxis::setIndexRange(IndexRange newRange)
{
    qDebug() << "IndexRange(" << newRange.minIndex << ", " << newRange.maxIndex() << ")";
    if(indexRange == newRange) return;

    IndexRange oldRange = indexRange;
    indexRange = newRange;
    updateAxisMap();
    updateAxisDiv();
    emit indexRangeChanged(oldRange, newRange);
}

void AbstractItemAxis::drawTracer(QPainter *painter, int tracerPos) const
{
    double value = scaleDraw()->scaleMap().invTransform(tracerPos);

    int index = qRound(value);
    QwtText lbl = getTracerLabel(index);

    lbl.setRenderFlags( 0 );
    lbl.setLayoutAttribute( QwtText::MinimumLayout );

    ( void )lbl.textSize( painter->font() ); // initialize the internal cache

    if ( lbl.isEmpty() )
        return;

    QPointF pos = scaleDraw()->labelPosition( value );
    QSizeF labelSize = lbl.textSize( painter->font() );

    painter->setBrush(palette().highlight());

    //Alignment::BottomScale:
    //绘制指向label的短直线
    painter->drawLine(getTracerPos(),
                      0.0,
                      getTracerPos(),
                      scaleDraw()->maxTickLength());

    int margin = 6;

    //绘制label所在区域的边框
    painter->drawRect(pos.x() - labelSize.width()/2.0 - margin,
                      0.5*painter->pen().widthF() + scaleDraw()->maxTickLength(),
                      labelSize.width() + 2*margin,
                      height() - 0.5*painter->pen().widthF() - scaleDraw()->maxTickLength());

    ///////////////////////////////////////////////////////////////////
    //painter->drawRect(QRect( QPoint( 0, 0 ), labelSize.toSize() ));//
    ///////////////////////////////////////////////////////////////////

    const QTransform transform = scaleDraw()->labelTransformation( pos, labelSize );
    painter->save();
    painter->setWorldTransform( transform, true );

    painter->setPen(palette().highlightedText().color());
    painter->setBrush(palette().highlightedText());
    lbl.draw ( painter, QRect( QPoint( 0, 0 ), labelSize.toSize() ) );

    painter->restore();
}

void AbstractItemAxis::updateLayout(bool update)
{
    TraceScaleWidget::updateLayout(update);
    updateAxisMap();
    updateAxisDiv();
}

}
