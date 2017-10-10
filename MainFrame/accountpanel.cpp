#include "accountpanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QHeaderView>
#include <QSplitter>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QTextCodec>
#include <QDebug>
#include "stringhelper.h"
#include "ctpaccountmanager.h"
#include <QTextEdit>
#include <QMessageBox>

AccountPanel::AccountPanel(QWidget *parent) : QTabWidget(parent),
    mOpList(new QTreeWidget()),
    mStackedWidget(new QStackedWidget())
{
    tabBar()->setObjectName(QString("PanelTabBar"));

    QAction *loginAct = new QAction(QIcon::fromTheme("go-home"), tr("&Login"), this);
    QAction *logoutAct = new QAction(QIcon::fromTheme("system-shutdown"), tr("&Logout"), this);
    QAction *refreshAct = new QAction(QIcon::fromTheme("view-refresh"), tr("&Refresh"), this);
    QAction *settingsAct = new QAction(QIcon::fromTheme("emblem-system"), tr("&Settings"), this);

    loginAct->setStatusTip(tr("Login CTP Account"));
    logoutAct->setStatusTip(tr("Logout"));
    refreshAct->setStatusTip(tr("Refresh data"));
    settingsAct->setStatusTip(tr("Settings"));

    setupOpList();

    QSplitter* mainWidget = new QSplitter(Qt::Horizontal);
    mainWidget->addWidget(mOpList);
    mainWidget->addWidget(mStackedWidget);

    addTab(mainWidget, "股票");
    addTab(new FuturesAccountPanel(), "期货");
}

void AccountPanel::setupOpList()
{
    mOpList->header()->hide();
    QTreeWidgetItem* sendOrderItem = new QTreeWidgetItem(mOpList);
    sendOrderItem->setText(0, "Entrust");
    QTreeWidgetItem* queryItem = new QTreeWidgetItem(mOpList);
    queryItem->setText(0, "Query");
    QTreeWidgetItem* queryEntrust = new QTreeWidgetItem(queryItem);
    QTreeWidgetItem* queryBargain = new QTreeWidgetItem(queryItem);
    queryEntrust->setText(0, "Query Entrust");
    queryBargain->setText(0, "Query Bargain");
}

CtpLoginPanel::CtpLoginPanel(QWidget *parent):
    QWidget(parent),
    mBrokerIdLineEdit(new QLineEdit()),
    mUserCodeLineEdit(new QLineEdit()),
    mPasswordLineEdit(new QLineEdit()),
    mDynamicPwdLineEdit(new QLineEdit()),
    mServerComboBox(new QComboBox()),
    mStatusLabel(new QLabel()),
    mLoginButton(new QPushButton("登陆"))
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QFormLayout* formLayout = new QFormLayout();

    mBrokerIdLineEdit->setText("9999");
    mUserCodeLineEdit->setText("035286");
    mPasswordLineEdit->setText("111111");
    mPasswordLineEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow("选择服务器", mServerComboBox);
    formLayout->addRow("经纪公司ID", mBrokerIdLineEdit);
    formLayout->addRow("用户代码", mUserCodeLineEdit);
    formLayout->addRow("交易密码", mPasswordLineEdit);
    formLayout->addRow("动态密码", mDynamicPwdLineEdit);
    formLayout->addRow("登陆状态", mStatusLabel);

    mStatusLabel->setFrameShape(QFrame::StyledPanel);
    mStatusLabel->setFrameShadow(QFrame::Sunken);
    mStatusLabel->setText(CtpClient::instance().isConnected() ? "已连接" : "未连接");

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(mLoginButton);
    setLayout(mainLayout);

    connect(mLoginButton, SIGNAL(clicked()), this, SLOT(onLoginButtonPressed()));
    connect(&CtpClient::instance().notifier(), SIGNAL(OnFrontConnected()), this, SLOT(onFrontConnected()));
    connect(&CtpClient::instance().notifier(), SIGNAL(OnFrontDisconnected(int)), this, SLOT(onFrontDisconnected(int)));
    connect(&CtpClient::instance().notifier(), SIGNAL(OnRspUserLogin(int,QString,int,bool)), this, SLOT(onRspUserLogin(int,QString,int,bool)));
    connect(&CtpClient::instance().notifier(), SIGNAL(OnRspError(int,QString,int,bool)), this, SLOT(onRspError(int,QString,int,bool)));
}

void CtpLoginPanel::doLogin()
{
    std::string brokerId = mBrokerIdLineEdit->text().toStdString();
    std::string userCode = mUserCodeLineEdit->text().toStdString();
    std::string password = mPasswordLineEdit->text().toStdString();
    std::string oneTimePassword = mDynamicPwdLineEdit->text().toStdString();

    CThostFtdcReqUserLoginField loginField;
    memset(&loginField, 0, sizeof(CThostFtdcReqUserLoginField));

    strcpy(loginField.BrokerID, brokerId.c_str());
    strcpy(loginField.UserID, userCode.c_str());
    strcpy(loginField.Password, password.c_str());
    strcpy(loginField.OneTimePassword, oneTimePassword.c_str());

    mCurrentRequestId = CtpClient::instance().nextRequestId();
    mTraderApi->ReqUserLogin(&loginField, mCurrentRequestId);
    qDebug() << "[CtpLoginPanel] [doLogin] mCurrentRequestId =" << mCurrentRequestId;
}

void CtpLoginPanel::onRspError(int nErrorId, const QString &errorMsg, int nRequestID, bool bIsLast)
{
    if (mCurrentRequestId == nRequestID) {
        QMessageBox::warning(this, "Error", errorMsg);
        mLoginButton->setEnabled(true);
    }
}

