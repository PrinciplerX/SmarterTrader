#ifndef DRAWERFRAME_H
#define DRAWERFRAME_H

#include "expandedwidgets_global.h"
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QMap>
#include <QStackedWidget>
#include <detailbutton.h>

/*!
 * \brief The DrawerFrame class
 *
 * +---+---+-------------------------------------+
 * | # | <-| Title                               |
 * +---+---+-------------------------------------+
 * | 1 |                                         |
 * +---+                                         |
 * | 2 |                                         |
 * +---+                                         |
 * | 3 |                                         |
 * +---+                                         |
 * |   |             Content Panel               |
 * |   |                                         |
 * |   |                                         |
 * +---+                                         |
 * | 4 |                                         |
 * +---+                                         |
 * | 5 |                                         |
 * +---+-----------------------------------------+
 */
class EXPANDEDWIDGETSSHARED_EXPORT DrawerFrame : public QWidget
{
    Q_OBJECT

public:
    DrawerFrame(QWidget *parent = 0);

    virtual void resizeEvent(QResizeEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    void addUpperButtonPage(DetailButton* button, QWidget* page, const QString &pageTitle);
    void addBottomButtonPage(DetailButton* button, QWidget* page, const QString &pageTitle);

    void addTitleWidget(QWidget* widget);

    /*!
     * \brief ����������ĵײ�����һ���ؼ�������ռ䲻��drawer�ϵİ�ť���ƣ�����ȫ����ʾ
     * \param contentWidget Ҫ�ӵĿؼ�
     */
    void addBottomContentWidget(QWidget* contentWidget);

    inline void setTitle(const QString& title){titleLabel->setText(title);}
    void setCurrentPage(int index);

signals:

public slots:
    void homeButtonClicked();
    void pageButtonClicked();

protected:
    void showDrawer();
    void hideDrawer();

    inline DetailButton* getHomeButton(){return homeButton;}
    inline QFrame* getTitleBar(){return titleWidget;}
    inline QFrame* getDrawerWidget(){return drawerWidget;}
    inline QLabel* getTitleLabel(){return titleLabel;}
    inline QStackedWidget* getStackedWidget(){return stackedWidget;}
    inline DetailButton* getBackButton(){return backButton;}

    virtual void setPageButtonStyleSheet(DetailButton* pageButton) const;

private:
    /*! ����˵��Ƿ�ɼ�*/
    bool drawerVisible;
    bool onAnimation;

    DetailButton * homeButton;
    DetailButton* currentToggledButton;
    DetailButton* backButton;

    QVector<QPushButton*> upperButtonVector;
    QVector<QPushButton*> bottomButtonVector;

    QVBoxLayout* drawerLayout;
    QVBoxLayout* upperButtonLayout;
    QVBoxLayout* bottomButtonLayout;
    QHBoxLayout* titleBarLayout;
    QVBoxLayout* contentPanelLayout;

    QFrame* titleWidget;
    QFrame* drawerWidget;

    QStackedWidget* stackedWidget;
    QLabel* titleLabel;//stackedWidget��ǰ��ʾWidget�ı���

    QMap<DetailButton*,QWidget*> pageMap;
};

#endif // DRAWERFRAME_H
