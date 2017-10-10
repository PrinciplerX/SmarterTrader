#ifndef FCHART_H
#define FCHART_H

#include <QAbstractSeries>
#include <AbstractDomain>
#include <QAbstractAxis>
#include <QVector>
#include <QChart>
#include <qchart_p.h>

#include "tradecharts_global.h"

SCHARTS_BEGIN_NAMESPACE

class FChart;
class FChartDataSet;
class FChartPresenter;
class FPanelLayout;

class FChartPrivate : public QChartPrivate
{
public:
    FChartPrivate(FChart *q);

    FChartPresenter* presenter();
    FChartDataSet* dataset();
};

/*!
 * \brief ��ʾһ��������ֱ����QChart�����ͼ���ࡣ
 *
 * FChart����̳���QChart���䱣���������Ϊ��QChart���е����������ÿ����ͼ�����˽�е���������������
 */
class TRADECHARTS_EXPORT FChart : public QChart
{
    Q_OBJECT
public:
    explicit FChart(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());

    virtual ~FChart();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ��ͼ�����
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    void append(QChart* chart);

    void insert(int index, QChart* chart);

    /*! �Ƴ���Ȼ��ɾ��ָ��λ���ϵ���ͼ��*/
    void remove(int index);

    /*! �Ƴ���Ȼ����ָ��λ������ͼ�������Ȩ��*/
    QChart* takeAt(int index);

    int count();

    QChart* chart(int index);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���ֹ���
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    FPanelLayout* panelLayout();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���з�Χ
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief ����ͼ������ʾ���ݵ���ŷ�Χ��
     * \param start �����š�
     * \param offset ��ŷ�Χ��С��
     */
    void setIndexRange(int start, int offset);

signals:
    void chartAdded(QChart* chart);
    void chartInserted(int index, QChart* chart);
    void chartRemoved(int index);

private:
    Q_DISABLE_COPY(FChart)

    QVector<QChart*> mCharts;
    FPanelLayout* mPanelLayout;
};

SCHARTS_END_NAMESPACE

#endif // FCHART_H
