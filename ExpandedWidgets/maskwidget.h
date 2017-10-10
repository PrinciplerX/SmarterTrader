#ifndef MASKWIDGET_H
#define MASKWIDGET_H

#include "expandedwidgets_global.h"
#include <QWidget>

/*!
 * \brief ��ʾʱ���ڵ������е��������ݡ������ڼ�������ʱ��ʾ�����������ض��������ڴ�
 * �ؼ����������������ʾ����ѡ�
 *
 * ����show()�����ڵ�������hide()���ء�
 */
class EXPANDEDWIDGETSSHARED_EXPORT MaskWidget : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief MaskWidget
     * \param parent
     * \param _folowToHeadWidget
     */
    explicit MaskWidget(QWidget* parent, bool _folowToHeadWidget = false);

protected:
    bool eventFilter(QObject* _object, QEvent* _event);

    void paintEvent(QPaintEvent* _event);

    void showEvent(QShowEvent* _event);

private:
    void updateSelf();

    bool m_isInUpdateSelf;

    QPixmap grabParentWidgetPixmap() const;

    QPixmap m_parentWidgetPixmap;
};

#endif // MASKWIDGET_H
