#include <QTimer>

#include <phonenumber.h>

#include "clientcontroller.h"
#include "forgotpasswordwidget.h"
#include "ui_forgotpasswordwidget.h"

ForgotPasswordWidget::ForgotPasswordWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ForgotPasswordWidget)
    , m_userId(0)
{
    m_ui->setupUi(this);

    m_ui->enterCodeGroup->hide();
    m_ui->passwordsGroup->hide();
    m_ui->errorLabel->hide();
}

ForgotPasswordWidget::~ForgotPasswordWidget()
{
    delete m_ui;
}

void ForgotPasswordWidget::on_sendMeCodeButton_clicked()
{
    m_ui->errorLabel->hide();

    bool isOk = true;
    auto phonenumberString = m_ui->phonenumberEdit->text();
    quint64 phonenumber = static_cast<quint64>(region()) * static_cast<quint64>(1000000000);
    phonenumber += phonenumberString.toULongLong(&isOk);

    if (phonenumberString.length() != 9 || !isOk)
    {
        failedPhonenumber(ForgotPasswordAnswerMessage::ErrorType::PhonenumberNotFound);
        return;
    }

    auto& controller = ClientController::instance();
    controller.forgotPasswordRequest(phonenumber, [this](const AbstractMessage& message) {
        auto& forgotPasswordAnswerMessage = dynamic_cast<const ForgotPasswordAnswerMessage&>(message);
        if (forgotPasswordAnswerMessage.result == ForgotPasswordAnswerMessage::ErrorType::NoError)
        {
            m_userId = forgotPasswordAnswerMessage.userId;
            m_code = forgotPasswordAnswerMessage.code;
            m_ui->phonenumberEdit->setDisabled(true);
            m_ui->sendMeCodeButton->hide();
            m_ui->enterCodeGroup->show();
            runSendCodeAgainTimer();
        }
        else
            failedPhonenumber(forgotPasswordAnswerMessage.result);
    });
}

void ForgotPasswordWidget::on_sendCodeAgainButton_clicked()
{
    on_sendMeCodeButton_clicked();
}

void ForgotPasswordWidget::on_codeOkButton_clicked()
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
    m_ui->passwordsGroup->show();
}

void ForgotPasswordWidget::on_changePasswordButton_clicked()
{
    if (m_ui->passwordEdit->text() != m_ui->repeatedPasswordEdit->text())
    {
        unequalPasswords();
        return;
    }

    if (m_ui->passwordEdit->text().isEmpty())
    {
        emptyPassword();
        return;
    }

    auto& controller = ClientController::instance();
    controller.changePasswordRequest(m_userId, m_ui->passwordEdit->text(), [this](const AbstractMessage& message) {
        auto& changePasswordAnswerMessage = dynamic_cast<const ChangePasswordAnswerMessage&>(message);
        if (changePasswordAnswerMessage.result == ChangePasswordAnswerMessage::ErrorType::NoError)
        {
            emit sgnPassswordChanged(string2Phonenumber(m_ui->phonenumberEdit->text()), m_ui->passwordEdit->text());
        }
        else
            failedChangingPassword(changePasswordAnswerMessage.result);
    });
}

void ForgotPasswordWidget::failedPhonenumber(ForgotPasswordAnswerMessage::ErrorType error)
{
    QString errorString;

    switch (error)
    {
    case ForgotPasswordAnswerMessage::ErrorType::NoError:
    {
        break;
    }
    case ForgotPasswordAnswerMessage::ErrorType::PhonenumberNotFound: {
        errorString = QObject::tr("Pnonenumber has not been found...");
        break;
    }
    default: {
        errorString = QObject::tr("Unknown error");
        break;
    }
    }

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void ForgotPasswordWidget::failedCode()
{
    QString errorString = QObject::tr("Entered code is wrong");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void ForgotPasswordWidget::unequalPasswords()
{
    QString errorString = QObject::tr("Entered passwords are not equal");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void ForgotPasswordWidget::emptyPassword()
{
    QString errorString = QObject::tr("Password can't be an empty line");

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void ForgotPasswordWidget::failedChangingPassword(ChangePasswordAnswerMessage::ErrorType error)
{
    QString errorString;

    switch (error)
    {
    case ChangePasswordAnswerMessage::ErrorType::NoError:
    {
        break;
    }
    case ChangePasswordAnswerMessage::ErrorType::PasswordNotChanged: {
        errorString = QObject::tr("Password has not been changed...");
        break;
    }
    }

    m_ui->errorLabel->setText(errorString);
    m_ui->errorLabel->show();
}

void ForgotPasswordWidget::runSendCodeAgainTimer()
{
    m_ui->sendCodeAgainButton->setEnabled(false);

    m_sendCodeAgainTimerValue = 5;
    m_ui->codeTimerLabel->show();
    m_ui->codeTimerLabel->setText(QObject::tr("Send code again in ") + QString::number(m_sendCodeAgainTimerValue) + QObject::tr("s"));

    auto sendCodeAgainTimer = new QTimer(this);
    sendCodeAgainTimer->setInterval(1000);
    connect(sendCodeAgainTimer, &QTimer::timeout, this, &ForgotPasswordWidget::sendCodeAginTimerTick);
    sendCodeAgainTimer->start();
}

void ForgotPasswordWidget::sendCodeAginTimerTick()
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

void ForgotPasswordWidget::on_backButton_clicked()
{
    emit sgnBack();
}
