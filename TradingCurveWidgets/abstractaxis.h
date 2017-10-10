#ifndef ABSTRACTAXIS_H
#define ABSTRACTAXIS_H

#include "tradingcurvewidgets_global.h"
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

/*!
 * \brief 抽象的坐标轴组件，包含了一些一般坐标轴都需要具备的功能。
 *
 * 相对于基类 QWidget 新增的一些概念：
 * 1. BorderDist（start，end）：坐标轴轴线（backbone）组件两端分别到坐标轴控件（坐标轴方向）两端边界的距离。
 * 2. Dimension（dim）：返回tick方向（坐标轴垂直方向）上最合适的宽度。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractAxis : public QWidget
{
    Q_OBJECT

public:
    /*! \brief 坐标轴类型。*/
    enum AxisType{
        //! 左边的纵坐标轴
        LeftScale,

        //! 右边的纵坐标轴
        RightScale,

        //! 下面的横坐标轴
        BottomScale,

        //! 上面的横坐标轴
        TopScale
    };

    explicit AbstractAxis( QWidget *parent = NULL );
    explicit AbstractAxis( AxisType axisType, QWidget *parent = NULL );

    Q_PROPERTY(bool axisEnabled READ isAxisEnabled WRITE enableAxis)
    Q_PROPERTY(int startBorderDist READ getStartBorderDist)
    Q_PROPERTY(int endBorderDist READ getEndBorderDist)

    Q_PROPERTY(BorderDistPolicy startBorderDistPolicy READ getStartBorderDistPolicy)
    Q_PROPERTY(BorderDistPolicy endBorderDistPolicy READ getEndBorderDistPolicy)
    Q_PROPERTY(int maximumEndBorderDist READ getMaximumEndBorderDist WRITE setMaximumEndBorderDist)
    Q_PROPERTY(int minimumEndBorderDist READ getMinimumEndBorderDist WRITE setMinimumEndBorderDist)
    Q_PROPERTY(int maximumStartBorderDist READ getMaximumStartBorderDist WRITE setMaximumStartBorderDist)
    Q_PROPERTY(int minimumStartBorderDist READ getMinimumStartBorderDist WRITE setMinimumStartBorderDist)

    AxisType getType() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Border Distance
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief BorderDist的大小调整策略。
     */
    enum BorderDistPolicy{
        //! 固定大小的BorderDist。
        Fixed,
        //! BorderDist有一个最小值，不接受。
        Minimum,
        //! BorderDist有一个最大值。
        Maximum,
        //! 使用期望的数值确定实际的BorderDist。
        Preferred
    };

    /*!
     * \brief 设置BorderDist，设置结果和当前的BorderDistPolicy有关。
     * \param start startBorderDist。
     * \param end endBorderDist。
     */
    void adjustBorderDist(int start, int end, bool updateLayout = true);
    /*!
     * \brief 根据自身BorderDistPolicy重新计算BorderDist。
     */
    void adjustBorderDist(bool updateLayout = true);

    void getBorderDist(int& start, int& end) const;
    int getStartBorderDist() const;
    int getEndBorderDist() const;

    void setMaximumStartBorderDist(int start);
    void setMaximumEndBorderDist(int end);
    void setMinimumStartBorderDist(int start);
    void setMinimumEndBorderDist(int end);
    int getMaximumStartBorderDist() const;
    int getMaximumEndBorderDist() const;
    int getMinimumStartBorderDist() const;
    int getMinimumEndBorderDist() const;

    void setFixedBorderDist(int start, int end);
    void setFixedStartBorderDist(int start);
    void setFixedEndBorderDist(int end);

    void setBorderDistPolicy(BorderDistPolicy policy);
    void setBorderDistPolicy(BorderDistPolicy start, BorderDistPolicy end);
    void setStartBorderDistPolicy(BorderDistPolicy start);
    void setEndBorderDistPolicy(BorderDistPolicy end);
    BorderDistPolicy getStartBorderDistPolicy() const;
    BorderDistPolicy getEndBorderDistPolicy() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Enable（是否在 AbstractPlot 中显示。）
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isAxisEnabled() const;
    void enableAxis(bool enable = true);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 外部布局（Plot）辅助函数
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*! \return 返回tick方向（坐标轴垂直方向）上最合适的宽度。*/
    virtual int dimHint() const = 0;

    /*!
     * \brief 给定坐标轴控件的scaleRange（坐标轴上用来对应canvas中坐标的区域）起点和长度，计算坐标轴的显示区域。
     * \param scaleRangeStart scaleRange的起点，紧贴着坐标轴控件边界(最接近backBone的边界)，并且位于坐标轴控件外部。
     * \param scaleRangeOffset scaleRange的长度。
     * \param dimension 经布局管理器计算，坐标轴的实际dimension（不一定等于dimHint()）。
     * \return 坐标轴控件的显示区域。
     */
    virtual QRect rectHint(QPoint scaleRangeStart, int scaleRangeOffset, int dimension) const;

    /*!
     * \return 坐标轴总长度。
     */
    static int caculateAxisLength(int startBorderDist, int backboneLength, int endBorderDist);

protected:

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 内部布局相关函数
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void resizeEvent( QResizeEvent *event ) override;

    virtual void updateLayout( bool update = true ) = 0;

    virtual void drawContents( QPainter* painter ) const = 0;

    /*!
     * \brief 在重新布局时，计算BorderDist的函数。
     * \param start startBorderDist。
     * \param end endBorderDist。
     */
    virtual void getBorderDistHint( int &start, int &end ) const = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 绘图相关函数
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void paintEvent(QPaintEvent* event ) override;

private:
    bool axisEnabled = true;

    int startBorderDist = 0;
    int endBorderDist = 0;
    int maximumStartBorderDist = 0;
    int minimumStartBorderDist = 0;
    int maximumEndBorderDist = 0;
    int minimumEndBorderDist = 0;

    BorderDistPolicy startBorderDistPolicy = Preferred;
    BorderDistPolicy endBorderDistPolicy = Preferred;

    AxisType type;
};


#endif // ABSTRACTAXIS_H
