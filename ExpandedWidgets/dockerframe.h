#ifndef DOCKERFRAME_H
#define DOCKERFRAME_H

#include "expandedwidgets_global.h"
#include <QWidget>
#include <QTabBar>

class EXPANDEDWIDGETSSHARED_EXPORT DockerFrame : public QWidget
{
    Q_OBJECT
public:
    explicit DockerFrame(QWidget* centerWidget = nullptr, QWidget *parent = 0);
    virtual ~DockerFrame();

signals:

public slots:

protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    QTabBar* mTopTabBar;
    QTabBar* mBottomTabBar;
    QTabBar* mLeftTabBar;
    QTabBar* mRightTabBar;

    QWidget* mCenterWidget;
};

#endif // DOCKERFRAME_H
