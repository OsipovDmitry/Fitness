#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

#include "messages.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0, quint64 = 0, const QString& = QString());
    ~LoginWidget();

signals:
    void sgnLogin(quint32);
    void sgnForgotPassword();
    void sgnCreateNewAccount();

private slots:
    void on_loginButton_clicked();
    void on_forgotPasswordButton_clicked();
    void on_creaneNewAccountButton_clicked();

private:
    void failedLogin(LoginAnswerMessage::ErrorType);

    quint32 region() const { return 7; }

    Ui::LoginWidget *m_ui;
};

#endif // LOGINWIDGET_H
