#ifndef ABSTRACTPLOT_H
#define ABSTRACTPLOT_H

#include "tradingcurvewidgets_global.h"
#include <QWidget>
#include "abstractaxis.h"
#include <QFrame>
#include <QList>
#include <QVariant>
#include <QPointer>

class AbstractPlotLayout;
class AbstractAxis;
class QwtScaleEngine;
class QwtScaleDiv;
class QwtTextLabel;

/*!
 * \brief The AbstractPlot class
 * \todo 完成sizeHint。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractPlot : public QFrame
{
public:
    /*! \brief 坐标轴序号。*/
    enum Axis{
        yLeft = 0,      //!< 左边的纵坐标轴
        yRight = 1,     //!< 右边的纵坐标轴
        xBottom = 2,    //!< 下面的横坐标轴
        xTop = 3,       //!< 上面的横坐标轴
        axisCnt = 4     //!< 坐标轴数量
    };

    explicit AbstractPlot(QWidget* parent = nullptr);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 布局相关
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual AbstractPlotLayout *getPlotLayout() = 0;
    virtual const AbstractPlotLayout* getPlotLayout() const = 0;

    /*!
     * \brief 根据Plot控件当前大小来调整布局（调整Plot组件的显示）。
     */
    virtual void updateLayout();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 组件1：坐标轴（axis / scale widget）
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief 返回指定位置的坐标轴控件。
     * \param axisId 坐标轴类型。
     * \return 如果存在（虽然不一定可见），返回坐标轴指针；如果不存在，返回nullptr。
     */
    virtual AbstractAxis *getAxisWidget( int axisId ) = 0;
    virtual const AbstractAxis *getAxisWidget(int axisId) const = 0;

    void disableAxis( int axisId );
    void enableAxis( int axisId, bool tf = true );

    bool isAxisExist( int axisId ) const;

    bool isAxisEnabled( int axisId ) const;

    static bool axisValid( int axisId );

    void setAxesPalette(const QPalette& pal);
    void setAxisPalette(int axisId, const QPalette& pal);

    static Axis axisType2Id(AbstractAxis::AxisType type);

    /*!
     * \brief 重建坐标轴。该函数通常由replot()调用，目的是在重绘前更新坐标轴信息，使坐标轴和画布内容同步。
     * \sa replot()。
     */
    virtual void updateAxes() = 0;

    /*!
     * \brief 创建默认的坐标轴控件。
     * \param axisId 坐标轴类型。
     */
    virtual void createDefaultAxis( int axisId ) = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 组件2：画布（canvas）
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual QWidget* getCanvas();

    void setCanvas( QWidget* widget );

    virtual const QWidget *getCanvas() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 事件处理
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void resizeEvent( QResizeEvent* event );

    virtual bool event(QEvent* event);

    virtual bool eventFilter(QObject* object, QEvent* event );

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 绘图
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void drawCanvas( QWidget* canvas, QPainter * painter ) = 0;

    void enableAutoReplot( bool flag = true );
    bool isAutoReplot() const;

public slots:
    virtual void replot();

    void autoRefresh();

private:
    QPointer<QWidget> canvas;

    bool autoReplot;
};

#endif // ABSTRACTPLOT_H
