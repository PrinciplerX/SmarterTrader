#include "abstractplot.h"
#include "abstractplotlayout.h"
#include <QApplication>

AbstractPlot::AbstractPlot(QWidget *parent):
    QFrame(parent)
{
    //! \todo ������ֱ��Ϊ����Plot���������С���Ƿ����
    this->setStyleSheet(QString("font: 12px;"));
}

void AbstractPlot::updateLayout()
{
    AbstractPlotLayout* layout = getPlotLayout();

    layout->activate( this, contentsRect() );

    QRect scaleRect[AbstractPlot::axisCnt];
    for ( int axisId = 0; axisId < axisCnt; axisId++ )
        scaleRect[axisId] = layout->getScaleRect( axisId ).toRect();
    QRect canvasRect = layout->getCanvasRect().toRect();

    // �����ɼ��ؼ��Ĵ�С��������ʾ������

    for ( int axisId = 0; axisId < axisCnt; axisId++ )
    {
        if ( isAxisEnabled( axisId ) )
        {
            getAxisWidget( axisId )->setGeometry( scaleRect[axisId] );

#if 1
            if ( axisId == xBottom || axisId == xTop )
            {
                // do we need this code any longer ???

                QRegion r( scaleRect[axisId] );
                if ( isAxisEnabled( yLeft ) )
                    r = r.subtracted( QRegion( scaleRect[yLeft] ) );
                if ( isAxisEnabled( yRight ) )
                    r = r.subtracted( QRegion( scaleRect[yRight] ) );
                r.translate( -scaleRect[ axisId ].x(),
                    -scaleRect[axisId].y() );

                getAxisWidget( axisId )->setMask( r );
            }
#endif
            if ( !getAxisWidget( axisId )->isVisibleTo( this ) )
                getAxisWidget( axisId )->show();
        }
        else
            getAxisWidget( axisId )->hide();
    }

    canvas->setGeometry( canvasRect );
}

/*!
  Resize and update internal layout
  \param e Resize event
*/
void AbstractPlot::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    updateLayout();
}

/*!
 * \brief ��ӶԲ�������Ĵ���
 * \param event �¼���
 * \return �μ�QFrame::event()��
 */
bool AbstractPlot::event(QEvent *event)
{
    bool ok = QFrame::event( event );
    switch ( event->type() ){
        case QEvent::LayoutRequest:
            updateLayout();
            break;
        case QEvent::PolishRequest:
            update();
            break;
        default:;
    }
    return ok;
}

/*!
 * \brief �¼���������
 *
 * ����ͼ��Plot���ؼ��������������¼���
 *
 * - QEvent::Resize
 * ������margin�������������С��
 *
 * - QEvent::ContentsRectChange
 * ������Ϣ��Ҫ���¼��㡣
 *
 * \param onject Ҫ���˵Ķ���
 * \param event �¼�
 * \return �μ�QFrame::eventFilter()��
 * \sa updateLayout()
 */

bool AbstractPlot::eventFilter(QObject *object, QEvent *event)
{
    if ( object = canvas ){
        if( (event->type() == QEvent::Resize) || (event->type() == QEvent::ContentsRectChange)){
            updateLayout();
        }
    }
    return QFrame::eventFilter( object, event );
}

/*!
 * \brief ���û��߽����Զ��ػ棨auto replot��ѡ�
 *
 * ����Զ��ػ�ѡ����ã���Plot����ͨ��������Ա��������ʽ���£�update implicitly������Ϊ��
 * ���ܻ�ȽϺ�ʱ�����Խ��齫��ѡ��رղ����ڱ�Ҫʱ��ʽ��explicity������replot()���ػ档
 *
 * �Զ��ػ�ѡ��Ĭ���ǹرգ�false���ģ�����ζ���û��������replot()��ʹ�仯�ɼ���
 *
 * \param flag \c true ���� \c false��Ĭ��Ϊ \c true��
 * \sa replot()
 */
void AbstractPlot::enableAutoReplot(bool flag){
    autoReplot = flag;
}

/*!
 * \return ����Զ��ػ棨auto replot��ѡ���Ѿ����ã��򷵻�true��
 * \sa enableAutoReplot()
 */
bool AbstractPlot::isAutoReplot() const {
    return autoReplot;
}

/*!
 * \brief �ػ�����ͼ�����ݡ�
 *
 * ����Զ��ػ棨auto replot��ѡ��û�����ã�����Ĭ����������������߾���ԭʼ���ݣ�raw data����
 * ������������ͼ��Ҫ��ʾˢ�£�����replot()����ʹ�ı�ɼ���
 *
 * \sa updateAxes(), setAutoReplot()
 */
