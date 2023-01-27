#include <phonenumber.h>

#include "clientcontroller.h"
#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent, quint64 phonenumber, const QString& password) :
    QWidget(parent),
    m_ui(new Ui::LoginWidget)
{
    m_ui->setupUi(this);
    m_ui->errorLabel->hide();

    if (phonenumber != 0)
        m_ui->phonenumberEdit->setText(phonenumber2String(phonenumber));

    if (!password.isNull())
        m_ui->passwordEdit->setText(password);
}

LoginWidget::~LoginWidget()
{
    delete m_ui;
}

void LoginWidget::on_loginButton_clicked()
{
    m_ui->errorLabel->hide();

    bool isOk = true;
    auto phonenumberString = m_ui->phonenumberEdit->text();
    quint64 phonenumber = static_cast<quint64>(region()) * static_cast<quint64>(1000000000);
    phonenumber += phonenumberString.toULongLong(&isOk);

    if (phonenumberString.length() != 9 || !isOk)
    {
        failedLogin(LoginAnswerMessage::ErrorType::PhonenumberNotFound);
        return;
    }

    auto& controller = ClientController::instance();
    controller.loginRequest(phonenumber, m_ui->passwordEdit->text(), [this](const AbstractMessage& message) {
        auto& loginAnswerMessage = dynamic_cast<const LoginAnswerMessage&>(message);
        if (loginAnswerMessage.result == LoginAnswerMessage::ErrorType::NoError)
        {
            emit sgnLogin(loginAnswerMessage.userId);
        }
        else
            failedLogin(loginAnswerMessage.result);
    });
}

void LoginWidget::on_forgotPasswordButton_clicked()
{
    emit sgnForgotPassword();
}

void LoginWidget::on_creaneNewAccountButton_clicked()
{
    emit sgnCreateNewAccount();
}

void LoginWidget::failedLogin(LoginAnswerMessage::ErrorType error)
{
    QString errorString;

    switch (error)
    {
    case LoginAnswerMessage::ErrorType::NoError:
    {
        break;
    }
    case LoginAnswerMessage::ErrorType::PhonenumberNotFound: {
        errorString = QObject::tr("Pnonenumber has not been found...");
        break;
    }
    case LoginAnswerMessage::ErrorType::PasswordNotFound:
    case LoginAnswerMessage::ErrorType::WrongPassword:
    {
        errorString = QObject::tr("Password is not correct...");
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
