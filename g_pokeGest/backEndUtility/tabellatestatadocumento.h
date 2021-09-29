#ifndef TABELLATESTATADOCUMENTO_H
#define TABELLATESTATADOCUMENTO_H

#include "tabellabase.h"
#include <QDate>

class tabellaTestataDocumento: public tabellaBase
{
protected:
    QString note;
    int idTestata;
    QDate dataTesDoc;
public:
    tabellaTestataDocumento(dataBaseConnection * m_dataBase=nullptr);

    virtual void resetOggetto();
    virtual bool insertOggetto();
    virtual bool uploadOggetto();
    virtual bool loadOggetto();
    virtual void deleteOggetto();

    QString getNote() const;
    void setNote(const QString &value);
    int getIdTestata() const;
    void setIdTestata(int value);
    QDate getDataTesDoc() const;
    void setDataTesDoc(const QDate &value);
};

#endif // TABELLATESTATADOCUMENTO_H
