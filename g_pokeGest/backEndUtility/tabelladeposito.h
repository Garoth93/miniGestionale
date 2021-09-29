#ifndef TABELLADEPOSITO_H
#define TABELLADEPOSITO_H

#include "tabellabase.h"

class tabellaDeposito: public tabellaBase
{
protected:
    QString codiceArticolo;
    double qta;
    QString posizione;
public:
    tabellaDeposito(dataBaseConnection * m_dataBase=nullptr);
    QString getCodiceArticolo() const;
    void setCodiceArticolo(const QString &value);
    double getQta() const;
    void setQta(double value);
    QString getPosizione() const;
    void setPosizione(const QString &value);

    virtual void resetOggetto();
    virtual void insertOggetto();
    virtual void uploadOggetto();
    virtual bool loadOggetto();
    virtual void deleteOggetto();
    virtual bool controlloCampi();
};

#endif // TABELLADEPOSITO_H
