#include <QDebug>
#include <QTcpSocket>

#include "servercontroller.h"
#include "database.h"

ServerController::ServerController()
    : QTcpServer()
{
    if (!listen(QHostAddress::Any, 2323))
    {
        qDebug() << "QTCPServer::listen returned error";
    }
}

void ServerController::incomingConnection(qintptr handle)
{
    auto socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(handle))
    {
        qDebug() << "QTcpSocket::setSocketDescriptor returned error";
    }
    connect(socket, &QTcpSocket::readyRead, this, &ServerController::sReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    qDebug() << "connected " + QString::number(handle);
}

void ServerController::sReadyRead()
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
            sendToClient(socket, loginUser(LoginRequestMessage::read(messageId, stream)));
            break;
        }
        case MessageType::ForgotPassword:
        {
            sendToClient(socket, forgotPassword(ForgotPasswordRequestMessage::read(messageId, stream)));
            break;
        }
        case MessageType::ChangePassword:
        {
            sendToClient(socket, changePassword(ChangePasswordRequestMessage::read(messageId, stream)));
            break;
        }
        case MessageType::GetUserInfo:
        {
            sendToClient(socket, getUserInfo(GetUserInfoRequestMessage::read(messageId, stream)));
            break;
        }
        case MessageType::CheckPhonenumber:
        {
            sendToClient(socket, checkPhonenumber(CheckPhonenumberRequestMessage::read(messageId, stream)));
            break;
        }
        case MessageType::CreateNewAccount:
            sendToClient(socket, createNewAccount(CreateNewAccountRequestMessage::read(messageId, stream)));
            break;
        default:
            break;
        }
    }
}

void ServerController::sendToClient(QTcpSocket* socket, const AbstractMessage& msg)
{
    QByteArray byteArray;
    byteArray.clear();

    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << static_cast<MesageSizeType>(0) << castFromMessageType(msg.type()) << msg.id();
    msg.write(stream);
    stream.device()->seek(0);
    stream << static_cast<MesageSizeType>(byteArray.size() - sizeof(MesageSizeType));

    socket->write(byteArray);
}

LoginAnswerMessage ServerController::loginUser(const LoginRequestMessage& message)
{
    LoginAnswerMessage::ErrorType result = LoginAnswerMessage::ErrorType::NoError;
    quint64 userId = 0;

    if (result == LoginAnswerMessage::ErrorType::NoError)
    {
        if (!Database::instance().findUserIdByPhonenumber(message.phonenumber, userId))
            result = LoginAnswerMessage::ErrorType::PhonenumberNotFound;

        if (result == LoginAnswerMessage::ErrorType::NoError)
        {
            QString userPassword;
            if (!Database::instance().userPasswordByUserId(userId, userPassword))
                result = LoginAnswerMessage::ErrorType::PasswordNotFound;

            if (result == LoginAnswerMessage::ErrorType::NoError)
                if (userPassword != message.password)
                    result = LoginAnswerMessage::ErrorType::WrongPassword;
        }
    }

    return LoginAnswerMessage(message, result, userId);
}

ForgotPasswordAnswerMessage ServerController::forgotPassword(const ForgotPasswordRequestMessage& message)
{
    ForgotPasswordAnswerMessage::ErrorType result = ForgotPasswordAnswerMessage::ErrorType::NoError;
    quint64 userId = 0;
    quint32 code = 0;

    if (result == ForgotPasswordAnswerMessage::ErrorType::NoError)
    {
        if (!Database::instance().findUserIdByPhonenumber(message.phonenumber, userId))
            result = ForgotPasswordAnswerMessage::ErrorType::PhonenumberNotFound;

        if (result == ForgotPasswordAnswerMessage::ErrorType::NoError)
        {
            code = 1234;
            // send code to phonenumber
        }
    }

    return ForgotPasswordAnswerMessage(message, result, userId, code);
}

ChangePasswordAnswerMessage ServerController::changePassword(const ChangePasswordRequestMessage& message)
{
    ChangePasswordAnswerMessage::ErrorType result = ChangePasswordAnswerMessage::ErrorType::NoError;

    if (result == ChangePasswordAnswerMessage::ErrorType::NoError)
    {
        if (!Database::instance().changeUserPassword(message.userId, message.password))
            result = ChangePasswordAnswerMessage::ErrorType::PasswordNotChanged;
    }

    return ChangePasswordAnswerMessage(message, result);
}

GetUserInfoAnswerMessage ServerController::getUserInfo(const GetUserInfoRequestMessage& message)
{
    QString firstname;
    QString lastname;
    quint64 phonenumber = 0;
    QByteArray picture;
    quint64 coachId = 0;

    auto& database = Database::instance();

    database.userFirstLastNamesByUserId(message.userId, firstname, lastname);
    database.userPhonenumberByUserId(message.userId, phonenumber);
    database.userPictureByUserId(message.userId, picture);
    database.coachIdByUserId(message.userId, coachId);

    return GetUserInfoAnswerMessage(message, firstname, lastname, phonenumber, picture, coachId);
}

CheckPhonenumberAnswerMessage ServerController::checkPhonenumber(const CheckPhonenumberRequestMessage& message)
{
    quint64 userId;
    quint32 code = 0;

    CheckPhonenumberAnswerMessage::ResultType result =
            Database::instance().findUserIdByPhonenumber(message.phonenumber, userId) ?
            CheckPhonenumberAnswerMessage::ResultType::PhonenumberRegistered :
            CheckPhonenumberAnswerMessage::ResultType::PhonenumberNotRegistered;

    code = 1234;
    // send code to phonenumber

    return CheckPhonenumberAnswerMessage(message, result, code);
}

CreateNewAccountAnswerMessage ServerController::createNewAccount(const CreateNewAccountRequestMessage& message)
{
    CreateNewAccountAnswerMessage::ResultType result =
            Database::instance().createNewAccount(message.phonenumber, message.firstName, message.lastName, message.password) ?
            CreateNewAccountAnswerMessage::ResultType::AccountCreated :
            CreateNewAccountAnswerMessage::ResultType::AccountNotCreated;

    return CreateNewAccountAnswerMessage(message, result);
}
