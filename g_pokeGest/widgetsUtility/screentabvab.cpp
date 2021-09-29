#include "screentabvab.h"
#include "ui_screentabvab.h"

#include "../widgetsUtility/dialoginfoalert.h"

#include <QMessageBox>

screenTabvab::screenTabvab(QWidget *parent, dataBaseConnection * db) :
    screenBase(parent,db),
    ui(new Ui::screenTabvab)
{
    ui->setupUi(this);
}

screenTabvab::~screenTabvab()
{
    delete ui;
}

void screenTabvab::init()
{
    /*richiamo init base*/
    screenBase::init();

    m_model=new QStandardItemModel();
    m_tableView=ui->tableView;
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setModel(m_model);
    connect(m_tableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(clickOnTableView(const QModelIndex &)));

    m_oggTabvab=new tabellaTabvab(m_dataBase);

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

    /*tasto connessione*/
    connect(ui->button_cambiaLogin, &QPushButton::clicked, this, [=](){
        m_dataBase->chiamaConnesione();
        ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));
    });
    ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));
}

/* reimplemento la funzione virtuale di crea menu*/
void screenTabvab::creaMenu()
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

    /*pressione elimina*/
    connect(m_actionElimina, &QAction::triggered, this, [=](){
        pressioneElimina();
    });

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
    m_toolBar->addAction(m_actionEsci);
}

void screenTabvab::impostaViewConStato()
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

void screenTabvab::setVisualizza()
{
    m_actionInserisciNuovo->setDisabled(false);
    m_actionConferma->setDisabled(true);
    m_actionModifica->setDisabled(false);
    m_actionEsci->setDisabled(false);
    m_actionLock->setDisabled(true);
    m_actionElimina->setDisabled(false);
    attivaDisattivaDettagli();
}

void screenTabvab::setModifica()
{
    m_actionInserisciNuovo->setDisabled(true);
    m_actionConferma->setDisabled(false);
    m_actionModifica->setDisabled(true);
    m_actionEsci->setDisabled(false);
    m_actionLock->setDisabled(false);
    m_actionElimina->setDisabled(true);
    attivaDisattivaDettagli();
}

void screenTabvab::setInserimento()
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

void screenTabvab::popolaModello()
{
    QString testoLine=ui->l_cercaCodice->text();
    m_model->clear();
    if(!m_tableView->isVisible())
        m_tableView->show();
    QString queryToEx=QString("select codicevab,fill1 from tabvab where tipotabella='%1' and (codicevab like '%%2%' or fill1 like '%%3%') order by codicevab")
            .arg(tipoTabella)
            .arg(testoLine)
            .arg(testoLine);
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    m_model->clear();
    m_model->setHorizontalHeaderLabels(QStringList() << "Codice" << "Descrizione");
    while(queryRes.next())
    {
        QList<QStandardItem*> ll;
        ll.push_back(new QStandardItem(queryRes.value(0).toString()));
        ll.push_back(new QStandardItem(queryRes.value(1).toString()));
        ll[0]->setEditable(false);
        ll[1]->setEditable(false);
        m_model->appendRow(ll);
    }
    m_tableView->resizeColumnsToContents();
}

void screenTabvab::pressioneConferma()
{
    if(statoView=="inserimento")
    {
        if(controlloCampiPrimaDiInsUp()) return;

        m_oggTabvab->resetOggetto();
        m_oggTabvab->setTpTab(tipoTabella);
        m_oggTabvab->setCdtab(ui->l_insertCodice->text());
        m_oggTabvab->setDescr(ui->l_insertDescrizione->text());
        m_oggTabvab->insertOggetto();

        resetViewDettaglio();
        loadArticoloInView();
        popolaModello();
    }
    else if(statoView=="modifica")
    {
        if(m_oggTabvab->getCdtab()=="")
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Sei nello stato di modifica senza oggetto attivo",2);
            tmpDialog.exec();
            return;
        }

        if(controlloCampiPrimaDiInsUp()) return;

        m_oggTabvab->resetOggetto();
        m_oggTabvab->setTpTab(tipoTabella);
        m_oggTabvab->setCdtab(ui->l_insertCodice->text());
        m_oggTabvab->setDescr(ui->l_insertDescrizione->text());
        m_oggTabvab->uploadOggetto();

        resetViewDettaglio();
        loadArticoloInView();
        popolaModello();
    }
}

bool screenTabvab::controlloCampiPrimaDiInsUp()
{
    QString err;
    bool isErr=false;
    if(!isErr && (ui->l_insertCodice->text()==""))
    {
        err="Codice vuoto";
        isErr=true;
    }
    else if(!isErr && (ui->l_insertDescrizione->text().length()>=50))
    {
            err="Descrizione troppo lunga";
            isErr=true;
    }
    else if(!isErr && (ui->l_insertDescrizione->text()==""))
    {
            err="Descrizione vuota";
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

void screenTabvab::resetViewDettaglio()
{
    ui->l_insertCodice->setText("");
    ui->l_insertDescrizione->setText("");
}

void screenTabvab::loadArticoloInView()
{
    ui->l_insertCodice->setText(m_oggTabvab->getCdtab());
    ui->l_insertDescrizione->setText(m_oggTabvab->getDescr());
}

void screenTabvab::attivaDisattivaDettagli()
{
    if(statoView=="modifica")
    {
        ui->l_insertCodice->setEnabled(false);
        ui->l_insertDescrizione->setEnabled(true);
        m_tableView->setEnabled(false);
    }
    else if (statoView=="inserimento")
    {
        ui->l_insertCodice->setEnabled(true);
        ui->l_insertDescrizione->setEnabled(true);
        m_tableView->setEnabled(false);
    }
    else if (statoView=="visualizza")
    {
        ui->l_insertCodice->setEnabled(false);
        ui->l_insertDescrizione->setEnabled(false);
        m_tableView->setEnabled(true);
    }
}

void screenTabvab::clickOnTableView(const QModelIndex & index)
{
    if(index.isValid())
    {
        m_oggTabvab->resetOggetto();
        m_oggTabvab->setTpTab(tipoTabella);
        m_oggTabvab->setCdtab(m_model->index(index.row(),0).data().toString());
        m_oggTabvab->setDescr(m_model->index(index.row(),1).data().toString());
        m_oggTabvab->loadOggetto();
        resetViewDettaglio();
        loadArticoloInView();
    }
}

QString screenTabvab::getTipoTabella() const
{
    return tipoTabella;
}

void screenTabvab::setTipoTabella(const QString &value)
{
    tipoTabella = value;
}

void screenTabvab::pressioneElimina()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Richiesta conferma", "Vuoi eliminare l'articolo?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_oggTabvab->deleteOggetto();
        m_oggTabvab->resetOggetto();
        resetViewDettaglio();
        popolaModello();
    } else {
        qDebug() << "Yes was *not* clicked";
    }
}
