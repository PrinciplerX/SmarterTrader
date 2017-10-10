#ifndef TRADINGCURVEPLOT_H
#define TRADINGCURVEPLOT_H

#include "tradingcurvewidgets_global.h"
#include "traceplot.h"
#include <QKeyEvent>
#include "qwt_date_scale_draw.h"
#include "qwt_date_scale_engine.h"
#include "abstractitemplot.h"
#include "tradingcurveitemaxis.h"
#include <QResizeEvent>
#include <tradedata.h>
#include <QList>
#include <QLabel>

#define BASIC_SCALE_LEVEL_COUNT 11

namespace TradingCurveWidgets{

/*!
 * \brief ������ʾ�۸����Ƶ�ר�ÿؼ������Ի��Ƽ۸�����ͼ�Լ������ļ���������ͼ��
 *
 * ���������Ҫ��ʾ��OHLC���ݡ�����ǰ���ÿؼ�����֪���������ķ�Χ���Ա��ܹ���
 * ȫ��ʾ���ض�ʱ����ڵļ۸�����ͼ�����û�������ʾ��ʱ���ʱ�������»���ǰ����
 * Ҫ���������ᡣ
 *
 * ������ؼ�����ֵ���ʱ���ᣩֻ������ʾ����ʾ��Χ�Լ�ʱ��̶�ֵ��label���ļ���
 * ���ֶ��ɱ��ฺ��
 *
 * �����������ʾ��ΧӦ���ܹ�����Ŀǰ��������������ͼ���ڵ��������ݡ�
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT TradingCurvePlot : public AbstractItemPlot
{
    Q_OBJECT
public:
    explicit TradingCurvePlot(QWidget* parent=nullptr);

    virtual ~TradingCurvePlot();

    virtual void keyPressEvent(QKeyEvent* event) override;

    virtual AbstractItemAxis *getItemAxis() override;
    virtual const AbstractItemAxis *getItemAxis() const override;

    virtual int bodyWidth() const override;

    void setHistoryData(AbstractHistoryData* data);

    void zoomIn();
    void zoomOut();

public slots:
    void onDataUpdated();
    void onDataLoaded();

private:
    TradingCurveItemAxis* itemAxis;
    AbstractHistoryData* data = nullptr;
};

}
#endif // TRADINGCURVEPLOT_H