void CtpLoginPanel::onLoginButtonPressed()
{
    std::string userCode = mUserCodeLineEdit->text().toStdString();
    mTraderApi = CtpClient::instance().getTraderApi(userCode);

    if (CtpClient::instance().isConnected()) {
        doLogin();
    } else {
        mWaitingConnect = true;
        mStatusLabel->setText("正在连接...");
    }

    mLoginButton->setEnabled(false);
}

void CtpLoginPanel::onFrontConnected()
{
    mStatusLabel->setText("已连接");
    if (mWaitingConnect) {
        mWaitingConnect = false;
        doLogin();
    }
}

void CtpLoginPanel::onFrontDisconnected(int nReason)
{
    mStatusLabel->setText(QString("连接中断（%1）").arg(QString::number(nReason)));
}

void CtpLoginPanel::onRspUserLogin(int nErrorId, const QString &errorMsg, int nRequestID, bool bIsLast)
{
    if (mCurrentRequestId != nRequestID) return;

    qDebug() << "[CtpLoginPanel] [onRspUserLogin] nRequestID =" << nRequestID << ", error_id =" << nErrorId << ", error_msg =" << errorMsg;
    mStatusLabel->setText(QString("error_id = %1, error_msg = %2").arg(QString::number(nErrorId), errorMsg));
    if (nErrorId == 0) {
        emit loginSuccess(mUserCodeLineEdit->text());
    } else {
        QMessageBox::warning(this, "Error", errorMsg);
    }
    mLoginButton->setEnabled(true);
}

FuturesAccountPanel::FuturesAccountPanel(QWidget *parent) :
    QWidget(parent),
    mStackedWidget(new QStackedWidget()),
    mLoginPanel(new CtpLoginPanel()),
    mDetailPanel(new CtpAccountDetailPanel())
{
    mDetailPanelIndex = mStackedWidget->addWidget(mDetailPanel);
    mLoginPanelIndex = mStackedWidget->addWidget(mLoginPanel);
    mStackedWidget->setCurrentIndex(mLoginPanelIndex);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(mStackedWidget);
    setLayout(mainLayout);

    connect(mLoginPanel, SIGNAL(loginSuccess(QString)), this, SLOT(onLoginSuccess(QString)));
    connect(mLoginPanel, SIGNAL(loginSuccess(QString)), mDetailPanel, SLOT(onLoginSuccess(QString)));
}

void FuturesAccountPanel::onLoginSuccess(const QString &userCode)
{
    mStackedWidget->setCurrentIndex(mDetailPanelIndex);
}

CtpAccountDetailPanel::CtpAccountDetailPanel(QWidget *parent):
    QWidget(parent)
{
    connect(&CtpClient::instance().notifier(), SIGNAL(OnRspSettlementInfoConfirm(int,QString,int,bool)), this, SLOT(onRspSettlementInfoConfirm(int,QString,int,bool)));
    connect(&CtpClient::instance().notifier(), SIGNAL(OnRspQrySettlementInfo(int,QString,QString,int,bool)), this, SLOT(onRspQrySettlementInfo(int,QString,QString,int,bool)));
    connect(&CtpClient::instance().notifier(), SIGNAL(OnRspQrySettlementInfoConfirm(int,QString,int,bool)), this, SLOT(onRspQrySettlementInfoConfirm(int,QString,int,bool)));
    connect(&CtpClient::instance().notifier(), SIGNAL(OnRspError(int,QString,int,bool)), this, SLOT(onRspError(int,QString,int,bool)));
}

void CtpAccountDetailPanel::onRspError(int nErrorId, const QString &errorMsg, int nRequestID, bool bIsLast)
{
    if (nRequestID == mCurrentRequestId)
        QMessageBox::warning(this, "Error", errorMsg);
}

void CtpAccountDetailPanel::onLoginSuccess(const QString &userCode)
{
    mUserCode = userCode;

    mUserApi = CtpClient::instance().getTraderApi(userCode.toStdString());
    CThostFtdcQrySettlementInfoField field;
    CtpAccountManager::instance().initField(field, userCode.toStdString());

    mCurrentRequestId = CtpClient::instance().nextRequestId();

    std::string errMsg;
    bool result = CtpClient::checkRequestResult(mUserApi->ReqQrySettlementInfo(&field, mCurrentRequestId), errMsg);
    if (!result) {
        QMessageBox::warning(this, "Error", QString::fromStdString(errMsg));
    } else {
        qDebug() << "ErrorMsg: " << QString::fromStdString(errMsg);
    }

    qDebug() << "[CtpAccountDetailPanel] [onLoginSuccess] mCurrentRequestId =" << mCurrentRequestId;
}

void CtpAccountDetailPanel::onRspSettlementInfoConfirm(int nErrorId, const QString &errorMsg, int nRequestID, bool bIsLast)
{
}

void CtpAccountDetailPanel::onRspQrySettlementInfo(int nErrorId, const QString &errorMsg, const QString &content, int nRequestID, bool bIsLast)
{
    if (nErrorId == 0) {
        mSettlementInfo.append(content);
    } else {
        QMessageBox::warning(this, "Error", errorMsg);
    }

    if (bIsLast) {
        QTextEdit* edit = new QTextEdit();
        QFont font;
        font.setFamily("Ubuntu Mono");
        font.setPointSize(9);
        edit->setFont(font);

        edit->setText(mSettlementInfo);
        edit->show();
    }
}

void CtpAccountDetailPanel::onRspQrySettlementInfoConfirm(int nErrorId, const QString &errorMsg, int nRequestID, bool bIsLast)
{

}
