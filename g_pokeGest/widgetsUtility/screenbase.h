#ifndef SCREENBASE_H
#define SCREENBASE_H

#include <QWidget>
#include <QToolBar>
#include <QSqlDatabase>
#include "../backEndUtility/databaseconnection.h"
#include <QDebug>

class screenBase : public QWidget
{
    Q_OBJECT
public:
    explicit screenBase(QWidget *parent = nullptr, dataBaseConnection * m_dataBase=nullptr);

    /** chiamare la init anche nella classe derivata*/
    virtual void init();

    /** crea il menu nella toolbar, attenzione la toolbar non è inserità*/
    virtual void creaMenu();

    /** funzione non definita ma da implementare la action del menù la chiamano */
    virtual void impostaViewConStato();

    QString statoView="visualizza";

    QAction *actionConferma() const;

    QAction *actionElimina() const;

    QAction *actionEsci() const;

    QString getStatoView() const;
    void setStatoView(const QString &value);

public:
    virtual QString getDescrizioneArticoloDaCodice(QString codiceArt);

protected:
    /** oggeto che contiene dati di connesione al db
     * possibilità di cambiare utente*/
    dataBaseConnection * m_dataBase;

    QToolBar * m_toolBar;
    QAction * m_actionInserisciNuovo;
    QAction * m_actionConferma;
    QAction * m_actionModifica;
    QAction * m_actionEsci;
    QAction * m_actionSinistra;
    QAction * m_actionDestra;
    QAction * m_actionLock;
    QAction * m_actionElimina;
};

#endif // SCREENBASE_H
