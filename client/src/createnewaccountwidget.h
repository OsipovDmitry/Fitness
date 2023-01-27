#ifndef CREATENEWACCOUNTWIDGET_H
#define CREATENEWACCOUNTWIDGET_H

#include <QWidget>

#include <messages.h>

namespace Ui {
class CreateNewAccountWidget;
}

class CreateNewAccountWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreateNewAccountWidget(QWidget *parent = 0);
    ~CreateNewAccountWidget();

    quint32 region() const { return 7; }
    quint64 phonenumber() const;
    QString password() const;

signals:
    void sgnAccountCreated(quint64, QString, QString, QString);
    void sgnBack();

private slots:
    void on_backButton_clicked();
    void on_sendMeCodeButton_clicked();
    void on_codeOkButton_clicked();
    void on_sendCodeAgainButton_clicked();
    void on_createAccountButton_clicked();

private:
    void failedPhonenumber();
    void phonenumberRegistered();
    void failedCode();
    void unequalPasswords();
    void emptyFirstName();
    void emptyLastName();
    void emptyPassword();
    void failedCreatingAccount(CreateNewAccountAnswerMessage::ResultType);

    void runSendCodeAgainTimer();
    void sendCodeAginTimerTick();

    Ui::CreateNewAccountWidget *m_ui;
    quint64 m_phonenumber;
    quint32 m_code;
    int m_sendCodeAgainTimerValue;
};

#endif // CREATENEWACCOUNTWIDGET_H
