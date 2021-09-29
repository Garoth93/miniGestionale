#include "tabellabase.h"

tabellaBase::tabellaBase(dataBaseConnection * db)
{
    m_dataBase=db;
    if(m_dataBase)
        query=new QSqlQuery(*m_dataBase->dataBase());
}

bool tabellaBase::doQuery()
{
    qDebug() << qyeryToDo;
    query->clear();
    query->prepare(qyeryToDo);
    bool toRet=false;
    toRet=query->exec();
    qDebug() << query->lastError();
    return toRet;
}

void tabellaBase::setQyeryToDo(const QString &value)
{
    qyeryToDo = value;
}
