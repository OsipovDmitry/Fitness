#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>

#include "database.h"

QSqlDatabase Database::sqlDatabase() const
{
    return m_sqlBase;
}

bool Database::findUserIdByPhonenumber(quint64 phonenumber, quint64& userId) const
{
    QSqlQuery query(m_sqlBase);
    query.prepare("SELECT rowid FROM users WHERE phonenumber = :phonenumber");
    query.bindValue(":phonenumber", phonenumber);
    bool result = query.exec();
    checkErrors(query);

    if (result)
    {
        if (query.first())
        {
            bool isOk = false;
            userId = query.value(0).toULongLong(&isOk);
            result = isOk;
        }
        else
            result = false;
    }

    return result;
}

bool Database::userPhonenumberByUserId(quint64 userId, quint64& phonenumber) const
{
    QSqlQuery query(m_sqlBase);
    query.prepare("SELECT phonenumber FROM users WHERE rowid = :rowid");
    query.bindValue(":rowid", userId);
    bool result = query.exec();
    checkErrors(query);

    if (result)
    {
        if (query.first())
        {
            bool isOk = false;
            phonenumber = query.value(0).toULongLong(&isOk);
            result = isOk;
        }
        else
            result = false;
    }

    return result;
}

bool Database::userPasswordByUserId(quint64 userId, QString& password) const
{
    QSqlQuery query(m_sqlBase);
    query.prepare("SELECT password FROM users WHERE rowid = :rowid");
    query.bindValue(":rowid", userId);
    bool result = query.exec();
    checkErrors(query);

    if (result)
    {
        if (query.first())
        {
            password = query.value(0).toString();
        }
        else
            result = false;
    }

    return result;
}

bool Database::userFirstLastNamesByUserId(quint64 userId, QString& firstname, QString& lastname) const
{
    QSqlQuery query(m_sqlBase);
    query.prepare("SELECT firstname, lastname FROM users WHERE rowid = :rowid");
    query.bindValue(":rowid", userId);
    bool result = query.exec();
    checkErrors(query);

    if (result)
    {
        if (query.first())
        {
            firstname = query.value(0).toString();
            lastname = query.value(1).toString();
        }
        else
            result = false;
    }

    return result;
}

bool Database::userPictureByUserId(quint64 userId, QByteArray& userPicture) const
{
    QSqlQuery query(m_sqlBase);
    query.prepare("SELECT picture FROM users WHERE rowid = :rowid");
    query.bindValue(":rowid", userId);
    bool result = query.exec();
    checkErrors(query);

    if (result)
    {
        if (query.first())
        {
            userPicture = query.value(0).toByteArray();
        }
        else
            result = false;
    }

    return result;
}

bool Database::changeUserPassword(quint64 userId, const QString& password) const
{
    QSqlQuery query(m_sqlBase);
    query.prepare("UPDATE users SET password = :password WHERE rowid = :userId");
    query.bindValue(":userId", userId);
    query.bindValue(":password", password);
    bool result = query.exec();
    checkErrors(query);

    return result;
}

bool Database::createNewAccount(quint64 phonenumber, const QString& firstName, const QString& lastName, const QString& password)
{
    QSqlQuery query(m_sqlBase);
    query.prepare("INSERT INTO users (phonenumber, password, firstname, lastname) "
                  "VALUES (:phonenumber, :password, :firstName, :lastName)");
    query.bindValue(":phonenumber", phonenumber);
    query.bindValue(":password", password);
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":password", password);
    bool result = query.exec();
    checkErrors(query);

    return result;
}

bool Database::coachIdByUserId(quint64 userId, quint64& coachId) const
{
    QSqlQuery query(m_sqlBase);
    query.prepare("SELECT coach_id FROM athlete_coach_connections WHERE athlete_id = :athleteId");
    query.bindValue(":athleteId", userId);
    bool result = query.exec();
    checkErrors(query);

    if (result)
    {
        if (query.first())
        {
            bool isOk = false;
            coachId = query.value(0).toULongLong(&isOk);
            result = isOk;
        }
        else
            coachId = 0u;
    }

    return result;
}

