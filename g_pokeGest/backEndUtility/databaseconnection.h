#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>


class dataBaseConnection
{
private:
    QSqlDatabase * m_dataBase;
    QString m_hostName;
    QString m_dataBaseName;
    QString m_utente;
    QString m_password;
    int m_porta;
public:
    dataBaseConnection();
    virtual void init();
    virtual bool chiamaConnesione();
    QSqlDatabase *dataBase() const;
    void setDataBase(QSqlDatabase *dataBase);
    QString hostName() const;
    void setHostName(const QString &hostName);
    QString dataBaseName() const;
    void setDataBaseName(const QString &dataBaseName);
    QString utente() const;
    void setUtente(const QString &utente);
    int porta() const;
    void setPorta(int porta);
};

#endif // DATABASECONNECTION_H
