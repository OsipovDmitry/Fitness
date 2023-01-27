#ifndef CUSTOMDELETER_H
#define CUSTOMDELETER_H

#include <QString>

inline QString phonenumber2String(quint64 phonenumber)
{
    auto str = QString::number(phonenumber);
    return QString(9 - str.length(), '0') + str;
}

inline quint64 string2Phonenumber(const QString& str)
{
    return str.toULongLong();
}

#endif // CUSTOMDELETER_H
