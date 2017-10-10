#ifndef ABSTRACTTRACEAXIS_H
#define ABSTRACTTRACEAXIS_H

#include <QWidget>
#include "abstractaxis.h"

/*!
 * \brief ������α꣨Tracer�������ᡣ������һЩ�α������ᶼӦ�þ߱��Ĺ��ܡ�
 *
 * ����ڻ��� AbstractAxis ������һЩ���
 * 1. Tracer���α꣩���α�������������ʾ������ָʾ�������ϵ�һ��λ�ã��α����״����������ġ�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractTraceAxis : public AbstractAxis
{
    Q_OBJECT
public:
    explicit AbstractTraceAxis( QWidget *parent = NULL );
    explicit AbstractTraceAxis( AxisType axisType, QWidget *parent = NULL );

    void enableTracking(bool enable = true);
    inline bool isTrackingEnabled() const;

    /*!
     * \brief �����α�ı�������λ�á�
     * \param pos ���������߷����ϣ��α�����ꡣ���磬�����Ӧ�ĺ����꣬�����Ӧ�������ꡣ
     */
    void setTracerLocalPos(int pos);

    /*!
     * \brief ��ȡTracerλ��
     * \return Tracer�������᷽���ϵı�������
     */
    int getTracerPos() const;

signals:

public:
    virtual void onTargetLeaved();

protected:
    /*!
     * \brief �����α꣨Tracer����
     * \param painter ���ʡ�
     * \param value �α��������᷽���ϵı������ꡣ
     */
    virtual void drawTracer(QPainter* painter, int tracerPos) const;

    /*!
     * \brief ��������ͼ�ϵĹ̶��������ݣ��������α꣩��
     * \param painter ���ʡ�
     */
    virtual void drawAxis(QPainter* painter) const = 0;

    /*!
     * \brief ��������ͼ���ݡ�������ƹ����ǹ̶��ģ������Ȼ������������ݣ��ٻ����αꡣ��������
     * ���������ݸ����α�������
     * \param painter
     */
    virtual void drawContents( QPainter* painter ) const override final;

    /*!
     * \brief �������Զ����α�λ�øı�ʱ�Ķ�����
     * \param pos �α�λ�á�
     * \warning ��Ҫ�ڸú����������α�λ�ã��������ɺ����ݹ���ò�����ѭ����
     */
    virtual void onTargetMoved(int pos);
    virtual void onTargetMoved(QPoint globalPos);

private:

    int tracerPosition = -1;

    //! �Ƿ���ʾ�αꡣ
    bool tracking = false;
};

#endif // ABSTRACTTRACEAXIS_H