bool Database::athleteIdsByUserId(quint64 userId, QVector<quint64>& athletesIds) const
{
    QSqlQuery query(m_sqlBase);
    query.prepare("SELECT athlete_id FROM athlete_coach_connections WHERE coach_id = :coachId");
    query.bindValue(":coachId", userId);
    bool result = query.exec();
    checkErrors(query);

    if (result)
    {
        athletesIds.reserve(query.size());

        while (query.next() && result)
        {
            bool isOk = false;
            athletesIds.push_back(query.value(0).toULongLong(&isOk));
            result = isOk;
        }
    }

    return result;
}

Database::Database()
{
    m_sqlBase = QSqlDatabase::addDatabase("QSQLITE");
    //m_sqlBase.setDatabaseName("./testDB.db");
    if (!m_sqlBase.open())
    {
        qDebug() << "Database hasn't been opened..";
    }

    createTables();
    fillTablesFromTestData();
}

void Database::createTables()
{
    QSqlQuery query(m_sqlBase);
    query.prepare("CREATE TABLE IF NOT EXISTS users ("
                      "phonenumber INTEGER NOT NULL UNIQUE,"
                      "password TEXT NOT NULL,"
                      "firstname TEXT NOT NULL,"
                      "lastname TEXT NOT NULL,"
                      "picture BLOB"
                  ")");
    query.exec();
    checkErrors(query);

    query.prepare("CREATE TABLE IF NOT EXISTS athlete_coach_connections ("
                      "athlete_id INTEGER NOT NULL UNIQUE,"
                      "coach_id INTEGER NOT NULL,"

                      "FOREIGN KEY (athlete_id)"
                          "REFERENCES users (rowid)"
                          "ON DELETE CASCADE,"
                      "FOREIGN KEY (coach_id)"
                          "REFERENCES users (rowid)"
                          "ON DELETE CASCADE"
                  ")");
    query.exec();
    checkErrors(query);
}

void Database::fillTablesFromTestData()
{
    const QString namesFilename = ":/testdata/names.txt";
    const QString picturesFolder = ":/testdata/faces/";

    QFile namesFile(namesFilename);
    if (!namesFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open test data file: " + namesFilename;
        return;
    }

    for (int i = 0; i < 150; ++i)
    {
        QStringList firstLastNames = QString(namesFile.readLine()).split(QRegExp("\\s+"), QString::SkipEmptyParts);

        int i1 = i / 100;
        int i2 = (i - 100*i1) / 10;
        int i3 = i % 10;
        quint64 phonenumber = 7000000000u + i2 * 10000 + i1 * 100 + i3;

        QString password(5 + rand() % 5, '_');
        for (auto& c : password)
            c = 'a' + rand() % 26;

        QByteArray picture;
        QFile pictureFile(picturesFolder + QString::number(i) + ".png");
        if (pictureFile.open(QIODevice::ReadOnly))
            picture = pictureFile.readAll();

        QSqlQuery query(m_sqlBase);
        query.prepare("INSERT INTO users (phonenumber, password, firstname, lastname, picture) "
                      "VALUES (:phonenumber, :password, :firstname, :lastname, :picture)");
        query.bindValue(":phonenumber", phonenumber);
        query.bindValue(":password", password);
        query.bindValue(":firstname", firstLastNames.at(0));
        query.bindValue(":lastname", firstLastNames.at(1));
        query.bindValue(":picture", picture);

        query.exec();
        checkErrors(query);
    }

    for (int i = 0; i < 150; ++i)
    {
        quint64 athlete_id, coach_id;

        if (i % 15 == 0)
        {
            if (i == 0) continue;
            else
            {
                athlete_id = i + 1;
                coach_id = 0 + 1;
            }
        }
        else
        {
            athlete_id = i + 1;
            coach_id = 15 * (i / 15) + 1;
        }


        QSqlQuery query(m_sqlBase);
        query.prepare("INSERT INTO athlete_coach_connections (athlete_id, coach_id) "
                      "VALUES (:athlete_id, :coach_id)");
        query.bindValue(":athlete_id", athlete_id);
        query.bindValue(":coach_id", coach_id);

        query.exec();
        checkErrors(query);
    }
}

void Database::checkErrors(const QSqlQuery& query)
{
    if (query.lastError().type() != QSqlError::NoError)
    {
        qDebug() << query.lastQuery() + ": " + query.lastError().text();
    }
}
