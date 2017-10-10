#ifndef SPLITTERPLOTCANVAS_H
#define SPLITTERPLOTCANVAS_H

#include "tradingcurvewidgets_global.h"
#include "traceplotcanvas.h"
#include <QSplitter>
#include <QList>
#include <QMap>

class AbstractPlot;

namespace TradingCurveWidgets{

/*!
 * \brief ���岿����һ��QSplitter��canvas�������ֶ������ڲ����������ݵĴ�С��
 *
 * ��Ƹ������ҪĿ����ʵ�ֶ�����ú������ᣬ���ҿ����ֶ�������С��canvas��ͬ����ʾ��
 * ͬʱ���������ڴ������Զ��岻ͬcanvas�����������ʾ��
 * ����տ�ʼĬ�Ͻ���һ��traceCanvas�����ǿ�����ʹ��ʱ��������ؼ�������traceCanvas��
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT SplitterPlotCanvas : public QSplitter
{
    Q_OBJECT
public:
    SplitterPlotCanvas(Qt::Orientation orientation = Qt::Vertical, int canvasCount = 1, AbstractPlot* parent = nullptr);

    virtual ~SplitterPlotCanvas();

    void setPlot(AbstractPlot* plot);
    const AbstractPlot* getPlot() const;

    TracePlotCanvas* getMainCanvas();
    int getMainCanvasIndex();
    int getCanvasCount() const;
    int indexOfCanvas(QWidget* canvas) const;
    TracePlotCanvas *getCanvasAt(int index = 0);
    TracePlotCanvas* getTraceCanvas(int index = 0);

    void setCanvasBackground(const QBrush &brush);
    void setCanvasBackground(const QBrush &brush, int index);

    /*!
     * \brief ����tracer��λ��
     * \param tracerPos ���ڱ�������ϵ��tracer����
     */
    void setTracerLocalPos(QPoint tracerPos);
    QPoint getTracerLocalPos();

    void setTracerGlobalPos(QPoint tracerPos);
    QPoint getTracerGlobalPos();

    /*!
     * \brief ����Ѿ��뿪��Ӧ��Χ������αꡣ
     */
    void clearTracer();

    void appendCanvas();

signals:
    /*!
     * \brief Splitter�е�ĳ��canvas��С�����˱仯��
     * \param index ��С�����仯��canvas����š�
     */
    void canvasResized(int index);

    /*!
     * \brief �����ؼ���С�����˱仯�����е�canvas��Щ��С�ı䣬��һЩ���ܲ��䡣
     */
    void canvasResized();

    /*!
     * \param targetGlobalPos Ŀ������ʾ���ϵ�ȫ������λ�á����ź�һ���plot����󶨡�
     */
    void targetMoved(QPoint targetGlobalPos);

    /*!
     * \brief Ŀ���뿪canvas���򡣸��ź�һ���plot����󶨡�
     */
    void targetLeaved();

public slots:
    void replot();

protected:
    virtual bool eventFilter(QObject* obj, QEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    /*! canvas֮�����Ź�ϵ��canvas�����������Ϊ1�� */
    QVector<TracePlotCanvas*> canvasVector;

    AbstractPlot* plot;
};

inline const AbstractPlot *SplitterPlotCanvas::getPlot() const { return canvasVector[0]->plot(); }

inline TracePlotCanvas *SplitterPlotCanvas::getMainCanvas() { return canvasVector[getMainCanvasIndex()]; }

inline int SplitterPlotCanvas::getMainCanvasIndex() { return 0; }

inline int SplitterPlotCanvas::getCanvasCount() const { return canvasVector.size(); }

inline TracePlotCanvas *SplitterPlotCanvas::getCanvasAt(int index) { return canvasVector[index]; }

inline TracePlotCanvas *SplitterPlotCanvas::getTraceCanvas(int index) { return canvasVector[index]; }

//inline QPoint SplitterPlotCanvas::getTracerLocalPos() { return canvasVector[index]->mapFrom( tracerPos); }

//inline QPoint SplitterPlotCanvas::getTracerGlobalPos() { return mapToGlobal(tracerPos); }

}

#endif // SPLITTERPLOTCANVAS_H
