#include "messages.h"

IdGenerator<MessageIdType> MessageBase::s_idGenerator;

LoginRequestMessage::LoginRequestMessage(quint64 _phonenumber, const QString& _password)
    : MessageBase()
    , phonenumber(_phonenumber)
    , password(_password)

{
}

void LoginRequestMessage::write(QDataStream& stream) const
{
    stream << phonenumber << password;
}

void LoginRequestMessage::read(QDataStream& stream)
{
    stream >> phonenumber >> password;
}

LoginAnswerMessage::LoginAnswerMessage(const LoginRequestMessage& requestMessage, ErrorType _result, quint64 _userId)
    : MessageBase(requestMessage.id())
    , result(_result)
    , userId(_userId)
{
}

void LoginAnswerMessage::write(QDataStream& stream) const
{
    stream << castFromErrorType(result) << userId;
}

void LoginAnswerMessage::read(QDataStream& stream)
{
    ErrorTypeInternalType resultTmp;
    stream >> resultTmp >> userId;

    result = castToErrorType(resultTmp);
}

ForgotPasswordRequestMessage::ForgotPasswordRequestMessage(quint64 _phonenumber)
    : MessageBase()
    , phonenumber(_phonenumber)
{
}

void ForgotPasswordRequestMessage::write(QDataStream& stream) const
{
    stream << phonenumber;
}

void ForgotPasswordRequestMessage::read(QDataStream& stream)
{
    stream >> phonenumber;
}

ForgotPasswordAnswerMessage::ForgotPasswordAnswerMessage(const ForgotPasswordRequestMessage& requestMessage, ErrorType _result, quint64 _userId, quint32 _code)
    : MessageBase(requestMessage.id())
    , result(_result)
    , userId(_userId)
    , code(_code)
{
}

void ForgotPasswordAnswerMessage::write(QDataStream& stream) const
{
    stream << castFromErrorType(result) << userId << code;
}

void ForgotPasswordAnswerMessage::read(QDataStream& stream)
{
    ErrorTypeInternalType resultTmp;
    stream >> resultTmp >> userId >> code;

    result = castToErrorType(resultTmp);
}

ChangePasswordRequestMessage::ChangePasswordRequestMessage(quint64 _userId, const QString& _password)
    : MessageBase()
    , userId(_userId)
    , password(_password)
{
}

void ChangePasswordRequestMessage::write(QDataStream& stream) const
{
    stream << userId << password;
}

void ChangePasswordRequestMessage::read(QDataStream& stream)
{
    stream >> userId >> password;
}

ChangePasswordAnswerMessage::ChangePasswordAnswerMessage(const ChangePasswordRequestMessage& requestMessage, ErrorType _result)
    : MessageBase(requestMessage.id())
    , result(_result)
{
}

void ChangePasswordAnswerMessage::write(QDataStream& stream) const
{
    stream << castFromErrorType(result);
}

void ChangePasswordAnswerMessage::read(QDataStream& stream)
{
    ErrorTypeInternalType resultTmp;
    stream >> resultTmp;

    result = castToErrorType(resultTmp);
}

GetUserInfoRequestMessage::GetUserInfoRequestMessage(quint64 _userId)
    : MessageBase()
    , userId(_userId)
{
}

void GetUserInfoRequestMessage::write(QDataStream& stream) const
{
    stream << userId;
}

void GetUserInfoRequestMessage::read(QDataStream& stream)
{
    stream >> userId;
}

GetUserInfoAnswerMessage::GetUserInfoAnswerMessage(const GetUserInfoRequestMessage& requestMessage,
                                                   const QString& _firstName,
                                                   const QString& _lastName,
                                                   quint64 _phonenumber,
                                                   const QByteArray& _picture,
                                                   quint64 _coachId)
    : MessageBase(requestMessage.id())
    , firstname(_firstName)
    , lastname(_lastName)
    , phonenumber(_phonenumber)
    , picture(_picture)
    , coachId(_coachId)
{
}

void GetUserInfoAnswerMessage::write(QDataStream& stream) const
{
    stream << firstname << lastname << phonenumber << picture << coachId;
}

void GetUserInfoAnswerMessage::read(QDataStream& stream)
{
    stream >> firstname >> lastname >> phonenumber >> picture >> coachId;
}

CheckPhonenumberRequestMessage::CheckPhonenumberRequestMessage(quint64 _phonenumber)
    : MessageBase()
    , phonenumber(_phonenumber)
{
}

void CheckPhonenumberRequestMessage::write(QDataStream& stream) const
{
    stream << phonenumber;
}

void CheckPhonenumberRequestMessage::read(QDataStream& stream)
{
    stream >> phonenumber;
}

CheckPhonenumberAnswerMessage::CheckPhonenumberAnswerMessage(const CheckPhonenumberRequestMessage& requestMessage, ResultType _result, quint32 _code)
    : MessageBase(requestMessage)
    , result(_result)
    , code(_code)
{
}

void CheckPhonenumberAnswerMessage::write(QDataStream& stream) const
{
    stream << castFromResultType(result) << code;
}

void CheckPhonenumberAnswerMessage::read(QDataStream& stream)
{
    ResultTypeInternalType resultTmp;
    stream >> resultTmp >> code;
    result = castToResultType(resultTmp);
}

CreateNewAccountRequestMessage::CreateNewAccountRequestMessage(quint64 _phonenumber,
                                                               const QString& _firstName,
                                                               const QString& _lastName,
                                                               const QString& _password)
    : MessageBase()
    , phonenumber(_phonenumber)
    , firstName(_firstName)
    , lastName(_lastName)
    , password(_password)
{
}

void CreateNewAccountRequestMessage::write(QDataStream& stream) const
{
    stream << phonenumber << firstName << lastName << password;
}

void CreateNewAccountRequestMessage::read(QDataStream& stream)
{
    stream >> phonenumber >> firstName >> lastName >> password;
}

CreateNewAccountAnswerMessage::CreateNewAccountAnswerMessage(const CreateNewAccountRequestMessage& requestMessage, ResultType _result)
    : MessageBase(requestMessage)
    , result(_result)
{
}

void CreateNewAccountAnswerMessage::write(QDataStream& stream) const
{
    stream << castFromResultType(result);
}

void CreateNewAccountAnswerMessage::read(QDataStream& stream)
{
    ResultTypeInternalType resultTmp;
    stream >> resultTmp;
    result = castToResultType(resultTmp);
}
