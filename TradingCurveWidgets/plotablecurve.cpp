#include "plotablecurve.h"
#include <cfloat>

PlotableCurve::PlotableCurve(Operation<double> *input, const QString &name, QPen curvePen) :
    mInput(input),
    pen(curvePen)
{
    setName(name);
}

PlotableCurve::PlotableCurve(const Operation<double> &input, const QString &name, QPen curvePen) :
    PlotableCurve(input.clone(), name, curvePen)
{

}

PlotableCurve::~PlotableCurve()
{
}

void PlotableCurve::setInput(Operation<double>* input)
{
    mInput = input;
}

void PlotableCurve::setInput(Operation<double> &input)
{
    mInput = input.clone();
}

double PlotableCurve::operator[] (int index) const
{
    return mInput->at(index);
}

double PlotableCurve::at(int index) const
{
    return mInput->at(index);
}

Operation<double> *PlotableCurve::operation()
{
    return mInput;
}

void PlotableCurve::draw(const QVector<int>& itemPosX,
		  TradingCurveWidgets::IndexRange indexRange,
		  int bodyWidth,
		  const QwtScaleMap& yMap,
		  QPainter* painter)
{
    Q_UNUSED(bodyWidth)
    if(size() <= indexRange.maxIndex()) return;

    painter->save();

    painter->setPen(pen);

    QVector<double> valueYList;
    valueYList.resize(indexRange.size());

    for(int i = 0; i < indexRange.size(); i++){
        valueYList[i] = yMap.transform(mInput->at(i+indexRange.minIndex));
    }

    QPainterPath path(QPoint(itemPosX.at(0),valueYList.at(0)));

    for(int i = 1;i<valueYList.size();i++){
        path.lineTo(itemPosX.at(i),valueYList.at(i));
    }
    painter->drawPath(path);

    painter->restore();
}

void PlotableCurve::setPen(const QPen &pen){
    this->pen = pen;
}

QPen PlotableCurve::getPen(){
    return pen;
}

double PlotableCurve::regionalMaxValue(int minIndex, int maxIndex) const
{
	minIndex = qBound(0, minIndex, size());
	maxIndex = qBound(0, maxIndex, size());

	double result = DBL_MIN;
	for (int i = minIndex; i<maxIndex; i++){
        result = qMax(result, mInput->at(i));
	}
	return result;
}

double PlotableCurve::regionalMinValue(int minIndex, int maxIndex) const
{
	minIndex = qBound(0, minIndex, size());
	maxIndex = qBound(0, maxIndex, size());

	double result = DBL_MAX;
	for (int i = minIndex; i<maxIndex; i++){
        result = qMin(result, mInput->at(i));
	}
	return result;
}

int PlotableCurve::size() const
{
    return mInput->size();
}

int PlotableCurve::tracerTarget(int itemIndex, const QwtScaleMap &yMap) const
{
    return yMap.transform(mInput->at(itemIndex));
}

QString PlotableCurve::titleLabelString(int index)
{
    if(index >= size()) return QString("越界");

    if(titleCache.size() == size()){
        return titleCache[index];
    }
    QString color = pen.color().name(QColor::HexRgb);

    QString format = QString("<font color=\"%1\">%2:%3</font>");

    return format.arg(color, name(), valueToString(mInput->at(index)));
}

void PlotableCurve::createTitleCache()
{
    titleCache.resize(size());

    //固定部分预先计算，提高速度
    QString color = pen.color().name(QColor::HexRgb);
    QString former = QString("<font color=\"") + color + QString("\">") + name() + QString(":");
    QString latter = QString("</font>");

    for(int i = 0; i < titleCache.size(); i++){
        titleCache[i] =  former +
                         valueToString(mInput->at(i)) +
                         latter;
    }
}

QString PlotableCurve::valueToString(double value)
{
    return QString::number(value, 'f', 3);
}

double PlotableCurve::regionalSum(int start, int offset) const
{
    double result = 0.0;
    int end = start + offset;
    for(int i = start; i < end; i++){
        result += at(i);
    }
    return result;
}
