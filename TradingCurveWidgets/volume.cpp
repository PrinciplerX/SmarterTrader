#include "volume.h"
#include <climits>

const QString Volume::mCategory = QString("VOL");

Volume::Volume(AbstractHistoryData *data):
    TechniqueIndicator(mCategory),
    vol(VOL(data)),
    mHistoryData(data)
{
    setName(QString("总手"));

    // 输入
    CLOSE close(data);
    OPEN open(data);

    gain = new BufferedOperationWrapper<bool>(IF<bool>((close == open), (close > REF<double>(close, 1)), (close > open)));
    initData();
}

void Volume::draw(const QVector<int>& itemPosX, TradingCurveWidgets::IndexRange indexRange, int bodyWidth, const QwtScaleMap &yMap, QPainter *painter)
{
    int x;
    int volumeY;
    int baseY = yMap.transform(0);

    QPen raisePen = getRaisePen();
    QPen fallPen = getFallPen();
    QPen stillPen = getStillPen();
    QBrush raiseBrush = getRaiseBrush();
    QBrush fallBrush = getFallBrush();
    QBrush stillBrush = getStillBrush();

    painter->save();

    int maxIndex = indexRange.maxIndex() + 1;
    for(int i = indexRange.minIndex; i < maxIndex; i++){
        x = itemPosX.at(i-indexRange.minIndex);
        volumeY = yMap.transform(vol.at(i));

        if(gain->at(i)){
            painter->setPen(raisePen);
            painter->setBrush(raiseBrush);
        }else{
            painter->setPen(fallPen);
            painter->setBrush(fallBrush);
        }

        painter->drawRect(x-(bodyWidth-1)/2, volumeY, bodyWidth - 1, baseY - volumeY);
    }

    painter->restore();
}

QPen Volume::getRaisePen() const
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
            raisePen.setColor(Qt::black);
            break;
        case HighContrastBlack:
            raisePen.setColor(Qt::white);
            break;
        }
    }
    return raisePen;
}

QBrush Volume::getRaiseBrush() const
{
    static QBrush raiseBrush(Qt::SolidPattern);
    static ColorTheme theme = InvalidTheme;
    if(theme != getTheme()){
        theme = getTheme();
        switch(theme){
        case Dark:
            raiseBrush.setColor(Qt::black);
            break;
        case Light:
            raiseBrush.setColor(Qt::white);
            break;
        case HighContrastWhite:
            raiseBrush.setColor(Qt::white);
            break;
        case HighContrastBlack:
            raiseBrush.setColor(Qt::black);
            break;
        }
    }
    return raiseBrush;
}

QPen Volume::getFallPen() const
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
            fallPen.setColor(Qt::black);
            break;
        case HighContrastBlack:
            fallPen.setColor(Qt::white);
            break;
        }
    }
    return fallPen;
}

QBrush Volume::getFallBrush() const
{
    static QBrush stillBrush(Qt::SolidPattern);
    static ColorTheme theme = InvalidTheme;
    if(theme != getTheme()){
        theme = getTheme();
        switch(theme){
        case Dark:
            stillBrush.setColor(Qt::black);
            break;
        case Light:
            stillBrush.setColor(Qt::darkGreen);
            break;
        case HighContrastWhite:
            stillBrush.setColor(Qt::black);
            break;
        case HighContrastBlack:
            stillBrush.setColor(Qt::white);
            break;
        }
    }
    return stillBrush;
}

QPen Volume::getStillPen() const
{
    static QPen stillPen;
    static ColorTheme theme = InvalidTheme;
    if(theme != getTheme()){
        theme = getTheme();
        switch(theme){
        case Dark:
            stillPen.setColor(Qt::red);
            break;
        case Light:
            stillPen.setColor(Qt::red);
            break;
        case HighContrastWhite:
            stillPen.setColor(Qt::white);
            break;
        case HighContrastBlack:
            stillPen.setColor(Qt::white);
            break;
        }
    }
    return stillPen;
}

QBrush Volume::getStillBrush() const
{
    static QBrush stillBrush(Qt::SolidPattern);
    static ColorTheme theme = InvalidTheme;
    if(theme != getTheme()){
        theme = getTheme();
        switch(theme){
        case Dark:
            stillBrush.setColor(Qt::black);
            break;
        case Light:
            stillBrush.setColor(Qt::white);
            break;
        case HighContrastWhite:
            stillBrush.setColor(Qt::white);
            break;
        case HighContrastBlack:
            stillBrush.setColor(Qt::white);
            break;
        }
    }
    return stillBrush;
}

double Volume::regionalMaxValue(int minIndex, int maxIndex) const {
    return vol.regionalMaxValue(minIndex, maxIndex);
}

double Volume::regionalMinValue(int minIndex, int maxIndex) const {
    Q_UNUSED(minIndex)
    Q_UNUSED(maxIndex)
    return 0.0;
}

void Volume::onDataLoaded()
{
    initData();
}

void Volume::onDataUpdated()
{
    gain->onUpdated();
}

void Volume::onDataAppended()
{
    gain->onAppended();
}

QString Volume::dateString(int index){
    return mHistoryData->at(index).date;
}

QString Volume::titleLabelString(int index){
    if(titleCache.size()-1 < index){
        QString nameValueTemplate = name() + QString(":%1");
        QString redTemplate = QString("<font color=\"%1\">%2</font>").arg(QColor(Qt::red).name(QColor::HexRgb), nameValueTemplate);
        QString whiteTemplate = QString("<font color=\"%1\">%2</font>").arg(QColor(Qt::green).name(QColor::HexRgb), nameValueTemplate);
        return (gain->at(index) ? redTemplate : whiteTemplate).arg(QString::number(vol.at(index)));
    }
    return titleCache[index];
}

void Volume::createTitleCache()
{
    titleCache.resize(size());

    //固定部分预先计算，提高速度
    QString redFormer = QString("<font color=\"%1\">%2:").arg(QColor(Qt::red).name(QColor::HexRgb), name());
    QString whiteFormer = QString("<font color=\"%1\">%2:").arg(QColor(Qt::green).name(QColor::HexRgb), name());

    QString latter = QString("</font>");

    for(int i = 0; i < titleCache.size(); i++){
        titleCache[i] =  (gain->at(i) ? redFormer : whiteFormer) +
                         QString::number(vol.at(i), 'f', 3) +
                         latter;
    }
}

void Volume::initData(){
    gain->onLoaded();
}

AbstractHistoryData *Volume::historyData()
{
    return mHistoryData;
}
