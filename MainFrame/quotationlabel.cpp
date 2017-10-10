#include "quotationlabel.h"
#include <QGridLayout>
#include <QPalette>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QDateTime>
#include <QStringList>

QuotationLabel::QuotationLabel(QWidget *parent)
    : QWidget(parent)
    , currentLabel(new QLabel(tr("5178.19")))
    , changeRateLabel(new QLabel(tr("-45.4%")))
    , changeLabel(new QLabel(tr("-951.08")))
    , openLabel(new TitledLabel(tr("开"),tr("5178.19")))
    , highLabel(new TitledLabel(tr("高"),tr("5178.19")))
    , lowLabel(new TitledLabel(tr("低"),tr("2827.11")))
    , turnoverRateLabel(new TitledLabel(tr("换"),tr("xxxx.xx")))
    , volumeLabel(new TitledLabel(tr("量"), tr("xxxx.xx")))
    , turnoverLabel(new TitledLabel(tr("额"), tr("xxxx.xx")))
{
    //设定字体
    QFont currentLabelFont = currentLabel->font();
    currentLabelFont.setBold(true);
    currentLabelFont.setPointSize(19);
    currentLabel->setFont(currentLabelFont);
    currentLabel->setContentsMargins(0,0,0,0);

    QFont changeRateLabelFont = changeRateLabel->font();
    changeRateLabelFont.setPointSize(12);
    changeRateLabel->setFont(changeRateLabelFont);
    changeRateLabel->setContentsMargins(0,0,0,2); // 和currentLabel底部对齐

    QFont changeLabelFont = changeLabel->font();
    changeLabelFont.setPointSize(12);
    changeLabel->setFont(changeLabelFont);
    changeLabel->setContentsMargins(0,0,0,2);// 和currentLabel底部对齐

    //自适应内容长度
    /**************************************
    dateLabel->setMinimumWidth(30);
    timeLabel->setMinimumWidth(30);
    currentLabel->setMinimumWidth(30);
    openLabel->setMinimumWidth(30);
    highLabel->setMinimumWidth(30);
    lowLabel->setMinimumWidth(30);
    volumeLabel->setMinimumWidth(30);
    turnoverLabel->setMinimumWidth(30);
    **************************************/

    QHBoxLayout* priceLayout = new QHBoxLayout();
    priceLayout->addWidget(currentLabel, 0, Qt::AlignBottom);
    priceLayout->addStretch();
    priceLayout->addWidget(changeRateLabel, 0, Qt::AlignBottom);
    priceLayout->addStretch();
    priceLayout->addWidget(changeLabel, 0, Qt::AlignBottom);
    priceLayout->addStretch();

    QGridLayout* gridLayout=new QGridLayout();
    gridLayout->addWidget(highLabel,0,0);
    gridLayout->addWidget(lowLabel,1,0);
    gridLayout->addWidget(openLabel,0,1);
    gridLayout->addWidget(turnoverRateLabel,1,1);
    gridLayout->addWidget(volumeLabel, 0, 2);
    gridLayout->addWidget(turnoverLabel, 1, 2);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(priceLayout);
    mainLayout->addLayout(gridLayout);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    this->setLayout(mainLayout);
    this->setContentsMargins(0,0,0,0);
}

QuotationLabel::~QuotationLabel() {}

void QuotationLabel::setData(const QString& current,
                             const QString& change,
                             const QString& changeRate,
                             const QString& open,
                             const QString& high,
                             const QString& low,
                             const QString& lastClose,
                             const QString& turnoverRate){
    turnoverRateLabel->setContent(turnoverRate);

    if(changeRate.isEmpty()) return;

    if(changeRate==QString("----")){
        currentLabel->setText(labelColorfulText(QString("----"),QString("black")));
        lowLabel->setContent(labelColorfulText(QString("----"),QString("black")));
        changeRateLabel->setText(labelColorfulText(QString("----"),QString("black")));
        highLabel->setContent(labelColorfulText(QString("----"),QString("black")));
        openLabel->setContent(labelColorfulText(QString("----"),QString("black")));
    }else if(changeRate.at(0)=='-'){
        double openPrice=open.toDouble();
        double highPrice=high.toDouble();
        double lastClosePrice=lastClose.toDouble();

        changeLabel->setText(labelColorfulText(change, QString("#00A800")));
        currentLabel->setText(labelColorfulText(current,QString("#00A800")));
        lowLabel->setContent(labelColorfulText(low,QString("#00A800")));
        changeRateLabel->setText(labelColorfulText(changeRate,QString("#00A800")));
        highLabel->setContent(labelColorfulText(high,QString(highPrice>lastClosePrice?"red":"#00A800")));
        openLabel->setContent(labelColorfulText(open,QString(openPrice>lastClosePrice?"red":"#00A800")));
    }else{
        double openPrice=open.toDouble();
        double lowPrice=low.toDouble();
        double lastClosePrice=lastClose.toDouble();

        changeLabel->setText(labelColorfulText(change, QString("red")));
        currentLabel->setText(labelColorfulText(current,QString("red")));
        highLabel->setContent(labelColorfulText(high,QString("red")));
        changeRateLabel->setText(labelColorfulText(changeRate,QString("red")));
        lowLabel->setContent(labelColorfulText(low,QString(lowPrice>lastClosePrice?"red":"#00A800")));
        openLabel->setContent(labelColorfulText(open,QString(openPrice>lastClosePrice?"red":"#00A800")));
    }
}

void QuotationLabel::clearData(){
    QString emptyData("----");
    setData(emptyData,
            emptyData,
            emptyData,
            emptyData,
            emptyData,
            emptyData,
            emptyData,
            emptyData);
}

void QuotationLabel::setLabelTextColor(QLabel* label,const QColor& color){
    QPalette palette =label->palette();
    palette.setColor(QPalette::WindowText,color);
    label->setPalette(palette);
}

QString QuotationLabel::labelColorfulText(const QString& text,const QString& colorName){
    return QString("<font color='")+colorName+QString("'>")+text+QString("</font>");
}

QString QuotationLabel::formatDoubleString(const QString& num,char format,int precision){
    bool result;
    double d=num.toDouble(&result);
    if(!result)
        return num;

    return QString::number(d,format,precision);
}

void QuotationLabel::updateData(const QuoteData *data)
{
    setData(data->current,
            data->change,
            data->changeRate,
            data->open,
            data->high,
            data->low,
            data->lastClose,
            data->turnoverRate);
}
