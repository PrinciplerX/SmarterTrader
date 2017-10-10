#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include "dockerframe_global.h"

#include <QWidget>
#include <QDockWidget>

class QPushButton;

class DockWidgetTabButton;
class DockerFrame;
class DockWidgetTabBar;
class DockWidgetTitle;
class QVBoxLayout;

enum class DockWidgetState
{
    Unknown = -1,
    Docked = 0,    //! DockWidget 停靠到了 DockerFrame 上
    Floating = 1,  //! DockWidget 处于悬浮状态
    Hidden = 2,    //! DockWidget 处于自动隐藏状态
    Closed = 3,    //! DockWidget 被用户关闭（通过点击关闭按钮）
};

/*!
 * \brief 自动隐藏的 QDockWidget。
 */
class DOCKERFRAMESHARED_EXPORT DockWidget : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(Qt::DockWidgetArea area READ area NOTIFY areaChanged)
    Q_PROPERTY(DockWidgetState state READ state WRITE setState NOTIFY stateChanged)


public:
    explicit DockWidget(const QString& title, QWidget *parent = 0);
    virtual ~DockWidget();

    QString windowTitle() const;
    void setWindowTitle(const QString& title);

    bool isMinimized() const { return (mState == DockWidgetState::Hidden); }
    bool isDocked() const { return (mState == DockWidgetState::Docked); }
    bool isFloating() const { return (mState == DockWidgetState::Floating); }

    /*! \brief 返回控件的当前显示区域。*/
    Qt::DockWidgetArea area() const { return mArea; }

    DockWidgetState state() const { return mState; }
    void setState(DockWidgetState state);

    void closeDockWidget();

    void setWidget(QWidget* widget);

    void setTabifiedDocks(const QList<QDockWidget*>& dockWidgetList);
    const std::vector<DockWidget*>& tabfiedDocks() const { return mTabifieds; }
    void clearTabifiedDocks() { mTabifieds.clear(); }

    QAction* menuAction() const { return mMenuAction; }
    void setMenuAction(QAction* action) { mMenuAction = action; }

private:
    void openTitleMenu();
    void autoHideStateToggle();
    void updateDockLocation(Qt::DockWidgetArea area);
    void updateTopLevelState(bool topLevel);
    void removeFromTabifiedDocks(DockWidget* dockWidget);

protected:
    virtual bool event(QEvent* event) override;

signals:
    void areaChanged(Qt::DockWidgetArea area);

    void pinned(DockWidget* dockWidget);
    void unpinned(DockWidget* dockWidget);
    void docked(DockWidget* dockWidget);
    void undocked(DockWidget* dockWidget);

private:
    Qt::DockWidgetArea mArea;
    DockWidgetTitle* mTitleWidget;
    DockWidgetState* mState;
    QVBoxLayout* mLayout;
    std::vector<DockWidget*> mTabifieds;
    QAction* mMenuAction;
};

#endif // DOCKWIDGET_H
