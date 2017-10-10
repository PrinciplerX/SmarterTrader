#ifndef QUOTATIONLABEL_H
#define QUOTATIONLABEL_H

#include <QWidget>
#include <QLabel>
#include <tradedata.h>
#include "mainframe_global.h"
#include <titledlabel.h>

class QuotationLabel : public QWidget
{
    Q_OBJECT
public:
    explicit QuotationLabel(QWidget *parent = 0);
    virtual ~QuotationLabel();

    void setData(const QString& current,
                 const QString& change,
                 const QString& changeRate,
                 const QString& open,
                 const QString& high,
                 const QString& low,
                 const QString& lastClose,
                 const QString& turnoverRate);

    void clearData();
    static void setLabelTextColor(QLabel *label, const QColor& color);
    static QString labelColorfulText(const QString& text,const QString& colorName);
    static QString formatDoubleString(const QString& num, char format, int precision);

public slots:
    void updateData(const QuoteData* data);

private:
    QLabel* currentLabel;
    QLabel* changeRateLabel;
    QLabel* changeLabel;
    TitledLabel* openLabel;
    TitledLabel* highLabel;
    TitledLabel* lowLabel;
    TitledLabel* volumeLabel;
    TitledLabel* turnoverLabel;
    TitledLabel* turnoverRateLabel;

    const AbstractRealTimeData* mData;
};

#endif // QUOTATIONLABEL_H
