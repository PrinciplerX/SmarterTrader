#include "realtimevolume.h"

RealTimeVolume::RealTimeVolume(AbstractHistoryData *data, const QPen &pen) : Volume(data)
{
    this->pen = pen;
}

QPen RealTimeVolume::getRaisePen() const
{
    static QPen raisePen;
    static ColorTheme theme = InvalidTheme;
    if(theme != getTheme()){
        theme = getTheme();
        switch(theme){
        case Dark:
            raisePen.setColor(Qt::red);
            break;
        case Light:
            raisePen.setColor(Qt::red);
            break;
        case HighContrastWhite:
            raisePen.setColor(QColor("#F0F888"));
            break;
        case HighContrastBlack:
            raisePen.setColor(Qt::white);
            break;
        }
    }
    return raisePen;
}

QPen RealTimeVolume::getFallPen() const
{
    static QPen fallPen;
    static ColorTheme theme = InvalidTheme;
    if(theme != getTheme()){
        theme = getTheme();
        switch(theme){
        case Dark:
            fallPen.setColor(Qt::black);
            break;
        case Light:
            fallPen.setColor(Qt::darkGreen);
            break;
        case HighContrastWhite:
            fallPen.setColor(QColor("#54FCFC"));
            break;
        case HighContrastBlack:
            fallPen.setColor(Qt::white);
            break;
        }
    }
    return fallPen;
}

QString RealTimeVolume::titleLabelString(int index){
    return QString("<font color=\"") + pen.color().name(QColor::HexRgb) + QString("\">") +
           QString(":") + QString::number(this->at(index)) +
           QString("</font>");
}
