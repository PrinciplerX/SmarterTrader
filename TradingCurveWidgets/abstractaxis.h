#ifndef ABSTRACTAXIS_H
#define ABSTRACTAXIS_H

#include "tradingcurvewidgets_global.h"
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

/*!
 * \brief ����������������������һЩһ�������ᶼ��Ҫ�߱��Ĺ��ܡ�
 *
 * ����ڻ��� QWidget ������һЩ���
 * 1. BorderDist��start��end�������������ߣ�backbone��������˷ֱ�������ؼ��������᷽�����˱߽�ľ��롣
 * 2. Dimension��dim��������tick���������ᴹֱ����������ʵĿ�ȡ�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractAxis : public QWidget
{
    Q_OBJECT

public:
    /*! \brief ���������͡�*/
    enum AxisType{
        //! ��ߵ���������
        LeftScale,

        //! �ұߵ���������
        RightScale,

        //! ����ĺ�������
        BottomScale,

        //! ����ĺ�������
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
     * \brief BorderDist�Ĵ�С�������ԡ�
     */
    enum BorderDistPolicy{
        //! �̶���С��BorderDist��
        Fixed,
        //! BorderDist��һ����Сֵ�������ܡ�
        Minimum,
        //! BorderDist��һ�����ֵ��
        Maximum,
        //! ʹ����������ֵȷ��ʵ�ʵ�BorderDist��
        Preferred
    };

    /*!
     * \brief ����BorderDist�����ý���͵�ǰ��BorderDistPolicy�йء�
     * \param start startBorderDist��
     * \param end endBorderDist��
     */
    void adjustBorderDist(int start, int end, bool updateLayout = true);
    /*!
     * \brief ��������BorderDistPolicy���¼���BorderDist��
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
    // Enable���Ƿ��� AbstractPlot ����ʾ����
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isAxisEnabled() const;
    void enableAxis(bool enable = true);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // �ⲿ���֣�Plot����������
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*! \return ����tick���������ᴹֱ����������ʵĿ�ȡ�*/
    virtual int dimHint() const = 0;

    /*!
     * \brief ����������ؼ���scaleRange����������������Ӧcanvas��������������ͳ��ȣ��������������ʾ����
     * \param scaleRangeStart scaleRange����㣬������������ؼ��߽�(��ӽ�backBone�ı߽�)������λ��������ؼ��ⲿ��
     * \param scaleRangeOffset scaleRange�ĳ��ȡ�
     * \param dimension �����ֹ��������㣬�������ʵ��dimension����һ������dimHint()����
     * \return ������ؼ�����ʾ����
     */
    virtual QRect rectHint(QPoint scaleRangeStart, int scaleRangeOffset, int dimension) const;

    /*!
     * \return �������ܳ��ȡ�
     */
    static int caculateAxisLength(int startBorderDist, int backboneLength, int endBorderDist);

protected:

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // �ڲ�������غ���
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void resizeEvent( QResizeEvent *event ) override;

    virtual void updateLayout( bool update = true ) = 0;

    virtual void drawContents( QPainter* painter ) const = 0;

    /*!
     * \brief �����²���ʱ������BorderDist�ĺ�����
     * \param start startBorderDist��
     * \param end endBorderDist��
     */
    virtual void getBorderDistHint( int &start, int &end ) const = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ��ͼ��غ���
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
