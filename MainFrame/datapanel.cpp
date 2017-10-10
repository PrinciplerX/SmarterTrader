#include "datapanel.h"
#include "newstestwidget.h"
#include "billboardwidget.h"
#include "codeeditor.h"
#include <formulahighlighter.h>

DataPanel::DataPanel(QWidget *parent) : QTabWidget(parent)
{
    tabBar()->setObjectName(QString("PanelTabBar"));

    this->addTab(new TradeDataPanel(),tr("��������"));
    this->addTab(new QWidget(),tr("�ο�����"));
    this->addTab(new QWidget(),tr("��������"));
    this->addTab(new QWidget(),tr("��������"));
    this->addTab(new QWidget(),tr("��۾���"));
    this->addTab(new NewsTestWidget(),tr("�����¼�"));
    this->addTab(new BillboardWidget(),tr("������"));

    CodeEditor* editor = new CodeEditor();
    FormulaHighlighter* highlighter = new FormulaHighlighter(editor->document());

    this->addTab(editor, tr("��ʽ�༭"));
}
