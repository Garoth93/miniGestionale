#include "tabelladeposito.h"
#include "../widgetsUtility/dialoginfoalert.h"

QString tabellaDeposito::getCodiceArticolo() const
{
    return codiceArticolo;
}

void tabellaDeposito::setCodiceArticolo(const QString &value)
{
    codiceArticolo = value;
}

double tabellaDeposito::getQta() const
{
    return qta;
}

void tabellaDeposito::setQta(double value)
{
    qta = value;
}

QString tabellaDeposito::getPosizione() const
{
    return posizione;
}

void tabellaDeposito::setPosizione(const QString &value)
{
    posizione = value;
}

void tabellaDeposito::resetOggetto()
{
    codiceArticolo="";
    qta=0;
    posizione="";
    qyeryToDo="";
}

void tabellaDeposito::insertOggetto()
{
    if(codiceArticolo=="") return;
    if(controlloCampi()) return;
    qyeryToDo="";
    qyeryToDo=QString("select  count(*) from deposito where cdart='%1'")
            .arg(codiceArticolo);
    doQuery();
    if(query->next())
    {
        if(query->value(0).toInt()>=1)
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Chiave duplicata",2);
            tmpDialog.exec();
            return;
        }
    }
    qyeryToDo="";
    qyeryToDo=QString("select  count(*) from artico where cdart='%1'")
            .arg(codiceArticolo);
    doQuery();
    if(query->next())
    {
        if(!(query->value(0).toInt()>=1))
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Chiave articolo non presente in anagrafica",2);
            tmpDialog.exec();
            return;
        }
    }
    qyeryToDo=QString("insert into deposito (cdart,qta, posizione) "
                      "values ('%1',%2,'%3')")
            .arg(codiceArticolo)
            .arg(qta)
            .arg(posizione);
    doQuery();
}

void tabellaDeposito::uploadOggetto()
{
    if(codiceArticolo=="") return;
    if(controlloCampi()) return;
    qyeryToDo="";
    qyeryToDo=QString("update deposito set qta=%1,posizione='%2' where cdart='%3'")
            .arg(qta)
            .arg(posizione)
            .arg(codiceArticolo);
    doQuery();
}

bool tabellaDeposito::loadOggetto()
{
    if(codiceArticolo=="") return false;
    qyeryToDo="";
    qyeryToDo=QString("select  * from deposito where cdart='%1'")
            .arg(codiceArticolo);
    doQuery();
    while (query->next())
    {
        codiceArticolo=query->value(query->record().indexOf("cdart")).toString();
        qta=query->value(query->record().indexOf("qta")).toDouble();
        posizione=query->value(query->record().indexOf("posizione")).toString();
        return true;
    }
    dialogInfoAlert tmpDialog;
    tmpDialog.init("Errore","Nessun riga trovata con la chiave",2);
    tmpDialog.exec();
    return false;
}

void tabellaDeposito::deleteOggetto()
{
    if(codiceArticolo=="") return;
    qyeryToDo="";
    qyeryToDo=QString("delete from deposito where cdart='%1'")
            .arg(codiceArticolo);
    doQuery();
}

bool tabellaDeposito::controlloCampi()
{
    return false;
}

tabellaDeposito::tabellaDeposito(dataBaseConnection * m_dataBase):
    tabellaBase(m_dataBase)
{

}
