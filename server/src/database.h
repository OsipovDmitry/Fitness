#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include <singletoon.h>
#include <noncopyble.h>

class Database
{
    SINGLETON(Database)
    NONCOPYBLE(Database)
public:
    QSqlDatabase sqlDatabase() const;

    bool findUserIdByPhonenumber(quint64, quint64&) const;

    bool userPhonenumberByUserId(quint64, quint64&) const;
    bool userPasswordByUserId(quint64, QString&) const;
    bool userFirstLastNamesByUserId(quint64, QString&, QString&) const;
    bool userPictureByUserId(quint64, QByteArray&) const;
    bool changeUserPassword(quint64, const QString&) const;
    bool createNewAccount(quint64, const QString&, const QString&, const QString&);

    bool coachIdByUserId(quint64, quint64&) const;
    bool athleteIdsByUserId(quint64, QVector<quint64>&) const;

private:
    Database();
    void createTables();
    void fillTablesFromTestData();

    static void checkErrors(const QSqlQuery&);

    QSqlDatabase m_sqlBase;
};

#endif // DATABASE_H
