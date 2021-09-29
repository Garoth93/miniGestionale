#ifndef TABELLATABVAB_H
#define TABELLATABVAB_H

#include "tabellabase.h"

class tabellaTabvab: public tabellaBase
{
protected:
    QString tpTab;
    QString cdtab;
    QString descr;
public:
    tabellaTabvab(dataBaseConnection * m_dataBase=nullptr);
    QString getTpTab() const;
    void setTpTab(const QString &value);
    QString getCdtab() const;
    void setCdtab(const QString &value);
    QString getDescr() const;
    void setDescr(const QString &value);

    virtual void resetOggetto();
    virtual void insertOggetto();
    virtual void uploadOggetto();
    virtual bool loadOggetto();
    virtual void deleteOggetto();
    virtual bool controlloCampi();

};

#endif // TABELLATABVAB_H
