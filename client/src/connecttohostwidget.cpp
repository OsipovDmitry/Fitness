#include "connecttohostwidget.h"
#include "ui_connecttohostwidget.h"

#include "clientcontroller.h"

ConnectToHostWidget::ConnectToHostWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ConnectToHostWidget)
{
    m_ui->setupUi(this);
    failedConnection();
}

ConnectToHostWidget::~ConnectToHostWidget()
{
    delete m_ui;
}

void ConnectToHostWidget::tryToConnect()
{
    on_tryAgainButton_clicked();
}

void ConnectToHostWidget::on_tryAgainButton_clicked()
{
    m_ui->infoLabel->setText(QObject::tr("Trying to connect to server..."));
    m_ui->tryAgainButton->hide();

    if (ClientController::instance().connectToServer())
        emit sgnConnected();
    else
        failedConnection();
}

void ConnectToHostWidget::failedConnection()
{
    m_ui->infoLabel->setText(QObject::tr("Failed connection to server"));
    m_ui->tryAgainButton->show();
}
