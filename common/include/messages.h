#ifndef MESSAGES_H
#define MESSAGES_H

#include <QDataStream>
#include <QSharedPointer>

#include "enumclass.h"
#include "idgenerator.h"

#define DECLARE_MESSAGE(Class, Type) \
    protected: \
        explicit Class(MessageIdType id) : MessageBase(id) {} \
    public: \
        MessageType type() const override { return Type; } \
        static Class read(MessageIdType id, QDataStream& stream) { Class result(id); result.read(stream); return result; }

ENUMCLASS(MessageType, quint16, Login, ForgotPassword, ChangePassword, GetUserInfo, CheckPhonenumber, CreateNewAccount)

using MesageSizeType = quint32;
using MessageIdType = quint16;

class AbstractMessage
{
public:
    virtual ~AbstractMessage() = default;
    virtual MessageType type() const = 0;
    virtual MessageIdType id() const = 0;

    virtual void write(QDataStream&) const = 0;
    virtual void read(QDataStream&) = 0;
};

class MessageBase : public AbstractMessage
{
public:
    explicit MessageBase() : m_id(s_idGenerator.generate()) {}
    explicit MessageBase(MessageIdType id) : m_id(id) {}
    MessageIdType id() const override { return m_id; }

protected:
    MessageIdType m_id;

    static IdGenerator<MessageIdType> s_idGenerator;
};

class LoginRequestMessage : public MessageBase
{
    DECLARE_MESSAGE(LoginRequestMessage, MessageType::Login)
public:
    LoginRequestMessage(quint64, const QString&);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    quint64 phonenumber;
    QString password;
};

class LoginAnswerMessage : public MessageBase
{
    DECLARE_MESSAGE(LoginAnswerMessage, MessageType::Login)
public:
    ENUMCLASS(ErrorType, quint32, NoError, PhonenumberNotFound, PasswordNotFound, WrongPassword)

    LoginAnswerMessage(const LoginRequestMessage&, ErrorType, quint64);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    ErrorType result;
    quint64 userId;
};

class ForgotPasswordRequestMessage : public MessageBase
{
    DECLARE_MESSAGE(ForgotPasswordRequestMessage, MessageType::ForgotPassword)
public:
    ForgotPasswordRequestMessage(quint64);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    quint64 phonenumber;
};

class ForgotPasswordAnswerMessage : public MessageBase
{
    DECLARE_MESSAGE(ForgotPasswordAnswerMessage, MessageType::ForgotPassword)
public:
    ENUMCLASS(ErrorType, quint32, NoError, PhonenumberNotFound)

    ForgotPasswordAnswerMessage(const ForgotPasswordRequestMessage&, ErrorType, quint64, quint32);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    ErrorType result;
    quint64 userId;
    quint32 code;
};

class ChangePasswordRequestMessage : public MessageBase
{
    DECLARE_MESSAGE(ChangePasswordRequestMessage, MessageType::ChangePassword)
public:
    ChangePasswordRequestMessage(quint64, const QString&);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    quint64 userId;
    QString password;
};

class ChangePasswordAnswerMessage : public MessageBase
{
    DECLARE_MESSAGE(ChangePasswordAnswerMessage, MessageType::ChangePassword)
public:
    ENUMCLASS(ErrorType, quint32, NoError, PasswordNotChanged)

    ChangePasswordAnswerMessage(const ChangePasswordRequestMessage&, ErrorType);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    ErrorType result;
};

class GetUserInfoRequestMessage : public MessageBase
{
    DECLARE_MESSAGE(GetUserInfoRequestMessage, MessageType::GetUserInfo)
public:
    GetUserInfoRequestMessage(quint64);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    quint64 userId;
};

class GetUserInfoAnswerMessage : public MessageBase
{
    DECLARE_MESSAGE(GetUserInfoAnswerMessage, MessageType::GetUserInfo)
public:
    GetUserInfoAnswerMessage(const GetUserInfoRequestMessage&, const QString&, const QString&, quint64, const QByteArray&, quint64);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    QString firstname;
    QString lastname;
    quint64 phonenumber;
    QByteArray picture;
    quint64 coachId;
};

class CheckPhonenumberRequestMessage : public MessageBase
{
    DECLARE_MESSAGE(CheckPhonenumberRequestMessage, MessageType::CheckPhonenumber)
public:
    CheckPhonenumberRequestMessage(quint64);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    quint64 phonenumber;
};

class CheckPhonenumberAnswerMessage : public MessageBase
{
    DECLARE_MESSAGE(CheckPhonenumberAnswerMessage, MessageType::CheckPhonenumber)
public:
    ENUMCLASS(ResultType, quint32, PhonenumberRegistered, PhonenumberNotRegistered)

    CheckPhonenumberAnswerMessage(const CheckPhonenumberRequestMessage&, ResultType, quint32);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    ResultType result;
    quint32 code;
};

class CreateNewAccountRequestMessage : public MessageBase
{
    DECLARE_MESSAGE(CreateNewAccountRequestMessage, MessageType::CreateNewAccount)
public:
    CreateNewAccountRequestMessage(quint64, const QString&, const QString&, const QString&);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    quint64 phonenumber;
    QString firstName;
    QString lastName;
    QString password;
};

class CreateNewAccountAnswerMessage : public MessageBase
{
    DECLARE_MESSAGE(CreateNewAccountAnswerMessage, MessageType::CreateNewAccount)
public:
    ENUMCLASS(ResultType, quint32, AccountCreated, AccountNotCreated)

    CreateNewAccountAnswerMessage(const CreateNewAccountRequestMessage&, ResultType);

    void write(QDataStream&) const override;
    void read(QDataStream&) override;

    ResultType result;
};

#endif // MESSAGES_H
