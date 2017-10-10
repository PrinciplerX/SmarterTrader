#ifndef ABSTRACTSCALEDRAW_H
#define ABSTRACTSCALEDRAW_H

#include "tradingcurvewidgets_global.h"
#include <qwt_scale_div.h>
#include <qwt_text.h>
#include <QFont>
#include <QPalette>
#include <QPainter>
#include <qwt_transform.h>
#include <qwt_scale_map.h>

/*!
 * \brief ������������ƹ��ߵĳ�����ࡣ
 *
 * AbstractScaleDraw���Ի��ƻ���������ֵ�������ᣬ��������ȿ��������ԣ�linear���ģ�Ҳ�����Ƕ�
 * ����logarithmic���ġ�
 *
 * �ڱ�ʾ������ֶ�ֵ��scale divison���� QwtScaleDiv ����ͨ�� setScaleDiv() ��ָ����������
 * ����ͨ����Ա���� draw() �����������ݡ�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractScaleDraw
{
public:
    /*!
     * \brief �����������
     * \sa enableComponent(), hasComponent
     */
    enum ScaleComponent
    {
        //! ���������ߣ�backbone�� = ��λ�̶ȣ�ticks�������ߡ�
        Backbone = 0x01,
        //! �̶ȣ�ticks��
        Ticks = 0x02,
        //! �̶Ƚű꣨labels��
        Labels = 0x04
    };

    //! �����������
    typedef QFlags<ScaleComponent> ScaleComponents;

    AbstractScaleDraw();
    virtual ~AbstractScaleDraw();

    void setScaleDiv( const QwtScaleDiv &s );
    const QwtScaleDiv& scaleDiv() const;
    QwtScaleDiv& scaleDiv();

    void setTransformation( QwtTransform * );
    const QwtScaleMap &scaleMap() const;
    QwtScaleMap &scaleMap();

    void enableComponent( ScaleComponent, bool enable = true );
    bool hasComponent( ScaleComponent ) const;

    void setTickLength( QwtScaleDiv::TickType, double length );
    double tickLength( QwtScaleDiv::TickType ) const;
    double maxTickLength() const;

    void setSpacing( double margin );
    double spacing() const;

    void setPenWidth( int width );
    int penWidth() const;

    virtual void draw( QPainter *, const QPalette & ) const;

    virtual QwtText label( double ) const;

    /*!
     * \brief ������չ��extent����
     *
     * ��չ�Ǵ����������ߣ�baseline����ʼ�����䴹ֱ�����ϣ��� scale draw ����ߵ����ص�ľ��롣������
     * Ϊ minimumExtent()��
     *
     * \param font �������ƿ̶Ƚű꣨tick labels�������塣
     * \return ��������
     * \sa setMinimumExtent(), minimumExtent()
     */
    virtual double extent( const QFont &font ) const = 0;

    void setMinimumExtent( double );
    double minimumExtent() const;

protected:

    /*!
     * \brief ����һ���̶ȣ�tick����
     * \param painter ���ʡ�
     * \param value �̶ȴ������ֵ��
     * \param len �̶ȵĳ��ȡ�
     * \sa drawBackbone(), drawLabel()
     */
    virtual void drawTick( QPainter *painter, double value, double len ) const = 0;

    /*!
     * \brief �����������ߣ�backbone����
     * \param painter ���ʡ�
     * \sa drawTick(), drawLabel()
     */
    virtual void drawBackbone( QPainter *painter ) const = 0;

    /*!
     * \brief ���ƿ̶ȣ�tick���ű꣨label����ֻ�����̶ȣ�major tick�����нűꡣ
     * \param painter ���ʡ�
     * \param value �ű���ʾ����ֵ��
     * \sa drawTick(), drawBackbone()
     */
    virtual void drawLabel( QPainter *painter, double value ) const = 0;

    void invalidateCache();
    const QwtText &tickLabel( const QFont &, double value ) const;

private:
    AbstractScaleDraw( const AbstractScaleDraw & );
    AbstractScaleDraw &operator=( const AbstractScaleDraw & );

    class PrivateData;
    PrivateData *d_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( AbstractScaleDraw::ScaleComponents )

#endif // ABSTRACTSCALEDRAW_H
