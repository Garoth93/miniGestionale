#include "screenbase.h"
#include <QAction>
#include "../backEndUtility/tabellaarticoli.h"

screenBase::screenBase(QWidget *parent,dataBaseConnection * db) :
    QWidget(parent),
    m_dataBase(db)
{
    setStyleSheet("QLineEdit:focus { "
                  "border: 2px solid rgb(66,165,245); padding: 0 5px; border-radius: 5px;"
                  "}"
                  "QPushButton {"
                  "border: 2px solid gray; border-radius: 5px; padding: 0 8px; background: rgb(208, 214, 211);"
                  "}"
                  "QPushButton:hover {"
                  "border: 2px solid gray; border-radius: 5px; padding: 0 8px; background: rgb(194, 207, 201);"
                  "}"
                  "QPushButton:pressed { "
                  "border: 2px solid gray; border-radius: 5px; padding: 0 8px; background: rgb(179, 186, 183);"
                  "}"
                  );
}

void screenBase::init()
{
    m_actionInserisciNuovo=nullptr;
    m_toolBar=new QToolBar();
    creaMenu();
}

void screenBase::creaMenu()
{
    m_actionInserisciNuovo = new QAction("Inserisci nuovo",this);
    m_actionInserisciNuovo->setIcon(QIcon(":/aggiungi.png"));
    connect(m_actionInserisciNuovo, &QAction::triggered, this, [=](){
        statoView="inserimento";
        impostaViewConStato();
    });

    m_actionConferma = new QAction("Conferma",this);
    m_actionConferma->setIcon(QIcon(":/conferma.png"));

    m_actionModifica = new QAction("Modifica",this);
    m_actionModifica->setIcon(QIcon(":/opzioni.png"));
    connect(m_actionModifica, &QAction::triggered, this, [=](){
        statoView="modifica";
        impostaViewConStato();
    });

    m_actionElimina = new QAction("Elimina",this);
    m_actionElimina->setIcon(QIcon(":/elimina.png"));

    m_actionEsci = new QAction("Esci",this);
    m_actionEsci->setIcon(QIcon(":/esci.png"));
    connect(m_actionEsci, &QAction::triggered, this, [=](){
        this->close();
    });

    m_actionSinistra = new QAction("",this);
    m_actionSinistra->setIcon(QIcon(":/left.png"));

    m_actionDestra = new QAction("",this);
    m_actionDestra->setIcon(QIcon(":/right.png"));

    m_actionLock = new QAction("Lock",this);
    m_actionLock->setIcon(QIcon(":/lock.png"));
    connect(m_actionLock, &QAction::triggered, this, [=](){
        statoView="visualizza";
        impostaViewConStato();
    });

    m_toolBar->addAction(m_actionInserisciNuovo);
    m_toolBar->insertAction(m_actionConferma,m_actionInserisciNuovo);
    m_toolBar->insertAction(m_actionModifica,m_actionConferma);
    m_toolBar->insertAction(m_actionElimina,m_actionModifica);
    m_toolBar->insertAction(m_actionLock,m_actionElimina);
    m_toolBar->insertAction(m_toolBar->addSeparator(),m_actionLock);
    m_toolBar->addAction(m_actionSinistra);
    m_toolBar->addAction(m_actionDestra);
    m_toolBar->insertAction(m_toolBar->addSeparator(),m_actionDestra);
    m_toolBar->addAction(m_actionEsci);
}

void screenBase::impostaViewConStato()
{

}

QString screenBase::getStatoView() const
{
    return statoView;
}

void screenBase::setStatoView(const QString &value)
{
    statoView = value;
}

QString screenBase::getDescrizioneArticoloDaCodice(QString codiceArt)
{
    tabellaArticoli * tmpOgg=new tabellaArticoli(m_dataBase);
    tmpOgg->setCodiceArticolo(codiceArt);
    tmpOgg->loadOggetto();
    QString toRet="";
    toRet=tmpOgg->getDescrizione();
    delete  tmpOgg;
    return toRet;
}

QAction *screenBase::actionConferma() const
{
    return m_actionConferma;
}

QAction *screenBase::actionElimina() const
{
    return m_actionElimina;
}

QAction *screenBase::actionEsci() const
{
    return m_actionEsci;
}
