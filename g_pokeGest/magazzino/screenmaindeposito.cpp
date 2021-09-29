#include "screenmaindeposito.h"
#include "ui_screenmaindeposito.h"

#include "../widgetsUtility/dialoginfoalert.h"
#include "../backEndUtility/tabellaarticoli.h"
#include "../widgetsUtility/dialogcercaarticolo.h"

#include <QMessageBox>

screenMainDeposito::screenMainDeposito(QWidget *parent, dataBaseConnection * db) :
    screenBase(parent,db),
    ui(new Ui::screenMainDeposito)
{
    ui->setupUi(this);
    this->setWindowTitle("Gestione deposito");
}

screenMainDeposito::~screenMainDeposito()
{
    delete ui;
}

void screenMainDeposito::init()
{
    /*richiamo init base*/
    screenBase::init();

    m_model=new QStandardItemModel();
    m_tableView=ui->tableView;
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setModel(m_model);
    connect(m_tableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(clickOnTableView(const QModelIndex &)));

    m_oggMagazzino=new tabellaDeposito(m_dataBase);

    /*imposta toolbar alla ui creata nella init base*/
    ui->layoyt_toolbar->addWidget(m_toolBar);

    impostaViewConStato();

    /*bottone cerca*/
    ui->tb_cerca->setIcon(QIcon((":/cerca.png")));
    connect(ui->tb_cerca, &QToolButton::clicked, this, [=](){
        popolaModello();
    });

    /*pressione conferma*/
    connect(m_actionConferma, &QAction::triggered, this, [=](){
        pressioneConferma();
    });

    /*bottone avvia dialog cerca articolo*/
    ui->tb_attivaCercaArticolo->setIcon(QIcon((":/up.png")));
    connect(ui->tb_attivaCercaArticolo, &QToolButton::clicked, this, [=](){
        QString toSearch="";
        dialogCercaArticolo cercaDialog(this,m_dataBase,&toSearch);
        cercaDialog.exec();
        if(toSearch=="") return;
        ui->l_insertCodice->setText(toSearch);
    });

    /*tasto connessione*/
    connect(ui->button_cambiaLogin, &QPushButton::clicked, this, [=](){
        m_dataBase->chiamaConnesione();
        ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));
    });
    ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));
}

void screenMainDeposito::creaMenu()
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

    /*pressione elimina*/
    connect(m_actionElimina, &QAction::triggered, this, [=](){
        pressioneElimina();
    });

    m_toolBar->addAction(m_actionInserisciNuovo);
    m_toolBar->insertAction(m_actionConferma,m_actionInserisciNuovo);
    m_toolBar->insertAction(m_actionModifica,m_actionConferma);
    m_toolBar->insertAction(m_actionElimina,m_actionModifica);
    m_toolBar->insertAction(m_actionLock,m_actionElimina);
    m_toolBar->insertAction(m_toolBar->addSeparator(),m_actionLock);
    m_toolBar->addAction(m_actionEsci);
}

void screenMainDeposito::impostaViewConStato()
{
    if(statoView=="visualizza")
    {
        setVisualizza();
    }
    else if(statoView=="modifica")
    {
        setModifica();
    }
    else if(statoView=="inserimento")
    {
        setInserimento();
    }
}

void screenMainDeposito::setVisualizza()
{
    m_actionInserisciNuovo->setDisabled(false);
    m_actionConferma->setDisabled(true);
    m_actionModifica->setDisabled(false);
    m_actionEsci->setDisabled(false);
    m_actionLock->setDisabled(true);
    m_actionElimina->setDisabled(false);
    attivaDisattivaDettagli();
}

void screenMainDeposito::setModifica()
{
    m_actionInserisciNuovo->setDisabled(true);
    m_actionConferma->setDisabled(false);
    m_actionModifica->setDisabled(true);
    m_actionEsci->setDisabled(false);
    m_actionLock->setDisabled(false);
    m_actionElimina->setDisabled(true);
    attivaDisattivaDettagli();
}

void screenMainDeposito::setInserimento()
{
    resetViewDettaglio();
    m_actionInserisciNuovo->setDisabled(true);
    m_actionConferma->setDisabled(false);
    m_actionModifica->setDisabled(true);
    m_actionEsci->setDisabled(false);
    m_actionLock->setDisabled(false);
    m_actionElimina->setDisabled(true);
    attivaDisattivaDettagli();
}

void screenMainDeposito::attivaDisattivaDettagli()
{
    if(statoView=="modifica")
    {
        ui->l_insertCodice->setEnabled(false);
        ui->tb_attivaCercaArticolo->setEnabled(false);
        ui->l_insertPosizione->setEnabled(true);
        ui->spinBox_qta->setEnabled(true);
        m_tableView->setEnabled(false);
    }
    else if (statoView=="inserimento")
    {
        ui->l_insertCodice->setEnabled(true);
        ui->tb_attivaCercaArticolo->setEnabled(true);
        ui->l_insertPosizione->setEnabled(true);
        ui->spinBox_qta->setEnabled(true);
        m_tableView->setEnabled(false);
    }
    else if (statoView=="visualizza")
    {
        ui->l_insertCodice->setEnabled(false);
        ui->tb_attivaCercaArticolo->setEnabled(false);
        ui->l_insertPosizione->setEnabled(false);
        ui->spinBox_qta->setEnabled(false);
        m_tableView->setEnabled(true);
    }
}

