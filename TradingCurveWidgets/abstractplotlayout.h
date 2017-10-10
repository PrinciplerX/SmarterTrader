#ifndef ABSTRACTPLOTLAYOUT_H
#define ABSTRACTPLOTLAYOUT_H

#include "tradingcurvewidgets_global.h"
#include "abstractplot.h"

/*!
 * \brief The AbstractPlotLayout class
 *
 * ��ظ��
 * 1. Spacing������ͼ�����֮��ļ�࣬��СΪ0��\sa setSpacing(), getSpacing()
 * 2. CanvasMargin��������ؼ��ͻ����ļ�ࡣ
 * 3. CanvasContentMargin�������������б߿�ģ����������ݺͻ����ؼ��߽��֮��Ĳ��־��ǻ����߿�����򡣲���ʱ
 * ����Ҫ���ǵ���һ�㣬����ᵼ��������ͻ���û�ж��롣�ر�ע�� CanvasContentMargin �� CanvasMargin ������
 *
 * \todo ����������Լ��ı߿���β��֣�
 * \todo Spacing��������ڲ��ֹ����У�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT AbstractPlotLayout
{
public:
    enum LayoutOption{
        // ��С�� CanvasMargin����ʱ������ͻ����ļ��������Ϊ0���������Ǹ��������BorderDist����������������
        // �������BorderDist��Ϊ0ʱ����������������ȫ�����������ᡣ
        MinimizeCanvasMargin,

        // ��� CanvasMargin����ʱ���������������Ϻ�������ļ��������ȣ����ҵ����ĸ������ʹ����С������
        // ���ѡ�MinimizeCanvasMargin��ʱ�����ֵ��
        MaximizeCanvasMargin,
    };

    explicit AbstractPlotLayout();
    virtual ~AbstractPlotLayout();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ������Ϣ
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    QRectF getScaleRect( int axis ) const;
    QRectF getCanvasRect() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ����ѡ��
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setSpacing( int );
    int getSpacing() const;

    void setAlignCanvasToScale( int axisId, bool );
    void setAlignCanvasToScales( bool );

    bool isCanvasAlignedToScale( int axisId ) const;

    void setLayoutOption(LayoutOption option);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���2��������canvas��
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setCanvasMargin( int margin, int axis = -1 );
    int getCanvasMargin( int axis ) const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���ֲ���
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void activate(const AbstractPlot* plot, const QRectF &rect);

    virtual int axisDimHint(const AbstractPlot *plot, int axisId) const;

    virtual QSize minimumSizeHint(const AbstractPlot * ) const;

    class LayoutData;

protected:

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���߱����಼�ֹ��ܵ����������ø�������ݳ�Ա
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setScaleRect( int axis, const QRectF &rect );
    void setCanvasRect( const QRectF &rect );

private:

    void invalidate();

    class PrivateData;

    PrivateData *d_data;

    QRectF axisRect[AbstractPlot::axisCnt];
    QRectF canvasRect;

    unsigned int canvasMargin[AbstractPlot::axisCnt];

    bool alignCanvasToScales[AbstractPlot::axisCnt];
    unsigned int spacing;

    LayoutOption layoutOption = LayoutOption::MinimizeCanvasMargin;
};

#endif // ABSTRACTPLOTLAYOUT_H
