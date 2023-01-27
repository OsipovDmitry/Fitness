#include "clientmainwindow.h"
#include "connecttohostwidget.h"
#include "loginwidget.h"
#include "forgotpasswordwidget.h"
#include "createnewaccountwidget.h"
#include "accountwidget.h"
#include "ui_clientmainwindow.h"

ClientMainWindow::ClientMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::ClientMainWindow)
{
    m_ui->setupUi(this);

    auto connectToHostWidget = new ConnectToHostWidget(this);
    connect(connectToHostWidget, &ConnectToHostWidget::sgnConnected, this, &ClientMainWindow::connectedToServer);
    setCentralWidget(connectToHostWidget);

    connectToHostWidget->tryToConnect();
}

ClientMainWindow::~ClientMainWindow()
{
    delete m_ui;
}

void ClientMainWindow::connectedToServer()
{
    auto loginWidget = new LoginWidget(this);
    connect(loginWidget, &LoginWidget::sgnLogin, this, &ClientMainWindow::login);
    connect(loginWidget, &LoginWidget::sgnForgotPassword, this, &ClientMainWindow::forgotPassword);
    connect(loginWidget, &LoginWidget::sgnCreateNewAccount, this, &ClientMainWindow::createNewAccount);
    setCentralWidget(loginWidget);
}

void ClientMainWindow::login(quint64 userId)
{
    auto accountWidget = new AccountWidget(this, userId);
    setCentralWidget(accountWidget);
}

void ClientMainWindow::forgotPassword()
{
    auto forgotPasswordWidget = new ForgotPasswordWidget(this);
    connect(forgotPasswordWidget, &ForgotPasswordWidget::sgnPassswordChanged, this, &ClientMainWindow::passwordChanged);
    connect(forgotPasswordWidget, &ForgotPasswordWidget::sgnBack, this, &ClientMainWindow::backToLogin);
    setCentralWidget(forgotPasswordWidget);
}

void ClientMainWindow::createNewAccount()
{
    auto createNewAccountWidget = new CreateNewAccountWidget(this);
    connect(createNewAccountWidget, &CreateNewAccountWidget::sgnAccountCreated, this, &ClientMainWindow::accountCreated);
    connect(createNewAccountWidget, &CreateNewAccountWidget::sgnBack, this, &ClientMainWindow::backToLogin);
    setCentralWidget(createNewAccountWidget);
}

void ClientMainWindow::passwordChanged(quint64 phonenumber, QString password)
{
    auto loginWidget = new LoginWidget(this, phonenumber, password);
    connect(loginWidget, &LoginWidget::sgnLogin, this, &ClientMainWindow::login);
    connect(loginWidget, &LoginWidget::sgnForgotPassword, this, &ClientMainWindow::forgotPassword);
    connect(loginWidget, &LoginWidget::sgnCreateNewAccount, this, &ClientMainWindow::createNewAccount);
    setCentralWidget(loginWidget);
}

void ClientMainWindow::backToLogin()
{
    auto loginWidget = new LoginWidget(this);
    connect(loginWidget, &LoginWidget::sgnLogin, this, &ClientMainWindow::login);
    connect(loginWidget, &LoginWidget::sgnForgotPassword, this, &ClientMainWindow::forgotPassword);
    connect(loginWidget, &LoginWidget::sgnCreateNewAccount, this, &ClientMainWindow::createNewAccount);
    setCentralWidget(loginWidget);
}

void ClientMainWindow::accountCreated(quint64 phonenumber, QString, QString, QString password)
{
    auto loginWidget = new LoginWidget(this, phonenumber, password);
    connect(loginWidget, &LoginWidget::sgnLogin, this, &ClientMainWindow::login);
    connect(loginWidget, &LoginWidget::sgnForgotPassword, this, &ClientMainWindow::forgotPassword);
    connect(loginWidget, &LoginWidget::sgnCreateNewAccount, this, &ClientMainWindow::createNewAccount);
    setCentralWidget(loginWidget);
}
