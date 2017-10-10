#ifndef TRADINGCURVEPLOTPANEL_H
#define TRADINGCURVEPLOTPANEL_H

#include <QWidget>
#include <QSplitter>
#include <QList>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "tradingcurveplot.h"
#include "abstractplotlayout.h"

namespace TradingCurveWidgets{

/*!
 * \brief �Ѷ��TradingCurvePlot��ֱ���ŵ�һ���ؼ��У�������ͬ��
 * ʱ���ᣬͬʱ�����Ͱ����¼�������Ӧ��
 *
 * ����Ч��������ʾ��
 * +---+--------------------------------------------+
 * |   |Title Legend                                |
 * | A +--------------------------------------------+
 * | x |                                            |
 * | i |                                            |
 * | s |                  canvas                    |
 * |   |                                            |
 * |   |                                            |
 * +---+--------------------------------------------+
 * |   |Title Legend                                |
 * | A +--------------------------------------------+
 * | x |                                            |
 * | i |                                            |
 * | s |                  canvas                    |
 * |   |                                            |
 * |   |                                            |
 * +---+--------------------------------------------+
 * |   |Title Legend                                |
 * | A +--------------------------------------------+
 * | x |                                            |
 * | i |                                            |
 * | s |                  canvas                    |
 * |   |                                            |
 * |   |                                            |
 * +---+--------------------------------------------+
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT TradingCurvePlotPanel : public QFrame
{
    Q_OBJECT

public:
    explicit TradingCurvePlotPanel(QWidget* parent=0);
    virtual ~TradingCurvePlotPanel(){}

    void setData(AbstractHistoryData* historyData);

    /*!
     * \brief ���һ��TradingCurvePlot��
     * \param plot �Ѿ���ʼ����TradingCurvePlot��
     */
    TradingCurvePlotPanel& operator<<(TradingCurvePlot* plot);

    /*!
     * \brief ���һ��TradingCurvePlot��
     * \param plot �Ѿ���ʼ����TradingCurvePlot��
     */
    void addTradingCurvePlot(TradingCurvePlot* plot);

    TradingCurvePlot* getPlotAt(int index);

    /*!
     * \brief ��Ϊpublic�������Ա��ڸ���Ŀؼ�����á�
     */
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual bool event(QEvent *e) override;

    /*!
     * \brief ����������ؼ���ֱ�����ϵ����ſ�ȡ�
     * \param plot �����������Ļ�ͼ�ؼ�ָ�롣
     * \param axisId �������ڻ�ͼ�ؼ��е�λ�á�
     * \return ������ؼ���ֱ�����ϵ����ſ�ȡ�
     */
    virtual int axisDimHint(const AbstractPlot *plot, int axisId) const;

    void zoomIn();
    void zoomOut();

public slots:
    void onTracerMoved(QPoint tracerGlobalPos);
    void onTracerLeaved();
    void onDataLoaded();
    void onDataUpdated();
    void onDataAppended();

protected slots:
    void onBottomPlotCanvasMapChanged(int axisId);
    void onScaleLevelChanged();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event);
    /*****************************************************************
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    *****************************************************************/
private:
    QList<TradingCurvePlot*> plotList;
    QSplitter* splitter;
    TradingCurvePlot* bottomPlot = nullptr;
    const AbstractHistoryData* data = nullptr;
};

}

#endif // TRADINGCURVEPLOTPANEL_H
