#ifndef TRACEPLOTCANVAS_H
#define TRACEPLOTCANVAS_H
#include <QMouseEvent>
#include "simpleplotcanvas.h"
#include <QPen>

namespace TradingCurveWidgets{

class TRADINGCURVEWIDGETSSHARED_EXPORT TracePlotCanvas : public SimplePlotCanvas
{
    Q_OBJECT

public:
    /*!
     * \brief ���ڴ洢��λ���ߵ����ͣ���ɫ�����ѡ��Լ���λ�߽��㴦��ͼ�����ࡣ
     */
    struct TracerSetting{
        QPen pen;
    };

    explicit TracePlotCanvas( AbstractPlot * = NULL );
    explicit TracePlotCanvas( TracerSetting setting, AbstractPlot *parent=NULL);

    inline void setTracerSetting(TracePlotCanvas::TracerSetting setting){
        tracerSetting=setting;
    }
    virtual void setTracking(bool enable);

    /*!
     * \brief ����tracer��λ��
     * \param tracerPos ���ڱ�������ϵ��tracer����
     */
    void setTracerPos(QPoint tracerPos);
    inline QPoint getTracerPos(){return tracerPos;}
    /*!
     * \brief ����Ѿ��뿪��Ӧ��Χ������αꡣ
     */
    void clearTracer();

signals:
    /*!
     * \param targetGlobalPos Ŀ������ʾ���ϵ�ȫ������λ�á�
     */
    void targetMoved(QPoint targetGlobalPos);
    /*!
     * \brief Ŀ���뿪canvas����
     */
    void targetLeaved();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

private:
    QPoint tracerPos = QPoint(-1,-1);
    TracerSetting tracerSetting;
    bool tracking;
};

}

#endif // TRACEPLOTCANVAS_H
