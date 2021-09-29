#ifndef TABELLARIGADOCUMENTO_H
#define TABELLARIGADOCUMENTO_H

#include "tabellabase.h"
#include <QDate>

class tabellarigadocumento: public tabellaBase
{
protected:
    QString note;
    int idTestata;
    double prezzo;
    QString tipoDoc;
    QString codiceArticolo;
    int numeroRiga;

public:
    tabellarigadocumento(dataBaseConnection * m_dataBase=nullptr);

    QString getNote() const;
    void setNote(const QString &value);
    int getIdTestata() const;
    void setIdTestata(int value);
    double getPrezzo() const;
    void setPrezzo(double value);
    QString getTipoDoc() const;
    void setTipoDoc(const QString &value);
    QString getCodiceArticolo() const;
    void setCodiceArticolo(const QString &value);
    int getNumeroRiga() const;
    void setNumeroRiga(int value);

    virtual void resetOggetto();
    virtual void insertOggetto();
    virtual bool loadOggetto();
    virtual void deleteOggetto();
};

#endif // TABELLARIGADOCUMENTO_H
