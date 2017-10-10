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
 * \todo ���sizeHint��
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractPlot : public QFrame
{
public:
    /*! \brief ��������š�*/
    enum Axis{
        yLeft = 0,      //!< ��ߵ���������
        yRight = 1,     //!< �ұߵ���������
        xBottom = 2,    //!< ����ĺ�������
        xTop = 3,       //!< ����ĺ�������
        axisCnt = 4     //!< ����������
    };

    explicit AbstractPlot(QWidget* parent = nullptr);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // �������
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual AbstractPlotLayout *getPlotLayout() = 0;
    virtual const AbstractPlotLayout* getPlotLayout() const = 0;

    /*!
     * \brief ����Plot�ؼ���ǰ��С���������֣�����Plot�������ʾ����
     */
    virtual void updateLayout();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���1�������ᣨaxis / scale widget��
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * \brief ����ָ��λ�õ�������ؼ���
     * \param axisId ���������͡�
     * \return ������ڣ���Ȼ��һ���ɼ���������������ָ�룻��������ڣ�����nullptr��
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
     * \brief �ؽ������ᡣ�ú���ͨ����replot()���ã�Ŀ�������ػ�ǰ������������Ϣ��ʹ������ͻ�������ͬ����
     * \sa replot()��
     */
    virtual void updateAxes() = 0;

    /*!
     * \brief ����Ĭ�ϵ�������ؼ���
     * \param axisId ���������͡�
     */
    virtual void createDefaultAxis( int axisId ) = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���2��������canvas��
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual QWidget* getCanvas();

    void setCanvas( QWidget* widget );

    virtual const QWidget *getCanvas() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // �¼�����
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void resizeEvent( QResizeEvent* event );

    virtual bool event(QEvent* event);

    virtual bool eventFilter(QObject* object, QEvent* event );

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ��ͼ
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
