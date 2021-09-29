#include "tabellatestatadocumento.h"
#include "../widgetsUtility/dialoginfoalert.h"

QString tabellaTestataDocumento::getNote() const
{
    return note;
}

void tabellaTestataDocumento::setNote(const QString &value)
{
    note = value;
}

int tabellaTestataDocumento::getIdTestata() const
{
    return idTestata;
}

void tabellaTestataDocumento::setIdTestata(int value)
{
    idTestata = value;
}

QDate tabellaTestataDocumento::getDataTesDoc() const
{
    return dataTesDoc;
}

void tabellaTestataDocumento::setDataTesDoc(const QDate &value)
{
    dataTesDoc = value;
}

tabellaTestataDocumento::tabellaTestataDocumento(dataBaseConnection * m_dataBase):
    tabellaBase(m_dataBase)
{

}

void tabellaTestataDocumento::resetOggetto()
{
    dataTesDoc=QDate();
    idTestata=0;
    note="";
}

bool tabellaTestataDocumento::insertOggetto()
{
    if(idTestata==0) return false;
    qyeryToDo="";
    qyeryToDo=QString("select  count(*) from testatadoc where idtestata=%1")
            .arg(idTestata);
    doQuery();
    if(query->next())
    {
        if(query->value(0).toInt()>=1)
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Chiave testata duplicata",2);
            tmpDialog.exec();
            return false;
        }
    }
    qyeryToDo=QString("insert into testatadoc (idtestata, note, data1) values (%1,'%2','%3')")
            .arg(idTestata)
            .arg(note)
            .arg(dataTesDoc.toString("yyyyMMdd"));
    if(doQuery()) return true;
    return false;
}

bool tabellaTestataDocumento::uploadOggetto()
{
    if(idTestata==0) return false;
    qyeryToDo="";
    qyeryToDo=QString("update testatadoc set note='%1', data1='%2' where idtestata=%3")
            .arg(note)
            .arg(dataTesDoc.toString("yyyyMMdd"))
            .arg(idTestata);
    if(doQuery()) return true;
    return false;
}

bool tabellaTestataDocumento::loadOggetto()
{
    if(idTestata==0) return false;
    qyeryToDo="";
    qyeryToDo=QString("select  * from testatadoc where idtestata=%1")
            .arg(idTestata);
    doQuery();
    while (query->next())
    {
        idTestata=query->value(query->record().indexOf("idtestata")).toInt();
        note=query->value(query->record().indexOf("note")).toString();
        dataTesDoc=query->value(query->record().indexOf("data1")).toDate();
        return true;
    }
    dialogInfoAlert tmpDialog;
    tmpDialog.init("Errore","Nessun articolo trovato con la chiave",2);
    tmpDialog.exec();
    return false;
}

void tabellaTestataDocumento::deleteOggetto()
{
    if(idTestata==0) return;
    qyeryToDo="";
    qyeryToDo=QString("delete from testatadoc where idtestata=%1")
            .arg(idTestata);
    doQuery();
}
