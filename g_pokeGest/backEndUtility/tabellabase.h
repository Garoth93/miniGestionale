#ifndef TABELLABASE_H
#define TABELLABASE_H

#include <QDebug>
#include <QString>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlRecord>
#include "databaseconnection.h"
#include <QSqlError>

class tabellaBase
{
protected:
    QSqlQuery * query;
    QString qyeryToDo="";
    dataBaseConnection * m_dataBase;
public:
    tabellaBase(dataBaseConnection * m_dataBase=nullptr);
    virtual ~tabellaBase() = default;
    virtual bool doQuery();
    void setQyeryToDo(const QString &value);
};

#endif // TABELLABASE_H
