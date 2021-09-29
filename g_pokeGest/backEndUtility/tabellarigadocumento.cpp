#include "tabellarigadocumento.h"

QString tabellarigadocumento::getNote() const
{
    return note;
}

void tabellarigadocumento::setNote(const QString &value)
{
    note = value;
}

int tabellarigadocumento::getIdTestata() const
{
    return idTestata;
}

void tabellarigadocumento::setIdTestata(int value)
{
    idTestata = value;
}

double tabellarigadocumento::getPrezzo() const
{
    return prezzo;
}

void tabellarigadocumento::setPrezzo(double value)
{
    prezzo = value;
}

QString tabellarigadocumento::getTipoDoc() const
{
    return tipoDoc;
}

void tabellarigadocumento::setTipoDoc(const QString &value)
{
    tipoDoc = value;
}

QString tabellarigadocumento::getCodiceArticolo() const
{
    return codiceArticolo;
}

void tabellarigadocumento::setCodiceArticolo(const QString &value)
{
    codiceArticolo = value;
}

int tabellarigadocumento::getNumeroRiga() const
{
    return numeroRiga;
}

void tabellarigadocumento::setNumeroRiga(int value)
{
    numeroRiga = value;
}

void tabellarigadocumento::resetOggetto()
{
    note="";
    idTestata=0;
    prezzo=0;
    tipoDoc="";
    codiceArticolo="";
    numeroRiga=0;
}

void tabellarigadocumento::insertOggetto()
{
    if(idTestata==0) return;
    qyeryToDo=QString("insert into rigadoc (idtestata, numeroriga, cdart, tipoDocumento, prezzo, note) "
                      "values (%1,%2,'%3','%4',%5,'%6')")
            .arg(idTestata)
            .arg(numeroRiga)
            .arg(codiceArticolo)
            .arg(tipoDoc)
            .arg(prezzo)
            .arg(note);
    doQuery();
}

bool tabellarigadocumento::loadOggetto()
{
    if(idTestata==0) return false;
    qyeryToDo="";
    qyeryToDo=QString("select  * from rigadoc where idtestata=%1 and numeroriga=%2")
            .arg(idTestata)
            .arg(numeroRiga);
    doQuery();
    while(query->next())
    {
        note=query->value(query->record().indexOf("note")).toString();
        idTestata=query->value(query->record().indexOf("idtestata")).toInt();
        prezzo=query->value(query->record().indexOf("prezzo")).toDouble();
        tipoDoc=query->value(query->record().indexOf("tipodocumento")).toString();
        codiceArticolo=query->value(query->record().indexOf("cdart")).toString();
        numeroRiga=query->value(query->record().indexOf("numeroriga")).toInt();
        return true;
    }
    return false;
}

void tabellarigadocumento::deleteOggetto()
{
    if(idTestata==0) return;
    qyeryToDo=QString("delete from rigadoc where idtestata=%1 and numeroriga=%2")
            .arg(idTestata)
            .arg(numeroRiga);
    doQuery();
}

tabellarigadocumento::tabellarigadocumento(dataBaseConnection * m_dataBase):
    tabellaBase(m_dataBase)
{

}
