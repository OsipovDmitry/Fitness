#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <functional>
#include <unordered_map>

#include <QTcpSocket>
#include <QPointer>

#include <singletoon.h>
#include <noncopyble.h>
#include <messages.h>


class ClientController : public QObject
{
    Q_OBJECT
    SINGLETON(ClientController)
    NONCOPYBLE(ClientController)

public:
    using ServerCallback = std::function<void(const AbstractMessage&)>;

    ClientController();

    bool connectToServer();
    void disconnectFromServer();

    void loginRequest(quint64, const QString&, const ServerCallback&);
    void forgotPasswordRequest(quint64, const ServerCallback&);
    void changePasswordRequest(quint64, const QString&, const ClientController::ServerCallback&);
    void getUserInfoRequest(quint64, const ClientController::ServerCallback&);
    void checkPhonenumber(quint64, const ClientController::ServerCallback&);
    void createNewAccount(quint64, const QString&, const QString&, const QString&, const ClientController::ServerCallback&);

private:
    QPointer<QTcpSocket> m_socket;
    std::unordered_map<MessageIdType, ServerCallback> m_serverCallbacks;

    void sReadyRead();
    void sendToServer(const AbstractMessage&);

    void addServerCallback(MessageIdType, const ServerCallback&);
    void callback(const AbstractMessage&);
};

#endif // CLIENTCONTROLLER_H
