#ifndef FORGOTPASSWORDWIDGET_H
#define FORGOTPASSWORDWIDGET_H

#include <QWidget>

#include <messages.h>

namespace Ui {
class ForgotPasswordWidget;
}

class ForgotPasswordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ForgotPasswordWidget(QWidget *parent = 0);
    ~ForgotPasswordWidget();

signals:
    void sgnPassswordChanged(quint64, QString);
    void sgnBack();

private slots:
    void on_sendMeCodeButton_clicked();
    void on_sendCodeAgainButton_clicked();
    void on_codeOkButton_clicked();
    void on_changePasswordButton_clicked();
    void on_backButton_clicked();

private:
    void failedPhonenumber(ForgotPasswordAnswerMessage::ErrorType);
    void failedCode();
    void unequalPasswords();
    void emptyPassword();
    void failedChangingPassword(ChangePasswordAnswerMessage::ErrorType);

    void runSendCodeAgainTimer();
    void sendCodeAginTimerTick();

    quint32 region() const { return 7; }

    Ui::ForgotPasswordWidget *m_ui;
    quint64 m_userId;
    quint32 m_code;
    int m_sendCodeAgainTimerValue;
};

#endif // FORGOTPASSWORDWIDGET_H
