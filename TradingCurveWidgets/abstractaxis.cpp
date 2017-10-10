#include "abstractaxis.h"
#include <QStyle>
#include <QStyleOption>

AbstractAxis::AbstractAxis(QWidget *parent):
    QWidget(parent)
{
    type = AbstractAxis::LeftScale;
}

AbstractAxis::AbstractAxis(AxisType axisType, QWidget *parent):
    QWidget(parent),
    type(axisType)
{
}

AbstractAxis::AxisType AbstractAxis::getType() const {
    return type;
}

void AbstractAxis::adjustBorderDist(int start, int end, bool updateLayout)
{
    int oldStart = startBorderDist;
    int oldEnd = endBorderDist;

    if (start != startBorderDist || end != endBorderDist) {
        switch(getStartBorderDistPolicy()){
        case Fixed: break;
        case Minimum: startBorderDist = qMax(start, minimumStartBorderDist); break;
        case Maximum: startBorderDist = qMin(start, maximumStartBorderDist); break;
        case Preferred: startBorderDist = start; break;
        }

        switch(getEndBorderDistPolicy()){
        case Fixed: break;
        case Minimum: endBorderDist = qMax(end, minimumEndBorderDist); break;
        case Maximum: endBorderDist = qMin(end, maximumEndBorderDist); break;
        case Preferred: endBorderDist = end; break;
        }
    }

    if(startBorderDist!=oldStart || endBorderDist!=oldEnd){
        if(updateLayout){
            this->updateLayout();
        }
    }
}

void AbstractAxis::adjustBorderDist(bool updateLayout){
    int startHint;
    int endHint;
    getBorderDistHint(startHint, endHint);
    adjustBorderDist(startHint, endHint, updateLayout);
}

void AbstractAxis::getBorderDist(int &start, int &end) const {
    start = startBorderDist;
    end = endBorderDist;
}

int AbstractAxis::getStartBorderDist() const{
    return startBorderDist;
}

int AbstractAxis::getEndBorderDist() const{
    return endBorderDist;
}

void AbstractAxis::setMaximumStartBorderDist(int start){
    startBorderDistPolicy = Maximum;
    maximumStartBorderDist = start;
}

void AbstractAxis::setMaximumEndBorderDist(int end){
    endBorderDistPolicy = Maximum;
    maximumEndBorderDist = end;
}

void AbstractAxis::setMinimumStartBorderDist(int start){
    startBorderDistPolicy = Minimum;
    minimumStartBorderDist = start;
}

void AbstractAxis::setMinimumEndBorderDist(int end){
    endBorderDistPolicy = Minimum;
    minimumEndBorderDist = end;
}

int AbstractAxis::getMaximumStartBorderDist() const {
    return maximumStartBorderDist;
}

int AbstractAxis::getMaximumEndBorderDist() const {
    return maximumEndBorderDist;
}

int AbstractAxis::getMinimumStartBorderDist() const {
    return minimumStartBorderDist;
}

int AbstractAxis::getMinimumEndBorderDist() const {
    return minimumEndBorderDist;
}

void AbstractAxis::setFixedBorderDist(int start, int end){
    setFixedStartBorderDist(start);
    setFixedEndBorderDist(end);
}

void AbstractAxis::setFixedStartBorderDist(int start){
    startBorderDistPolicy = Fixed;
    startBorderDist = start;
}

void AbstractAxis::setFixedEndBorderDist(int end){
    endBorderDistPolicy = Fixed;
    endBorderDist = end;
}

void AbstractAxis::setBorderDistPolicy(AbstractAxis::BorderDistPolicy policy){
    startBorderDistPolicy = policy;
    endBorderDistPolicy = policy;
}

void AbstractAxis::setBorderDistPolicy(AbstractAxis::BorderDistPolicy start, AbstractAxis::BorderDistPolicy end){
    startBorderDistPolicy = start;
    endBorderDistPolicy = end;
}

void AbstractAxis::setStartBorderDistPolicy(AbstractAxis::BorderDistPolicy start){
    startBorderDistPolicy = start;
}

void AbstractAxis::setEndBorderDistPolicy(AbstractAxis::BorderDistPolicy end){
    endBorderDistPolicy = end;
}

AbstractAxis::BorderDistPolicy AbstractAxis::getStartBorderDistPolicy() const {
    return startBorderDistPolicy;
}

AbstractAxis::BorderDistPolicy AbstractAxis::getEndBorderDistPolicy() const {
    return endBorderDistPolicy;
}

bool AbstractAxis::isAxisEnabled() const{
    return axisEnabled;
}

void AbstractAxis::enableAxis(bool enable){
    axisEnabled = enable;
}

QRect AbstractAxis::rectHint(QPoint scaleRangeStart, int scaleRangeOffset, int dimension) const
{
    int start = getStartBorderDist();
    int end = getEndBorderDist();

    QRect rect;

    switch ( getType() ) {
    case AxisType::LeftScale:
        rect.setX(scaleRangeStart.x() - dimension);
        rect.setY(scaleRangeStart.y() - scaleRangeOffset - start);
        rect.setWidth(dimension);
        rect.setHeight(start + scaleRangeOffset + end);
        break;
    case AxisType::RightScale:
        rect.setX(scaleRangeStart.x());
        rect.setY(scaleRangeStart.y() - scaleRangeOffset - start);
        rect.setWidth(dimension);
        rect.setHeight(start + scaleRangeOffset + end);
        break;
    case AxisType::TopScale:
        rect.setX(scaleRangeStart.x() - start);
        rect.setY(scaleRangeStart.y() - dimension);
        rect.setWidth(start + scaleRangeOffset + end);
        rect.setHeight(dimension);
        break;
    case AxisType::BottomScale:
        rect.setX(scaleRangeStart.x() - start);
        rect.setY(scaleRangeStart.y());
        rect.setWidth(start + scaleRangeOffset + end);
        rect.setHeight(dimension);
        break;
    default:
        qDebug() << "AbstractAxis::rectHint error: invalid axis type.";
    }
    return rect;
}

void AbstractAxis::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED( event );
    updateLayout( false );
}

void AbstractAxis::paintEvent(QPaintEvent *event)
{
    QPainter painter( this );
    painter.setClipRegion( event->region() );

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    drawContents( &painter );
}

int AbstractAxis::caculateAxisLength(int startBorderDist, int backboneLength, int endBorderDist){
    return startBorderDist + endBorderDist + backboneLength;
}
