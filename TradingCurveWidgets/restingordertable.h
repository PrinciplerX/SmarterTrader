#ifndef RESTINGORDERTABLE_H
#define RESTINGORDERTABLE_H

#include "tradingcurvewidgets_global.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <tradedata.h>

/*!
 * \brief 五档，十档挂单界面。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RestingOrderTable : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief 显示买卖盘固定档位数上的挂单情况。
     * \param n 档位数目。
     * \param parent 父控件。
     */
    explicit RestingOrderTable(QWidget *parent = nullptr);

signals:

public slots:
    void updateData(const QuoteData *data);

private:
    QTableWidget* orderTable;
};

#endif // RESTINGORDERTABLE_H
