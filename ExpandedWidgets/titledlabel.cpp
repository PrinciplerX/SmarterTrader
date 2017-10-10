#include "titledlabel.h"

TitledLabel::TitledLabel(const QString& title, const QString& content, const QString &splitter, QWidget* parent)
    :QLabel(parent)
    ,title(title)
    ,content(content)
    ,splitter(splitter)
{
    this->setText(title+splitter+content);
    QFont f = this->font();
    f.setPointSize(10);
    this->setFont(f);
}

TitledLabel::~TitledLabel()
{

}

void TitledLabel::setText(const QString& text){
    QLabel::setText(text);
}

void TitledLabel::setTitle(const QString& title){
    this->title=title;
    this->setText(title+splitter+content);
}

QString TitledLabel::getTitle(){
    return title;
    this->setText(title+splitter+content);
}

void TitledLabel::setContent(const QString& content){
    this->content=content;
    this->setText(title+splitter+content);
}

QString TitledLabel::getContent(){
    return content;
}
