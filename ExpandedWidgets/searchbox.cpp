#include "searchbox.h"
#include <QResizeEvent>
#include <QCompleter>

SearchBox::SearchBox(QFrame *parent):
    QFrame(parent),
    searchButton(new QPushButton()),
    clearButton(new QPushButton()),
    searchLineEdit(new QLineEdit()),
    emptyInput(true)
{
    // searchButton->setIcon(QIcon((":/ic_search_white_48dp.png")));
    searchButton->setIcon(QIcon::fromTheme("edit-find"));
    searchButton->setFocusPolicy(Qt::NoFocus);
    searchButton->setStyleSheet(QString("QPushButton{background-color:#FF3030;border:0px;padding:0px;}")+
                                QString("QPushButton:hover{background-color:#FF5050;border:0px;padding:0px;}")+
                                QString("QPushButton:pressed{background-color:red;border:0px;padding:0px;}"));

    clearButton->setFocusPolicy(Qt::NoFocus);
    clearButton->setStyleSheet(QString("QPushButton{border:0px;padding:0px;}")+
                               QString("QPushButton:hover{border:0px;padding:0px;}")+
                               QString("QPushButton:pressed{border:0px;padding:0px;}"));

    searchLineEdit->setStyleSheet(QString("min-height:28px;font: 18px;"));

    searchLineEdit->setFrame(false);
    // clearButton->setIcon(QIcon(":/ic_close_black_48dp.png"));
    clearButton->setIcon(QIcon::fromTheme("edit-clear"));
    clearButton->setCursor(Qt::ArrowCursor);
    clearButton->setIconSize(QSize(24,24));
    searchButton->setIconSize(QSize(24,24));
    searchButton->setFixedWidth(32);

    searchButton->setToolTip(tr("search"));
    searchLineEdit->setContentsMargins(0,0,0,0);
    searchLineEdit->setPlaceholderText(tr("搜索名称，代码，新闻等"));

    QHBoxLayout* searchLineEditLayout=new QHBoxLayout();
    searchLineEditLayout->setMargin(0);
    searchLineEditLayout->setSpacing(0);
    searchLineEditLayout->addStretch();
    searchLineEditLayout->addWidget(clearButton);
    searchLineEdit->setLayout(searchLineEditLayout);

    QHBoxLayout* mainLayout=new QHBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->addWidget(searchLineEdit);
    mainLayout->addWidget(searchButton);
    setLayout(mainLayout);

    connect(searchLineEdit,SIGNAL(textChanged(QString)),this,SLOT(onTextChanged(QString)));
    connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(onTextEditingFinished()));
    connect(clearButton,SIGNAL(clicked()),this,SLOT(onClearButtonClicked()));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(onTextEditingFinished()));
    connect(searchLineEdit,SIGNAL(textEdited(QString)),this,SLOT(onTextEdited()));

    clearButton->hide();
}

void SearchBox::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    clearButton->setFixedSize(searchLineEdit->height(),searchLineEdit->height());
    searchButton->setFixedHeight(searchLineEdit->height());
    searchLineEdit->setTextMargins(0,0,clearButton->width(),0);
}

void SearchBox::setSearchButtonIcon(const QIcon &icon)
{
    searchButton->setIcon(icon);
}

void SearchBox::setClearButtonIcon(const QIcon &icon)
{
    clearButton->setIcon(icon);
}

void SearchBox::onTextChanged(const QString &text)
{
    if(text.isEmpty()){
        emptyInput=true;
        clearButton->hide();
    }else{
        emptyInput=false;
        clearButton->show();
    }
}

void SearchBox::onTextEditingFinished(){
    QString keyword = searchLineEdit->text();
    if(!keyword.isEmpty())
        emit search(keyword);
}