void AbstractPlot::replot()
{
    // �����Ż��������ɼ�ʱ�����ػ档
    if(!isVisible()) return;

    bool doAutoReplot = isAutoReplot();
    enableAutoReplot( false );

    // updateAxes();

    // ������Ľű���ܷ����ı䣬��ʹ�ò�����Ҫ���¡�������Ҫ�ڻ���
    // ֮ǰ������Щ�����������ᣨscales���ͻ�����canvas����ͬ����

    QApplication::sendPostedEvents( this, QEvent::LayoutRequest );

    const bool ok = QMetaObject::invokeMethod(canvas, "replot", Qt::DirectConnection );

    if ( !ok ){
        // ��������canvas������û��replot�����ĺ󱸷�����
        canvas->update(canvas->contentsRect());
    }

    enableAutoReplot( doAutoReplot );
}

/*! \brief ���isAutoReplot()�� \c true ���ػ档*/
void AbstractPlot::autoRefresh()
{
    if ( isAutoReplot() )
        replot();
}

void AbstractPlot::disableAxis(int axisId)
{
    enableAxis(axisId, false);
}

/*!
 * \brief ���û����һ��������ؼ���
 *
 * ��һ��������ؼ�������ʱ��������ʾ��������������Ļ����ʾ�����ߣ���ǣ�markers�����Ժͱ����õ�������
 * �ؼ��������ͬʱ����Ļ�����ӳ��ת��������Ȼ��������������Ĭ������£�ֻ�� xBottom �� yLeft �Ǳ���
 * �õġ�
 *
 * ע�⣬����Ҫ���õĿؼ�������ʱ������Ĭ�ϵ����������Ȼ����������
 *
 * \param axisId ��������š�
 * \param tf \c true (����) or \c false (����)
 */
void AbstractPlot::enableAxis(int axisId, bool tf)
{
    if(!axisValid(axisId)){
        return;
    }

    AbstractAxis* axis = getAxisWidget(axisId);
    bool needUpdate = false;

    if(axis != nullptr){
        needUpdate = tf ^ axis->isAxisEnabled();
        axis->enableAxis(tf);
    }else{
        // ��Ҫ����������ؼ�����������ؼ������ڣ���ʹ��Ĭ�Ϸ�ʽ����һ����
        if(tf){
            needUpdate = true;
            createDefaultAxis(axisId);
            getAxisWidget(axisId)->enableAxis(tf);
        }
    }

    if(needUpdate)
        updateLayout();
}

/*!
 * \brief ������ؼ��Ƿ���ڡ�
 * \param axisId ���������͡�
 * \return ������ڣ���Ȼ��һ�������ã�������true����������ڣ�����false��
 */
bool AbstractPlot::isAxisExist(int axisId) const {
    return (getAxisWidget(axisId) != nullptr);
}

bool AbstractPlot::isAxisEnabled(int axisId) const
{
    const AbstractAxis* axis = getAxisWidget(axisId);
    return axis ? axis->isAxisEnabled() : false;
}

bool AbstractPlot::axisValid(int axisId)
{
    return ( ( axisId >= AbstractPlot::yLeft ) && ( axisId < AbstractPlot::axisCnt ) );
}

void AbstractPlot::setAxesPalette(const QPalette &pal)
{
    setAxisPalette(yLeft, pal);
    setAxisPalette(yRight, pal);
    setAxisPalette(xTop, pal);
    setAxisPalette(xBottom, pal);
}

void AbstractPlot::setAxisPalette(int axisId, const QPalette &pal)
{
    AbstractAxis* axis = getAxisWidget(axisId);
    if(axis != nullptr)
        axis->setPalette(pal);
}

AbstractPlot::Axis AbstractPlot::axisType2Id(AbstractAxis::AxisType type)
{
    switch(type){
    case AbstractAxis::LeftScale:   return yLeft;
    case AbstractAxis::RightScale:  return yRight;
    case AbstractAxis::TopScale:    return xTop;
    case AbstractAxis::BottomScale: return xBottom;
    default: return axisCnt;
    }
}

QWidget *AbstractPlot::getCanvas() {
    return canvas;
}

/*!
 * \brief ��������ͼ�ؼ��Ļ�����canvas����
 *
 * AbstractPlot�ѻ����ķ�����ΪԪ������meta methods�����ã���QMetaObject������ʹ�ô�ͳ��C++�麯������
 * ��ͬ�����ַ�������ʹ��canvas�̳���QWidget����QGLWidget�ĺ�����
 *
 * ����ʵ�����µĵ�Ԫ������meta methods����
 *
 * - replot()
 * ������û���ṩreplot()����ʱ��AbstractPlot�������update()��
 *
 * - borderPath()
 * ����Ҫ������������ʱ����Ҫ�߽�·����border path���ĸ��������û��������״ʱ������Բ�Ǿ��Σ���������
 * �������Ҳ�ǿ��Եġ�
 *
 * Ĭ�ϵĻ�����һ��SimplePlotCanvas��
 *
 * \param canvas �����ؼ���
 * \sa getCanvas()
 */
void AbstractPlot::setCanvas(QWidget *widget)
{
    if ( widget == canvas )
        return;

    delete canvas;
    canvas = widget;

    if ( canvas )
    {
        canvas->setParent( this );
        canvas->installEventFilter( this );

        if ( isVisible() )
            canvas->show();
    }
}

const QWidget *AbstractPlot::getCanvas() const {
    return canvas;
}
