#include "databaseconnection.h"
#include "dialogdatidatabase.h"
#include <QDebug>

QSqlDatabase *dataBaseConnection::dataBase() const
{
    return m_dataBase;
}

void dataBaseConnection::setDataBase(QSqlDatabase *dataBase)
{
    m_dataBase = dataBase;
}

QString dataBaseConnection::hostName() const
{
    return m_hostName;
}

void dataBaseConnection::setHostName(const QString &hostName)
{
    m_hostName = hostName;
}

QString dataBaseConnection::dataBaseName() const
{
    return m_dataBaseName;
}

void dataBaseConnection::setDataBaseName(const QString &dataBaseName)
{
    m_dataBaseName = dataBaseName;
}

QString dataBaseConnection::utente() const
{
    return m_utente;
}

void dataBaseConnection::setUtente(const QString &utente)
{
    m_utente = utente;
}

int dataBaseConnection::porta() const
{
    return m_porta;
}

void dataBaseConnection::setPorta(int porta)
{
    m_porta = porta;
}

dataBaseConnection::dataBaseConnection()
{
    
}

void dataBaseConnection::init()
{
    m_dataBase=new QSqlDatabase();
    *m_dataBase=QSqlDatabase::addDatabase("QPSQL");
    m_hostName="";
    m_dataBaseName="";
    m_utente="";
    m_password="";
    m_porta=0;

    chiamaConnesione();
}

bool dataBaseConnection::chiamaConnesione()
{
    bool isConn=false;
    dialogDatiDatabase dialogTmp;
    dialogTmp.init(m_dataBase,&m_hostName,&m_dataBaseName,&m_utente,&m_password,&m_porta,&isConn);
    dialogTmp.exec();
    return isConn;
}
