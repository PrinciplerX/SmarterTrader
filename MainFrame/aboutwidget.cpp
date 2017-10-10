#include "aboutwidget.h"
#include "mainframe_global.h"
#include <QPicture>

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    logoLabel(new QLabel()),
    appLabel(new QLabel(QString("SuperTrader 0.4.0"))),
    textEdit(new QTextEdit())
{
    QPixmap pixmap(":/AppIcon.ico");
    pixmap = pixmap.scaled(48, 48);

    logoLabel->setPixmap(pixmap);
    QFont font= appLabel->font();
    font.setPointSize(24);
    font.setBold(true);
    appLabel->setFont(font);

    initTextEdit(textEdit);

    logoLabel->setAlignment(Qt::AlignCenter);
    appLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout* logoLayout = new QHBoxLayout();
    logoLayout->addWidget(logoLabel);
    logoLayout->addWidget(appLabel);
    logoLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(logoLayout);
    mainLayout->addWidget(textEdit);
}

void AboutWidget::initTextEdit(QTextEdit *edit)
{
    edit->setReadOnly(true);
    edit->setText(QString(">> 相比上一版本，该版实现的新功能：\n\n"
                          "1. 将原有的坐标绘制模块进行MVC模式改造，将View和Model分离，提供类似Qt Model/View那种"
                          "支持数据实时动态更新的功能和接口。\n\n"
                          "2. 实现股票价格走势图的实时更新和切换（同一个坐标图控件可以切换显示多个股票走势图）。将"
                          "股票数据和坐标图控件彻底分离。实现股票数据Model和坐标图View的动态结合。\n\n"
                          "3. 实现新的指标计算方式，为以后实现公式脚本导入指标打下基础。\n\n"
                          "4. 完善股票数据接口，不再依赖本地数据文件（如第0.2.0版本需要的ticks.csv和table.csv）,"
                          "所需数据直接联网查询。\n\n"
                          "5. 实现股票历史（K线）数据，当日分笔（tick）数据，历史分笔数据，五档数据的获取和解析功能。\n\n"
                          "6. 搜索框不再是摆设。在搜索框内输入代码搜索股票。\n\n"
                          "7. 实现收看新浪财经新闻的功能。\n\n"
                          "8. 实现查看龙虎榜的功能。\n\n"
                          "9. 增加了可选的界面风格。\n\n"
                          ">> 至今借鉴和参考过的开源项目：Qwt，Qt Charts，Tushare。\n"
                          "市盈(动)：  现价/折算成全年的每股收益"
                          "量比：      开市后每分钟的平均成交量与过去5个交易日每分钟平均成交量之比"
                          "流通市值：  流通股本*现价"
                          "细分行业"
                          "地区"
                          "连涨天数"
                          "3日涨幅"


                          "总股本：    公司的总股本"
                          "利润同比：  (当期的利润-去年同期的利润) /去年同期的利润*100"
                          "收入同比：  (当期的收入-去年同期的收入) /去年同期的收入*100"
                          "市净率：    即是PB，现价/每股净资产"
                          "市现率：    现价/每股现金流量"
                          "市销率：    现价/每股营业收入"
                          "每股收益：  当期的净利润/总股本"
                          "每股净资：  净资产/总股本"
                          "每股公积：  公积金/总股本"
                          "每股未分配：未分配利润/总股本"
                          "权益比：    净资产/总资产*100"
                          "净益率：    即净资产收益率,当期的净利润/净资产*100"
                          "销售毛利率：(营业收入-营业成本) /营业收入*100"
                          "营业利润率：营业利润/营业收入*100"
                          "净利润率：  净利润/营业收入*100"

                          "市盈(TTM):  根据最近12个月的每股收益得到的市盈率"
                          "市盈(静):   静态市盈率(最近年报的市盈率)"
                          "贝塔系数:   个股相对于大盘的波动表现(最近60天统计),其绝对值越大,显示其收益变化幅度相对于大盘的变化幅度越大;如果是负值,则表示方面相反.0.5为低风险股票,1.0表示为平均风险股票,2.0高风险股票,大多数股票介于0.5到l.5间"
                          "财务更新"
                          "总资产"
                          "净资产"
                          "少数股权"
                          "资产负债率%"
                          "流动资产 固定资产 无形资产 流动负债 公积金 存货 应收账款 营业收入 营业成本 营业利润 投资收益 利润总额 税后利润 净利润 未分利润 经营现金流 总现金流 股东人数 人均市值"
                          ""));
}
