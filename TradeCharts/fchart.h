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
 * \brief 表示一个可以竖直叠加QChart对象的图表类。
 *
 * FChart本身继承自QChart，其保存的坐标轴为子QChart共有的坐标轴对象。每个子图表对象私有的组件由其自身管理。
 */
class TRADECHARTS_EXPORT FChart : public QChart
{
    Q_OBJECT
public:
    explicit FChart(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());

    virtual ~FChart();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 子图表管理
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    void append(QChart* chart);

    void insert(int index, QChart* chart);

    /*! 移除，然后删除指定位置上的子图表。*/
    void remove(int index);

    /*! 移除，然后释指定位置上子图表的所有权。*/
    QChart* takeAt(int index);

    int count();

    QChart* chart(int index);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 布局管理
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    FPanelLayout* panelLayout();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 序列范围
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief 设置图表上显示数据的序号范围。
     * \param start 起点序号。
     * \param offset 序号范围大小。
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
