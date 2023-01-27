#include <QTimer>

#include <phonenumber.h>

#include "clientcontroller.h"
#include "createnewaccountwidget.h"
#include "ui_createnewaccountwidget.h"

CreateNewAccountWidget::CreateNewAccountWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::CreateNewAccountWidget)
{
    m_ui->setupUi(this);

    m_ui->enterCodeGroup->hide();
    m_ui->infoGroup->hide();
    m_ui->errorLabel->hide();
}

CreateNewAccountWidget::~CreateNewAccountWidget()
{
    delete m_ui;
}

void CreateNewAccountWidget::on_backButton_clicked()
{
    emit sgnBack();
}

void CreateNewAccountWidget::on_sendMeCodeButton_clicked()
{
    m_ui->errorLabel->hide();

    bool isOk = true;
    auto phonenumberString = m_ui->phonenumberEdit->text();
    m_phonenumber = static_cast<quint64>(region()) * static_cast<quint64>(1000000000);
    m_phonenumber += phonenumberString.toULongLong(&isOk);

    if (phonenumberString.length() != 9 || !isOk)
    {
        failedPhonenumber();
        return;
    }

    auto& controller = ClientController::instance();
    controller.checkPhonenumber(m_phonenumber, [this](const AbstractMessage& message) {
        auto& checkPhonenumberAnswerMessage = dynamic_cast<const CheckPhonenumberAnswerMessage&>(message);
        if (checkPhonenumberAnswerMessage.result == CheckPhonenumberAnswerMessage::ResultType::PhonenumberNotRegistered)
        {
            m_code = checkPhonenumberAnswerMessage.code;
            m_ui->phonenumberEdit->setDisabled(true);
            m_ui->sendMeCodeButton->hide();
            m_ui->enterCodeGroup->show();
            runSendCodeAgainTimer();
        }
        else
            phonenumberRegistered();
    });
}

void CreateNewAccountWidget::on_codeOkButton_clicked()
{
    m_ui->errorLabel->hide();

    bool isOk = true;
    auto code = m_ui->codeEdit->text().toUInt(&isOk);

    if (m_code != code || !isOk)
    {
        failedCode();
        return;
    }

    m_ui->enterCodeGroup->hide();
    m_ui->infoGroup->show();
}

void CreateNewAccountWidget::on_sendCodeAgainButton_clicked()
{
    on_sendMeCodeButton_clicked();
}

void CreateNewAccountWidget::on_createAccountButton_clicked()
{
    if (m_ui->firstnameEdit->text().isEmpty())
    {
        emptyFirstName();
        return;
    }

    if (m_ui->lastnameEdit->text().isEmpty())
    {
        emptyLastName();
        return;
    }

    if (m_ui->passwordEdit->text().isEmpty())
    {
        emptyPassword();
        return;
    }

    if (m_ui->passwordEdit->text() != m_ui->repeatedPasswordEdit->text())
    {
        unequalPasswords();
        return;
    }

    auto& controller = ClientController::instance();
    controller.createNewAccount(m_phonenumber, m_ui->firstnameEdit->text(), m_ui->lastnameEdit->text(), m_ui->passwordEdit->text(), [this](const AbstractMessage& message) {
        auto& createNewAccountAnswerMessage = dynamic_cast<const CreateNewAccountAnswerMessage&>(message);
        if (createNewAccountAnswerMessage.result == CreateNewAccountAnswerMessage::ResultType::AccountCreated)
        {
            emit sgnAccountCreated(string2Phonenumber(m_ui->phonenumberEdit->text()), m_ui->firstnameEdit->text(), m_ui->lastnameEdit->text(), m_ui->passwordEdit->text());
        }
        else
            failedCreatingAccount(createNewAccountAnswerMessage.result);
    });
}

void CreateNewAccountWidget::failedPhonenumber()
{
    QString errorString = QObject::tr("Pnonenumber is incorrect...");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void CreateNewAccountWidget::phonenumberRegistered()
{
    QString errorString = QObject::tr("Pnonenumber is already registered...");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void CreateNewAccountWidget::failedCode()
{
    QString errorString = QObject::tr("Entered code is wrong");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void CreateNewAccountWidget::unequalPasswords()
{
    QString errorString = QObject::tr("Entered passwords are not equal");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void CreateNewAccountWidget::emptyFirstName()
{
    QString errorString = QObject::tr("First name can't be an empty line");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void CreateNewAccountWidget::emptyLastName()
{
    QString errorString = QObject::tr("Last name can't be an empty line");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void CreateNewAccountWidget::emptyPassword()
{
    QString errorString = QObject::tr("Password can't be an empty line");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void CreateNewAccountWidget::failedCreatingAccount(CreateNewAccountAnswerMessage::ResultType)
{
    QString errorString = QObject::tr("Account hasn't been created...");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void CreateNewAccountWidget::runSendCodeAgainTimer()
{
    m_ui->sendCodeAgainButton->setEnabled(false);

    m_sendCodeAgainTimerValue = 5;
    m_ui->codeTimerLabel->show();
    m_ui->codeTimerLabel->setText(QObject::tr("Send code again in ") + QString::number(m_sendCodeAgainTimerValue) + QObject::tr("s"));

    auto sendCodeAgainTimer = new QTimer(this);
    sendCodeAgainTimer->setInterval(1000);
    connect(sendCodeAgainTimer, &QTimer::timeout, this, &CreateNewAccountWidget::sendCodeAginTimerTick);
    sendCodeAgainTimer->start();
}

void CreateNewAccountWidget::sendCodeAginTimerTick()
{
    m_sendCodeAgainTimerValue--;
    m_ui->codeTimerLabel->setText(QObject::tr("Send code again in ") + QString::number(m_sendCodeAgainTimerValue) + QObject::tr("s"));

    if (m_sendCodeAgainTimerValue <= 0)
    {
        auto sendCodeAgainTimer = qobject_cast<QTimer*>(sender());
        sendCodeAgainTimer->stop();
        sendCodeAgainTimer->deleteLater();

        m_ui->codeTimerLabel->hide();
        m_ui->sendCodeAgainButton->setEnabled(true);
    }
}
