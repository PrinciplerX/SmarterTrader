#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include "expandedwidgets_global.h"
#include <QFrame>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCompleter>

/*!
 * \brief 搜索框
 *
 * 提供了默认的界面效果，但是可以自定义。
 */
class EXPANDEDWIDGETSSHARED_EXPORT SearchBox : public QFrame
{
    Q_OBJECT

public:
    SearchBox(QFrame *parent=0);
    virtual void resizeEvent(QResizeEvent* event) override;
    inline void setCompleter(QCompleter* completer){searchLineEdit->setCompleter(completer);}
    void setSearchHint(const QString& hint);
    void setSearchButtonIcon(const QIcon& icon);
    void setClearButtonIcon(const QIcon& icon);

signals:
    void search(const QString& keyword);

protected slots:
    void onClearButtonClicked(){searchLineEdit->clear();clearButton->hide();emptyInput=true;}
    void onTextChanged(const QString& text);
    void onTextEdited(){clearFocus();}
    void onTextEditingFinished();

protected:
    inline QPushButton* getSearchButton(){return searchButton;}
    inline QLineEdit* getLineEdit(){return searchLineEdit;}
    inline QPushButton* getClearButton(){return clearButton;}

private:
    QLineEdit* searchLineEdit;
    QPushButton* searchButton;
    QPushButton* clearButton;
    bool emptyInput;
};

inline void SearchBox::setSearchHint(const QString &hint) { searchLineEdit->setPlaceholderText(hint); }

#endif // SEARCHBOX_H