void screenMainDeposito::resetViewDettaglio()
{
    ui->l_insertCodice->setText("");
    ui->l_insertPosizione->setText("");
    ui->spinBox_qta->setValue(0);
}

void screenMainDeposito::pressioneConferma()
{
    if(statoView=="inserimento")
    {
        if(controlloCampiPrimaDiInsUp()) return;

        m_oggMagazzino->resetOggetto();
        m_oggMagazzino->setCodiceArticolo(ui->l_insertCodice->text());
        m_oggMagazzino->setQta(ui->spinBox_qta->value());
        m_oggMagazzino->setPosizione(ui->l_insertPosizione->text());
        m_oggMagazzino->insertOggetto();

        resetViewDettaglio();
        loadArticoloInView();
        popolaModello();
    }
    else if(statoView=="modifica")
    {
        if(m_oggMagazzino->getCodiceArticolo()=="")
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Sei nello stato di modifica senza oggetto attivo",2);
            tmpDialog.exec();
            return;
        }

        if(controlloCampiPrimaDiInsUp()) return;

        m_oggMagazzino->resetOggetto();
        m_oggMagazzino->setCodiceArticolo(ui->l_insertCodice->text());
        m_oggMagazzino->setQta(ui->spinBox_qta->value());
        m_oggMagazzino->setPosizione(ui->l_insertPosizione->text());
        m_oggMagazzino->uploadOggetto();

        resetViewDettaglio();
        loadArticoloInView();
        popolaModello();
    }
}

bool screenMainDeposito::controlloCampiPrimaDiInsUp()
{
    QString err;
    bool isErr=false;
    if(!isErr && (ui->l_insertCodice->text()==""))
    {
        err="Codice vuoto";
        isErr=true;
    }
    else if(!isErr && (ui->l_insertCodice->text().length()>=50))
    {
            err="Codice articolo troppo lunga";
            isErr=true;
    }
    else if(!isErr && (ui->l_insertPosizione->text().length()>=50))
    {
            err="Posizione troppo lunga";
            isErr=true;
    }
    if(isErr)
    {
        dialogInfoAlert tmpDialog;
        tmpDialog.init("Errore",err,2);
        tmpDialog.exec();
    }
    return isErr;
}

void screenMainDeposito::loadArticoloInView()
{
    ui->l_insertCodice->setText(m_oggMagazzino->getCodiceArticolo());
    ui->l_insertPosizione->setText(m_oggMagazzino->getPosizione());
    ui->spinBox_qta->setValue(m_oggMagazzino->getQta());
}

void screenMainDeposito::popolaModello()
{
    QString testoLine=ui->l_cercaCodice->text();
    m_model->clear();
    if(!m_tableView->isVisible())
        m_tableView->show();
    QString queryToEx=QString("select cdart,qta,posizione from deposito where (cdart like '%%1%' or posizione like '%%2%') order by cdart")
            .arg(testoLine)
            .arg(testoLine);
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    m_model->clear();
    m_model->setHorizontalHeaderLabels(QStringList() << "Codice" << "Quantità" << "Descrizione" << "Deposito");
    while(queryRes.next())
    {
        QList<QStandardItem*> ll;
        ll.push_back(new QStandardItem(queryRes.value(0).toString()));
        ll.push_back(new QStandardItem(queryRes.value(1).toString()));
        ll.push_back(new QStandardItem(getDescrizioneArticoloDaCodice(queryRes.value(0).toString())));
        ll.push_back(new QStandardItem(queryRes.value(2).toString()));
        ll[0]->setEditable(false);
        ll[1]->setEditable(false);
        ll[2]->setEditable(false);
        ll[3]->setEditable(false);
        m_model->appendRow(ll);
    }
    m_tableView->resizeColumnsToContents();
}

void screenMainDeposito::pressioneElimina()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Richiesta conferma", "Vuoi eliminare l'articolo?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_oggMagazzino->deleteOggetto();
        m_oggMagazzino->resetOggetto();
        resetViewDettaglio();
        popolaModello();
    } else {
        qDebug() << "Yes was *not* clicked";
    }
}

QString screenMainDeposito::getDescrizioneArticoloDaCodice(QString codiceArt)
{
    tabellaArticoli * tmpOgg=new tabellaArticoli(m_dataBase);
    tmpOgg->setCodiceArticolo(codiceArt);
    tmpOgg->loadOggetto();
    QString toRet="";
    toRet=tmpOgg->getDescrizione();
    delete  tmpOgg;
    return toRet;
}

void screenMainDeposito::clickOnTableView(const QModelIndex & index)
{
    if(index.isValid())
    {
        m_oggMagazzino->resetOggetto();
        m_oggMagazzino->setCodiceArticolo(m_model->index(index.row(),0).data().toString());
        m_oggMagazzino->loadOggetto();
        resetViewDettaglio();
        loadArticoloInView();
    }
}
