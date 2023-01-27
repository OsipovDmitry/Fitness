#include <QTcpSocket>
#include <QHostAddress>

#include "clientcontroller.h"

ClientController::ClientController()
    : QObject()
{
}

bool ClientController::connectToServer()
{
    disconnectFromServer();

    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientController::sReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, m_socket, &QTcpSocket::deleteLater);

    m_socket->connectToHost(QHostAddress::LocalHost, 2323);
    if (!m_socket->waitForConnected(3000))
        return false;

    return true;
}

void ClientController::disconnectFromServer()
{
    if (m_socket)
        m_socket->disconnectFromHost();
}

void ClientController::loginRequest(quint64 phonenumber, const QString& password, const ServerCallback& callback)
{
    LoginRequestMessage loginRequestMessage(phonenumber, password);
    addServerCallback(loginRequestMessage.id(), callback);
    sendToServer(loginRequestMessage);
}

void ClientController::forgotPasswordRequest(quint64 phonenumber, const ClientController::ServerCallback& callback)
{
    ForgotPasswordRequestMessage forgotPasswordRequestMessage(phonenumber);
    addServerCallback(forgotPasswordRequestMessage.id(), callback);
    sendToServer(forgotPasswordRequestMessage);
}

void ClientController::changePasswordRequest(quint64 userId, const QString& password, const ClientController::ServerCallback& callback)
{
    ChangePasswordRequestMessage changePasswordRequestMessage(userId, password);
    addServerCallback(changePasswordRequestMessage.id(), callback);
    sendToServer(changePasswordRequestMessage);
}

void ClientController::getUserInfoRequest(quint64 userId, const ClientController::ServerCallback& callback)
{
    GetUserInfoRequestMessage getUserInfoRequestMessage(userId);
    addServerCallback(getUserInfoRequestMessage.id(), callback);
    sendToServer(getUserInfoRequestMessage);
}

void ClientController::checkPhonenumber(quint64 phonenumber, const ClientController::ServerCallback& callback)
{
    CheckPhonenumberRequestMessage checkPhonenumberRequestMessage(phonenumber);
    addServerCallback(checkPhonenumberRequestMessage.id(), callback);
    sendToServer(checkPhonenumberRequestMessage);
}

void ClientController::createNewAccount(quint64 phonenumber,
                                        const QString& firstName,
                                        const QString& lastName,
                                        const QString& password,
                                        const ClientController::ServerCallback& callback)
{
    CreateNewAccountRequestMessage createNewAccountRequestMessage(phonenumber, firstName, lastName, password);
    addServerCallback(createNewAccountRequestMessage.id(), callback);
    sendToServer(createNewAccountRequestMessage);
}

void ClientController::sReadyRead()
{
    auto socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;

    if (socket->bytesAvailable() >= sizeof(MesageSizeType))
    {
        QDataStream stream(socket);

        auto socketPos = stream.device()->pos();

        MesageSizeType messageSize;
        stream >> messageSize;

        if (socket->bytesAvailable() < messageSize)
        {
            stream.device()->seek(socketPos);
            stream << messageSize;
            return;
        }

        MessageTypeInternalType messageTypeTmp;
        MessageIdType messageId;
        stream >> messageTypeTmp >> messageId;
        MessageType messageType = castToMessageType(messageTypeTmp);

        switch (messageType)
        {
        case MessageType::Login:
        {
            auto loginAnswerMessage = LoginAnswerMessage::read(messageId, stream);
            callback(loginAnswerMessage);
            break;
        }
        case MessageType::ForgotPassword:
        {
            auto forgotPasswordAnswerMessage = ForgotPasswordAnswerMessage::read(messageId, stream);
            callback(forgotPasswordAnswerMessage);
            break;
        }
        case MessageType::ChangePassword:
        {
            auto changePasswordAnswerMessage = ChangePasswordAnswerMessage::read(messageId, stream);
            callback(changePasswordAnswerMessage);
            break;
        }
        case MessageType::GetUserInfo:
        {
            auto getUserInfoAnswerMessage = GetUserInfoAnswerMessage::read(messageId, stream);
            callback(getUserInfoAnswerMessage);
            break;
        }
        case MessageType::CheckPhonenumber:
        {
            auto checkPhonenumberAnswerMessage = CheckPhonenumberAnswerMessage::read(messageId, stream);
            callback(checkPhonenumberAnswerMessage);
            break;
        }
        case MessageType::CreateNewAccount:
        {
            auto createNewAccountAnswerMessage = CreateNewAccountAnswerMessage::read(messageId, stream);
            callback(createNewAccountAnswerMessage);
            break;
        }
        default:
            break;
        }
    }
}

void ClientController::sendToServer(const AbstractMessage& message)
{
    QByteArray byteArray;
    byteArray.clear();

    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << static_cast<MesageSizeType>(0) << castFromMessageType(message.type()) << message.id();
    message.write(stream);
    stream.device()->seek(0);
    stream << static_cast<MesageSizeType>(byteArray.size() - sizeof(MesageSizeType));

    if (m_socket)
        m_socket->write(byteArray);
}

void ClientController::addServerCallback(MessageIdType id, const ServerCallback& callback)
{
    m_serverCallbacks.insert({id, callback});
}

void ClientController::callback(const AbstractMessage& message)
{
    auto it = m_serverCallbacks.find(message.id());
    if (it != m_serverCallbacks.end())
    {
        it->second(message);
        m_serverCallbacks.erase(it);
    }
}
