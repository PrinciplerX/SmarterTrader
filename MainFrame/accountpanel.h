#ifndef ACCOUNTPANEL_H
#define ACCOUNTPANEL_H

#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStackedWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include "ctpclient.h"

class AccountPanel : public QTabWidget
{
    Q_OBJECT
public:
    explicit AccountPanel(QWidget *parent = 0);

signals:

public slots:

protected:
    void setupOpList();

private:
    QTreeWidget* mOpList;
    QStackedWidget* mStackedWidget;
};

class CtpLoginPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CtpLoginPanel(QWidget *parent = 0);

signals:
    void loginSuccess(const QString& userCode);

protected:
    void doLogin();

private slots:
    void onRspError(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);
    void onLoginButtonPressed();
    void onFrontConnected();
    void onFrontDisconnected(int nReason);
    void onRspUserLogin(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

private:
    QLineEdit* mBrokerIdLineEdit;
    QLineEdit* mUserCodeLineEdit;
    QLineEdit* mPasswordLineEdit;
    QLineEdit* mDynamicPwdLineEdit;
    QComboBox* mServerComboBox;
    QLabel* mStatusLabel;

    QPushButton* mLoginButton;

    bool mWaitingConnect = false;
    CThostFtdcTraderApi* mTraderApi;
    int mCurrentRequestId;
};

class CtpAccountDetailPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CtpAccountDetailPanel(QWidget *parent = 0);

protected slots:
    void onRspError(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);
    void onLoginSuccess(const QString& userCode);

    void onRspSettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);
    void onRspQrySettlementInfo(int nErrorId, const QString& errorMsg, const QString& content, int nRequestID, bool bIsLast);
    void onRspQrySettlementInfoConfirm(int nErrorId, const QString& errorMsg, int nRequestID, bool bIsLast);

private:
    CThostFtdcTraderApi* mUserApi;
    QString mUserCode;
    int mCurrentRequestId;

    QString mSettlementInfo;
};

class FuturesAccountPanel : public QWidget
{
    Q_OBJECT
public:
    explicit FuturesAccountPanel(QWidget *parent = 0);

protected slots:
    void onLoginSuccess(const QString& userCode);

private:
    QStackedWidget* mStackedWidget;
    CtpLoginPanel* mLoginPanel;
    CtpAccountDetailPanel* mDetailPanel;

    int mLoginPanelIndex;
    int mDetailPanelIndex;

    int mCurrentRequestId;
};

#endif // ACCOUNTPANEL_H
