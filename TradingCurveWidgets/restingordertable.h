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
 * \brief �嵵��ʮ���ҵ����档
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RestingOrderTable : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief ��ʾ�����̶̹���λ���ϵĹҵ������
     * \param n ��λ��Ŀ��
     * \param parent ���ؼ���
     */
    explicit RestingOrderTable(QWidget *parent = nullptr);

signals:

public slots:
    void updateData(const QuoteData *data);

private:
    QTableWidget* orderTable;
};

#endif // RESTINGORDERTABLE_H
