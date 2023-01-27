#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <QTcpServer>

#include <singletoon.h>
#include <noncopyble.h>
#include <messages.h>

class AbstractMessage;

class ServerController : public QTcpServer
{
    Q_OBJECT
    SINGLETON(ServerController)
    NONCOPYBLE(ServerController)

protected:
    ServerController();

    void incomingConnection(qintptr) override;

private:
    void sReadyRead();
    void sendToClient(QTcpSocket*, const AbstractMessage&);

    LoginAnswerMessage loginUser(const LoginRequestMessage&);
    ForgotPasswordAnswerMessage forgotPassword(const ForgotPasswordRequestMessage&);
    ChangePasswordAnswerMessage changePassword(const ChangePasswordRequestMessage&);
    GetUserInfoAnswerMessage getUserInfo(const GetUserInfoRequestMessage&);
    CheckPhonenumberAnswerMessage checkPhonenumber(const CheckPhonenumberRequestMessage&);
    CreateNewAccountAnswerMessage createNewAccount(const CreateNewAccountRequestMessage&);
};

#endif // SERVERCONTROLLER_H
