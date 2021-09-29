#include "tabellatabvab.h"
#include "../widgetsUtility/dialoginfoalert.h"

QString tabellaTabvab::getTpTab() const
{
    return tpTab;
}

void tabellaTabvab::setTpTab(const QString &value)
{
    tpTab = value;
}

QString tabellaTabvab::getCdtab() const
{
    return cdtab;
}

void tabellaTabvab::setCdtab(const QString &value)
{
    cdtab = value;
}

QString tabellaTabvab::getDescr() const
{
    return descr;
}

void tabellaTabvab::setDescr(const QString &value)
{
    descr = value;
}

void tabellaTabvab::resetOggetto()
{
    tpTab="";
    cdtab="";
    descr="";
    qyeryToDo="";
}

void tabellaTabvab::insertOggetto()
{
    if(tpTab=="") return;
    if(cdtab=="") return;
    if(descr=="") return;
    if(controlloCampi()) return;
    qyeryToDo="";
    qyeryToDo=QString("select  count(*) from tabvab where tipotabella='%1' and codicevab='%2'")
            .arg(tpTab)
            .arg(cdtab);
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
    qyeryToDo=QString("insert into tabvab (tipotabella, codicevab, fill1) "
                      "values ('%1','%2','%3')")
            .arg(tpTab)
            .arg(cdtab)
            .arg(descr);
    doQuery();
}

void tabellaTabvab::uploadOggetto()
{
    if(tpTab=="") return;
    if(cdtab=="") return;
    if(descr=="") return;
    if(controlloCampi()) return;
    qyeryToDo="";
    qyeryToDo=QString("update tabvab set fill1='%1' where tipotabella='%2' and codicevab='%3'")
            .arg(descr)
            .arg(tpTab)
            .arg(cdtab);
    doQuery();
}

bool tabellaTabvab::loadOggetto()
{
    if(tpTab=="") return false;
    if(cdtab=="") return false;
    qyeryToDo="";
    qyeryToDo=QString("select  * from tabvab where tipotabella='%1' and codicevab='%2'")
            .arg(tpTab)
            .arg(cdtab);
    doQuery();
    while (query->next())
    {
        tpTab=query->value(query->record().indexOf("tipotabella")).toString();
        cdtab=query->value(query->record().indexOf("codicevab")).toString();
        descr=query->value(query->record().indexOf("fill1")).toString();
        return true;
    }
    dialogInfoAlert tmpDialog;
    tmpDialog.init("Errore","Nessun riga trovata con la chiave",2);
    tmpDialog.exec();
    return false;
}

void tabellaTabvab::deleteOggetto()
{
    if(tpTab=="") return;
    if(cdtab=="") return;
    qyeryToDo="";
    qyeryToDo=QString("select count(*) from artico where espansione='%1' or tipologia='$2'")
            .arg(cdtab)
            .arg(cdtab);
    doQuery();
    if(query->next())
    {
        if(query->value(0).toInt()>=1)
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Chiave usata in altre tabelle",2);
            tmpDialog.exec();
            return;
        }
    }
    qyeryToDo=QString("delete from tabvab where tipotabella='%1' and codicevab='%2'")
            .arg(tpTab)
            .arg(cdtab);
    doQuery();
}

bool tabellaTabvab::controlloCampi()
{
    QString textErr="";
    bool isErr=false;
    if(!isErr && cdtab.length()>10)
    {
        isErr=true; textErr="Codice tabella troppo lungo";
    }
    else if(!isErr && descr.length()>50)
    {
        isErr=true; textErr="Descrizione troppa lunga";
    }
    if(isErr)
    {
        dialogInfoAlert tmpDialog;
        tmpDialog.init("Errore",textErr,2);
        tmpDialog.exec();
    }
    return isErr;
}

tabellaTabvab::tabellaTabvab(dataBaseConnection * m_dataBase):
    tabellaBase(m_dataBase)
{

}
