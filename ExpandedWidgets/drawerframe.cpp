#include "drawerframe.h"
#include <QPropertyAnimation>
#include <QDebug>

DrawerFrame::DrawerFrame(QWidget *parent):
    QWidget(parent),
    drawerVisible(false),
    onAnimation(false),
    homeButton(new DetailButton(48,36,48,this)),
    backButton(new DetailButton(48,36,48,this)),
    drawerWidget(new QFrame(this)),
    titleLabel(new QLabel(QString("Title"))),
    titleWidget(new QFrame(this)),
    titleBarLayout(new QHBoxLayout()),
    stackedWidget(new QStackedWidget(this)),
    upperButtonLayout(new QVBoxLayout()),
    bottomButtonLayout(new QVBoxLayout()),
    drawerLayout(new QVBoxLayout()),
    contentPanelLayout(new QVBoxLayout()),
    currentToggledButton(nullptr)
{
    homeButton->setIcon(QIcon(QString(":/ic_menu_white_48dp.png")));
    homeButton->setFixedSize(48,48);
    homeButton->setIconSize(QSize(36,36));
    homeButton->setFocusPolicy(Qt::NoFocus);

    //Title Bar
    backButton->setIcon(QIcon::fromTheme("go-previous"));
    // backButton->setIcon(QIcon(QString(":/ic_keyboard_backspace_black_48dp.png")));
    backButton->setFixedSize(48,48);
    backButton->setIconSize(QSize(36,36));
    backButton->setFocusPolicy(Qt::NoFocus);

    titleBarLayout->addWidget(backButton);
    titleBarLayout->addWidget(titleLabel);
    titleBarLayout->addStretch();

    titleBarLayout->setMargin(0);
    titleWidget->setFrameShape(QFrame::StyledPanel);
    titleWidget->setLayout(titleBarLayout);
    titleWidget->setFixedHeight(48);
    titleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Content Panel
    stackedWidget->setFrameShape(QFrame::NoFrame);
    contentPanelLayout->addWidget(titleWidget);
    contentPanelLayout->addWidget(stackedWidget);

    //Drawer
    drawerLayout->setMargin(0);
    upperButtonLayout->setMargin(0);
    bottomButtonLayout->setMargin(0);

    drawerLayout->setSpacing(0);
    upperButtonLayout->setSpacing(0);
    bottomButtonLayout->setSpacing(0);

    QFrame* bottomButtonWidget=new QFrame();
    bottomButtonWidget->setLayout(bottomButtonLayout);
    bottomButtonWidget->setStyleSheet(QString("QFrame#BottomButtonWidget{border-top: 1px solid gray;}"));
    bottomButtonWidget->setObjectName(QString("BottomButtonWidget"));

    drawerLayout->addLayout(upperButtonLayout);
    drawerLayout->addStretch();
    drawerLayout->addWidget(bottomButtonWidget);

    drawerWidget->setLayout(drawerLayout);
    /*! 使宽度不随父窗口大小而改变*/
    drawerWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    drawerWidget->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout* menuLayout=new QVBoxLayout();
    menuLayout->setMargin(0);
    menuLayout->setSpacing(0);
    menuLayout->addWidget(homeButton);
    menuLayout->addWidget(drawerWidget);
    drawerWidget->raise();

    QHBoxLayout* mainLayout=new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(menuLayout);
    mainLayout->addLayout(contentPanelLayout);
    this->setLayout(mainLayout);

    stackedWidget->setFocusProxy(this);
    stackedWidget->setFocusPolicy(Qt::StrongFocus);

    connect(homeButton,SIGNAL(clicked()),this,SLOT(homeButtonClicked()));
}

void DrawerFrame::homeButtonClicked(){
    drawerVisible?hideDrawer():showDrawer();
}

