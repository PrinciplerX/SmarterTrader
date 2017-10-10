#ifndef DOCKWIDGETTITLE_H
#define DOCKWIDGETTITLE_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

/*!
 * \brief DockWidget 的标题控件，包含标题标签以及一些控制按钮和选项。
 */
class DockWidgetTitle : public QFrame
{
    Q_OBJECT
public:
    explicit DockWidgetTitle(QWidget *parent = 0);
    virtual ~DockWidgetTitle();

    QString text() const { return mTextLabel->text(); }
    void setText(const QString& text) { mTextLabel->setText(text); }

    void setFloating(bool state);
    void setAutoHideEnabled(bool state);

    QPoint menuPos() const;

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void menuButtonPressed();
    void autoHideButtonPressed();
    void closeButtonPressed();

public slots:
};

#endif // DOCKWIDGETTITLE_H
