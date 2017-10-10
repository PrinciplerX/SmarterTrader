#ifndef ABSTRACTTRACEPLOT_H
#define ABSTRACTTRACEPLOT_H

#include <QWidget>
#include "tradingcurvewidgets_global.h"
#include "abstractplot.h"
#include "abstracttraceaxis.h"

/*!
 * \brief ���ٹ��λ�õ�����ͼ�ؼ���
 *
 * ����꣨���ָ�룩λ���ڻ������ƶ�ʱ��
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractTracePlot : public AbstractPlot
{
    Q_OBJECT
public:
    explicit AbstractTracePlot(QWidget *parent = 0);

    virtual AbstractAxis *getAxisWidget( int axisId ) override final;
    virtual const AbstractAxis *getAxisWidget(int axisId) const override final;

    inline virtual AbstractTraceAxis* getTraceAxis( int axisId ) = 0;
    inline virtual const AbstractTraceAxis* getTraceAxis( int axisId ) const = 0;

signals:
    void targetMoved(QPoint mouseGlobalPos);
    void targetLeaved();

public slots:
    virtual void onTargetMove(QPoint mouseGlobalPos) = 0;
    virtual void onTargetLeave() = 0;

protected:
    /*!
     * \brief ����Tracer���������߽��㣩��λ�á�
     * \param mousePosIncanvas �����canvas����ϵ�е�����
     * \return ����ȫ������ϵ����Ļ����ϵ����Tracer��ʾλ�á�
     */
    virtual QPoint caculateAndUpdateTracerPos(QPoint globalPos) = 0;

private:

};

#endif // ABSTRACTTRACEPLOT_H