void DrawerFrame::pageButtonClicked()
{
    hideDrawer();
    DetailButton* b=dynamic_cast<DetailButton*>(sender());
    QMap<DetailButton*,QWidget*>::Iterator i=pageMap.find(b);
    if(i!=pageMap.end()){
        stackedWidget->setCurrentWidget(i.value());
        setTitle(i.value()->windowTitle());
        if(currentToggledButton!=nullptr) currentToggledButton->setChecked(false);
        currentToggledButton=b;
        b->setChecked(true);
    }
}

void DrawerFrame::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    drawerWidget->setGeometry(QRect(drawerWidget->x(),drawerWidget->y(),homeButton->width(),drawerWidget->height()));
    drawerVisible=false;
}

void DrawerFrame::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(drawerVisible){
        hideDrawer();
    }
}

void DrawerFrame::addUpperButtonPage(DetailButton *button, QWidget *page, const QString& pageTitle)
{
    page->setWindowTitle(pageTitle);
    button->setCheckable(true);
    button->setFocusPolicy(Qt::NoFocus);
    pageMap.insert(button,page);
    upperButtonLayout->addWidget(button);
    stackedWidget->addWidget(page);
    setPageButtonStyleSheet(button);
    connect(button,SIGNAL(clicked()),this,SLOT(pageButtonClicked()));

    drawerWidget->update();
}

void DrawerFrame::addBottomButtonPage(DetailButton* button, QWidget* page, const QString &pageTitle)
{
    page->setWindowTitle(pageTitle);
    button->setFocusPolicy(Qt::NoFocus);
    button->setCheckable(true);
    pageMap.insert(button,page);
    bottomButtonLayout->addWidget(button);
    stackedWidget->addWidget(page);
    setPageButtonStyleSheet(button);
    connect(button,SIGNAL(clicked()),this,SLOT(pageButtonClicked()));

    drawerWidget->update();
}

void DrawerFrame::addTitleWidget(QWidget *widget)
{
    titleBarLayout->addWidget(widget);
}

void DrawerFrame::addBottomContentWidget(QWidget *contentWidget){
    contentPanelLayout->addWidget(contentWidget);
    //让drawerWidget时刻保持在最顶层
    drawerWidget->raise();
}

void DrawerFrame::setCurrentPage(int index)
{
    QWidget* w = stackedWidget->widget(index);
    for(QMap<DetailButton*,QWidget*>::iterator i = pageMap.begin(); i != pageMap.end(); i++){
        if(i.value() == w)
            i.key()->click();
    }
    /*******
    if(index < upperButtonVector.size()){
        upperButtonVector[index]->click();
    }else{
        bottomButtonVector[index - upperButtonVector.size()]->click();
    }
    /*****
    stackedWidget->setCurrentWidget(index);
    setTitle(stackedWidget->currentWidget()->windowTitle());

    if(currentToggledButton!=nullptr) currentToggledButton->setChecked(false);
    currentToggledButton=b;
    b->setChecked(true);
    *******/
}

void DrawerFrame::showDrawer(){
    if(drawerVisible) return;

    QPropertyAnimation *animation = new QPropertyAnimation(drawerWidget, "geometry");
    animation->setDuration(200);
    animation->setStartValue(drawerWidget->geometry());
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setEndValue(QRect(drawerWidget->x(),drawerWidget->y(),drawerWidget->width()+210,drawerWidget->height()));
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    animation->start();

    drawerVisible=true;
}

void DrawerFrame::hideDrawer(){
    if(!drawerVisible) return;

    QPropertyAnimation *animation = new QPropertyAnimation(drawerWidget, "geometry");
    animation->setDuration(200);
    animation->setStartValue(drawerWidget->geometry());
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setEndValue(QRect(drawerWidget->x(),drawerWidget->y(),homeButton->width(),drawerWidget->height()));
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    animation->start();

    drawerVisible=false;
}

void DrawerFrame::setPageButtonStyleSheet(DetailButton *pageButton) const { Q_UNUSED(pageButton) }

