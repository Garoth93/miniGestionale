#ifndef TABELLAARTICOLI_H
#define TABELLAARTICOLI_H

#include "tabellabase.h"
#include <QDate>

class tabellaArticoli: public tabellaBase
{
protected:
    QString codiceArticolo;
    QString descrizione;
    QString espansione;
    QString rarezza;
    QString lingua;
    QString tipologia;
    QDate dataEspansione;
    QDate data1;
    QString fill1;
public:
    tabellaArticoli(dataBaseConnection * m_dataBase=nullptr);
    virtual void resetOggetto();
    virtual void insertOggetto();
    virtual void uploadOggetto();
    virtual bool loadOggetto();
    virtual void loadOggettoConQuery(QString qry);
    virtual void deleteOggetto();
    QString getCodiceArticolo() const;
    void setCodiceArticolo(const QString &value);
    QString getDescrizione() const;
    void setDescrizione(const QString &value);
    QString getEspansione() const;
    void setEspansione(const QString &value);
    QString getRarezza() const;
    void setRarezza(const QString &value);
    QString getLingua() const;
    void setLingua(const QString &value);
    QString getTipologia() const;
    void setTipologia(const QString &value);
    QDate getDataEspansione() const;
    void setDataEspansione(const QDate &value);
    QDate getData1() const;
    void setData1(const QDate &value);
    QString getFill1() const;
    void setFill1(const QString &value);
};

#endif // TABELLAARTICOLI_H
