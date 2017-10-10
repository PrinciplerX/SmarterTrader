#ifndef REALTIMEPLOT_H
#define REALTIMEPLOT_H

#include "tradingcurvewidgets_global.h"
#include "abstractitemplot.h"
#include "realtimeitemaxis.h"
#include <QLabel>
#include <QWidget>
#include <QMap>

namespace TradingCurveWidgets {

/*!
 * \brief ���ڻ���ʵʱ����ͼ��
 *
 * RealTimePlot���ǽ���������ȫ�����Ƶ�canvas�ϣ����Ҿ������������߷ֶ���������ʱ���ᡣ
 * RealTimePlot�Է���Ϊ��С��λ��ʱ�������κ�һ���ܹ�ӳ�䵽ĳһ���ӡ�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RealTimePlot : public AbstractItemPlot
{
    Q_OBJECT

public:

    explicit RealTimePlot(RealTimeItemAxis::UpdateDirection direction = RealTimeItemAxis::OldestLeft, QWidget* parent = nullptr);

    virtual ~RealTimePlot(){}

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���������
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual AbstractItemAxis *getItemAxis() override;
    virtual const AbstractItemAxis *getItemAxis() const override;

    void setPlotableIndexCount(int count);
    int getPlotableIndexCount() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ����
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void drawCanvasGrid(QPainter* painter);
    bool isVerticalGridVisible();
    void hideVerticalGrid();
    void showVerticalGrid();
    void setVerticalGrid(const QMap<int, QPen>& verticalGrid);

protected:

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ��ͼԪ�أ�PlotableItem��
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int bodyWidth() const override;

private:

    /*! \brief canvas����ʾ������������*/
    int plotableIndexCount;

    bool verticalGridVisible = true;

    QMap<int, QPen> verticalGrid;

    // ����ţ���ʾ����ͼ������ʾ�����������С��������PlotItem�е���š�����
    // UpdateDirection������ſ��ܶ�Ӧcanvas����ߣ�Ҳ���ܶ�Ӧcanvas���ұߡ�
    int baseIndex = 0;

    RealTimeItemAxis* axis;
};

}

#endif // REALTIMEPLOT_H
