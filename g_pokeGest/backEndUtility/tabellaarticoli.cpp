#include "tabellaarticoli.h"
#include "../widgetsUtility/dialoginfoalert.h"

QString tabellaArticoli::getCodiceArticolo() const
{
    return codiceArticolo;
}

void tabellaArticoli::setCodiceArticolo(const QString &value)
{
    codiceArticolo = value;
}

QString tabellaArticoli::getDescrizione() const
{
    return descrizione;
}

void tabellaArticoli::setDescrizione(const QString &value)
{
    descrizione = value;
}

QString tabellaArticoli::getEspansione() const
{
    return espansione;
}

void tabellaArticoli::setEspansione(const QString &value)
{
    espansione = value;
}

QString tabellaArticoli::getRarezza() const
{
    return rarezza;
}

void tabellaArticoli::setRarezza(const QString &value)
{
    rarezza = value;
}

QString tabellaArticoli::getLingua() const
{
    return lingua;
}

void tabellaArticoli::setLingua(const QString &value)
{
    lingua = value;
}

QString tabellaArticoli::getTipologia() const
{
    return tipologia;
}

void tabellaArticoli::setTipologia(const QString &value)
{
    tipologia = value;
}

QDate tabellaArticoli::getDataEspansione() const
{
    return dataEspansione;
}

void tabellaArticoli::setDataEspansione(const QDate &value)
{
    dataEspansione = value;
}

QDate tabellaArticoli::getData1() const
{
    return data1;
}

void tabellaArticoli::setData1(const QDate &value)
{
    data1 = value;
}

QString tabellaArticoli::getFill1() const
{
    return fill1;
}

void tabellaArticoli::setFill1(const QString &value)
{
    fill1 = value;
}

tabellaArticoli::tabellaArticoli(dataBaseConnection * m_dataBase):
    tabellaBase(m_dataBase)
{

}

void tabellaArticoli::resetOggetto()
{
    codiceArticolo="";
    descrizione="";
    espansione="";
    rarezza="";
    lingua="";
    tipologia="";
    dataEspansione=QDate();
    data1=QDate();
    fill1="";
    qyeryToDo="";
}

void tabellaArticoli::insertOggetto()
{
    if(codiceArticolo=="") return;
    qyeryToDo="";
    qyeryToDo=QString("select  count(*) from artico where cdart='%1'")
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
    qyeryToDo=QString("insert into artico (cdart, descr, espansione, "
                      "rarezza, lingua, tipologia, dataespansione, data1, fill1) "
                      "values ('%1','%2','%3','%4','%5','%6','%7','%8','%9')")
            .arg(codiceArticolo)
            .arg(descrizione)
            .arg(espansione)
            .arg(rarezza)
            .arg(lingua)
            .arg(tipologia)
            .arg(dataEspansione.toString("yyyyMMdd"))
            .arg(data1.toString("yyyyMMdd"))
            .arg(fill1);
    doQuery();
}

void tabellaArticoli::uploadOggetto()
{
    if(codiceArticolo=="") return;
    qyeryToDo="";
//    qyeryToDo=QString("select  count(*) from artico where cdart='%1'")
//            .arg(codiceArticolo);
//    doQuery();
//    if(query->value(0).toString()==0)
//    {
//        dialogInfoAlert tmpDialog;
//        tmpDialog.init("Errore","Chiave duplicata",2);
//        tmpDialog.exec();
//    }
    qyeryToDo=QString("update artico set descr='%1', espansione='%2', "
                      "rarezza='%3', lingua='%4', tipologia='%5', dataespansione='%6', data1='%7', fill1='%8'"
                      " where cdart='%9'")
            .arg(descrizione)
            .arg(espansione)
            .arg(rarezza)
            .arg(lingua)
            .arg(tipologia)
            .arg(dataEspansione.toString("yyyyMMdd"))
            .arg(data1.toString("yyyyMMdd"))
            .arg(fill1)
            .arg(codiceArticolo);
    doQuery();
}

bool tabellaArticoli::loadOggetto()
{
    if(codiceArticolo=="") return false;
    qyeryToDo="";
    qyeryToDo=QString("select  * from artico where cdart='%1'")
            .arg(codiceArticolo);
    doQuery();
    while (query->next())
    {
        codiceArticolo=query->value(query->record().indexOf("cdart")).toString();
        descrizione=query->value(query->record().indexOf("descr")).toString();
        espansione=query->value(query->record().indexOf("espansione")).toString();
        rarezza=query->value(query->record().indexOf("rarezza")).toString();
        lingua=query->value(query->record().indexOf("lingua")).toString();
        tipologia=query->value(query->record().indexOf("tipologia")).toString();
        dataEspansione=query->value(query->record().indexOf("dataespansione")).toDate();
        data1=query->value(query->record().indexOf("data1")).toDate();
        fill1=query->value(query->record().indexOf("fill1")).toString();
        return true;
    }
    dialogInfoAlert tmpDialog;
    tmpDialog.init("Errore","Nessun articolo trovato con la chiave",2);
    tmpDialog.exec();
    return false;
}

void tabellaArticoli::loadOggettoConQuery(QString qry)
{
    if(qry=="")
    {

    }
    else
    {

    }
}

void tabellaArticoli::deleteOggetto()
{
    if(codiceArticolo=="") return;
    qyeryToDo="";
    qyeryToDo=QString("select  count(*) from deposito where cdart='%1'")
            .arg(codiceArticolo);
    doQuery();
    if(query->next())
    {
        if(query->value(0).toInt()>=1)
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Chiave presente in altre tabelle impossibile eliminare",2);
            tmpDialog.exec();
            return;
        }
    }
    qyeryToDo="";
    qyeryToDo=QString("select  count(*) from rigadoc where cdart='%1'")
            .arg(codiceArticolo);
    doQuery();
    if(query->next())
    {
        if(query->value(0).toInt()>=1)
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Chiave presente in altre tabelle impossibile eliminare",2);
            tmpDialog.exec();
            return;
        }
    }
    qyeryToDo="";
    qyeryToDo=QString("delete from artico where cdart='%1'")
            .arg(codiceArticolo);
    doQuery();
}
