#include "datapanel.h"
#include "newstestwidget.h"
#include "billboardwidget.h"
#include "codeeditor.h"
#include <formulahighlighter.h>

DataPanel::DataPanel(QWidget *parent) : QTabWidget(parent)
{
    tabBar()->setObjectName(QString("PanelTabBar"));

    this->addTab(new TradeDataPanel(),tr("交易数据"));
    this->addTab(new QWidget(),tr("参考数据"));
    this->addTab(new QWidget(),tr("分类数据"));
    this->addTab(new QWidget(),tr("基本数据"));
    this->addTab(new QWidget(),tr("宏观经济"));
    this->addTab(new NewsTestWidget(),tr("新闻事件"));
    this->addTab(new BillboardWidget(),tr("龙虎榜"));

    CodeEditor* editor = new CodeEditor();
    FormulaHighlighter* highlighter = new FormulaHighlighter(editor->document());

    this->addTab(editor, tr("公式编辑"));
}
