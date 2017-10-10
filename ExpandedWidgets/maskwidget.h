#ifndef MASKWIDGET_H
#define MASKWIDGET_H

#include "expandedwidgets_global.h"
#include <QWidget>

/*!
 * \brief 显示时，遮挡窗口中的所有内容。常用在加载数据时显示进度条，加载动画或者在此
 * 控件内添加其他内容显示加载选项。
 *
 * 调用show()启用遮挡，调用hide()隐藏。
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
