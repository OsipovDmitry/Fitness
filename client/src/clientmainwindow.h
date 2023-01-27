#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class ClientMainWindow;
}

class ClientMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientMainWindow(QWidget *parent = 0);
    ~ClientMainWindow();

private:
    // ConnectoToHostWidget
    void connectedToServer();

    // LoginWidget
    void login(quint64);
    void forgotPassword();
    void createNewAccount();

    // ForgotPasswordWidget
    void passwordChanged(quint64, QString);
    void backToLogin();

    // CreateNewAccount
    void accountCreated(quint64, QString, QString, QString);

    Ui::ClientMainWindow *m_ui;
};

#endif // CLIENTMAINWINDOW_H
