#include "screenmainartica.h"
#include "ui_screenmainartica.h"

#include <QVBoxLayout>
#include "../widgetsUtility/dialoginfoalert.h"
#include "../widgetsUtility/dialogcercaarticolo.h"
#include <QMessageBox>
#include <QSqlRecord>

screenMainArtica::screenMainArtica(QWidget *parent,dataBaseConnection * db) :
    screenBase(parent,db),
    ui(new Ui::screenMainArtica)
{
    ui->setupUi(this);
}

screenMainArtica::~screenMainArtica()
{
    delete ui;
}

void screenMainArtica::init()
{
    this->setWindowTitle("Anagrafica articoli");

    ui->comboCercaTipo->setMinimumWidth(150);
    ui->comboCercaEspansione->setMinimumWidth(150);
    ui->comboInsertTipo->setMinimumWidth(150);
    ui->comboInsertEspansione->setMinimumWidth(150);

    /*richiamo init base*/
    screenBase::init();

    /*oggeto articolo principale lo creo*/
    m_oggArt=new tabellaArticoli(m_dataBase);

    /*imposta toolbar alla ui creata nella init base*/
    ui->layoyt_toolbar->addWidget(m_toolBar);

    /*label utente loggato*/
    impostaTitoloLabelDb();

    /*in base allo stato con cui entro imposta la view*/
    impostaViewConStato();
    tabWidgetCambio(ui->tabWidget->currentIndex());

    /*cambio tab widget*/
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidgetCambio(int)));

    /*bottone cerca*/
    ui->tb_cerca->setIcon(QIcon((":/cerca.png")));
    connect(ui->tb_cerca, &QToolButton::clicked, this, [=](){
        /*se tab 2 cerco solo con codice articolo*/
        if(ui->tabWidget->currentIndex()==1)
        {
            if(ui->l_cercaCodiceArticolo->text()=="")
                return;
            m_oggArt->resetOggetto();
            m_oggArt->setCodiceArticolo(ui->l_cercaCodiceArticolo->text());
            if(!m_oggArt->loadOggetto())
            {
                resetViewDettaglio();
                return;
            }
            resetViewDettaglio();
            loadArticoloInView();
            ricaricaModelloTab1();
        }
        if(ui->tabWidget->currentIndex()==0)
        {
            ricaricaModelloTab1();
        }
    });

    /*bottone avvia dialog cerca articolo*/
    ui->tb_attivaCercaArticolo->setIcon(QIcon((":/up.png")));
    connect(ui->tb_attivaCercaArticolo, &QToolButton::clicked, this, [=](){
        QString toSearch="";
        dialogCercaArticolo cercaDialog(this,m_dataBase,&toSearch);
        cercaDialog.exec();
        if(toSearch=="") return;
        m_oggArt->resetOggetto();
        m_oggArt->setCodiceArticolo(toSearch);
        if(!m_oggArt->loadOggetto())
        {
            resetViewDettaglio();
            return;
        }
        resetViewDettaglio();
        loadArticoloInView();
        ui->tabWidget->setCurrentIndex(1);
    });

    /*bottone mod tipo*/
    screenGestioneTabVab = new screenTabvab(nullptr,m_dataBase);
    screenGestioneTabVab->init();
    ui->tb_modTipo->setIcon(QIcon((":/opzioni.png")));
    connect(ui->tb_modTipo, &QToolButton::clicked, this, [=](){
        screenGestioneTabVab->setTipoTabella("tipoOgg");
        screenGestioneTabVab->popolaModello();
        screenGestioneTabVab->resetViewDettaglio();
        screenGestioneTabVab->setWindowTitle("Gestione tipo");
        screenGestioneTabVab->show();
    });

    /*bottone mod espansione*/
    ui->tb_modEspansione->setIcon(QIcon((":/opzioni.png")));
    connect(ui->tb_modEspansione, &QToolButton::clicked, this, [=](){
        screenGestioneTabVab->setTipoTabella("tipoEspa");
        screenGestioneTabVab->popolaModello();
        screenGestioneTabVab->resetViewDettaglio();
        screenGestioneTabVab->setWindowTitle("Gestione espansioni");
        screenGestioneTabVab->show();
    });

    /*quando premo conferma ripolo queste*/
    connect(screenGestioneTabVab->actionConferma(), &QAction::triggered, this, [=](){
        popolaComboBox();
        resetViewDettaglio();
        loadArticoloInView();
    });

    /*quando premo conferma ripolo queste*/
    connect(screenGestioneTabVab->actionElimina(), &QAction::triggered, this, [=](){
        popolaComboBox();
        resetViewDettaglio();
        loadArticoloInView();
    });

    /*connect quando le combo cambiano*/
    connect(ui->comboInsertTipo, &QComboBox::currentTextChanged, this, [=](){
        testoComboCambiato("comboInsertTipo");
    });
    connect(ui->comboCercaTipo, &QComboBox::currentTextChanged, this, [=](){
        testoComboCambiato("comboCercaTipo");
    });
    connect(ui->comboInsertEspansione, &QComboBox::currentTextChanged, this, [=](){
        testoComboCambiato("comboInsertEspa");
    });
    connect(ui->comboCercaEspansione, &QComboBox::currentTextChanged, this, [=](){
        testoComboCambiato("comboCercaEspa");
    });

    /*click sul widget calendario*/
    connect(ui->calendarWidgetDataIns, &QCalendarWidget::clicked, this, [=](){
        ui->l_InsertData->setText(ui->calendarWidgetDataIns->selectedDate().toString("dd/MM/yy"));
    });

    /*riempio combo*/
    popolaComboBox();

    /*pressione conferma*/
    connect(m_actionConferma, &QAction::triggered, this, [=](){
        pressioneConferma();
    });

    /*pressione elimina*/
    connect(m_actionElimina, &QAction::triggered, this, [=](){
        pressioneElimina();
    });

    /*parte modello tab1*/
    m_viewTab1=ui->tableViewPanoramica;
    m_viewTab1->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_modelTab1=nullptr;
    m_sortTab1=nullptr;

    /*intestazione modello*/
    creaModello();
    popolaModello();

    /*connect tableView*/
    connect(m_viewTab1, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doppioTableViewTab1(const QModelIndex &)));

    connect(m_viewTab1, &QTableView::clicked, this, [=](){
        if(m_viewTab1->currentIndex().isValid())
            m_rigaDoveSonoViewTab1=m_viewTab1->currentIndex().row();
    });

    /*pressione m_actionSinistra*/
    connect(m_actionSinistra, &QAction::triggered, this, [=](){
        clickSx();
    });
    /*pressione m_actionDestra*/
    connect(m_actionDestra, &QAction::triggered, this, [=](){
        clickDx();
    });

    /*tasto connessione*/
    connect(ui->button_cambiaLogin, &QPushButton::clicked, this, [=](){
        m_dataBase->chiamaConnesione();
        impostaTitoloLabelDb();
    });
}

void screenMainArtica::impostaTitoloLabelDb()
{
    ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));
}

void screenMainArtica::impostaViewConStato()
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

/*reset campi ui dettaglio*/
void screenMainArtica::resetViewDettaglio()
{
    ui->l_InsertCodiceArticolo->setText("");
    ui->l_InsertDescrizioneArticolo->setText("");
    ui->comboInsertTipo->setCurrentText("");
    ui->comboInsertEspansione->setCurrentText("");
    ui->l_InsertRarezza->setText("");
    ui->l_InsertLingua->setText("");
    ui->l_InsertData->setText(QDate::currentDate().toString("dd/MM/yy"));
    ui->calendarWidgetDataIns->setSelectedDate(QDate::currentDate());
}

void screenMainArtica::setVisualizza()
{
    m_actionInserisciNuovo->setDisabled(false);
    m_actionConferma->setDisabled(true);
    m_actionModifica->setDisabled(false);
    m_actionEsci->setDisabled(false);
    m_actionSinistra->setDisabled(false);
    m_actionDestra->setDisabled(false);
    m_actionLock->setDisabled(true);
    m_actionElimina->setDisabled(false);
    attivaDisattivaDettagli();
}

void screenMainArtica::setModifica()
{
    m_actionInserisciNuovo->setDisabled(true);
    m_actionConferma->setDisabled(false);
    m_actionModifica->setDisabled(true);
    m_actionEsci->setDisabled(false);
    m_actionSinistra->setDisabled(true);
    m_actionDestra->setDisabled(true);
    m_actionLock->setDisabled(false);
    m_actionElimina->setDisabled(true);
    attivaDisattivaDettagli();
}

void screenMainArtica::setInserimento()
{
    ui->tabWidget->setCurrentIndex(1);
    m_oggArt->resetOggetto();
    resetViewDettaglio();
    m_actionInserisciNuovo->setDisabled(true);
    m_actionConferma->setDisabled(false);
    m_actionModifica->setDisabled(true);
    m_actionEsci->setDisabled(false);
    m_actionSinistra->setDisabled(true);
    m_actionDestra->setDisabled(true);
    m_actionLock->setDisabled(false);
    m_actionElimina->setDisabled(true);
    attivaDisattivaDettagli();
}

void screenMainArtica::pressioneConferma()
{
    if(statoView=="inserimento")
    {
        if(controlloCampiPrimaDiInsUp()) return;

        m_oggArt->resetOggetto();
        m_oggArt->setCodiceArticolo(ui->l_InsertCodiceArticolo->text());
        m_oggArt->setDescrizione(ui->l_InsertDescrizioneArticolo->text());
        m_oggArt->setEspansione(ui->comboInsertEspansione->currentText());
        m_oggArt->setTipologia(ui->comboInsertTipo->currentText());
        m_oggArt->setLingua(ui->l_InsertLingua->text());
        m_oggArt->setRarezza(ui->l_InsertRarezza->text());
        m_oggArt->setData1(ui->calendarWidgetDataIns->selectedDate());
        m_oggArt->setDataEspansione(QDate::currentDate());

        m_oggArt->insertOggetto();

        resetViewDettaglio();
        loadArticoloInView();
        ricaricaModelloTab1();
    }
    else if(statoView=="modifica")
    {
        if(m_oggArt->getCodiceArticolo()=="")
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Sei nello stato di modifica senza articolo attivo",2);
            tmpDialog.exec();
            return;
        }

        if(controlloCampiPrimaDiInsUp()) return;

        m_oggArt->resetOggetto();
        m_oggArt->setCodiceArticolo(ui->l_InsertCodiceArticolo->text());
        m_oggArt->setDescrizione(ui->l_InsertDescrizioneArticolo->text());
        m_oggArt->setEspansione(ui->comboInsertEspansione->currentText());
        m_oggArt->setTipologia(ui->comboInsertTipo->currentText());
        m_oggArt->setLingua(ui->l_InsertLingua->text());
        m_oggArt->setRarezza(ui->l_InsertRarezza->text());
        m_oggArt->setData1(ui->calendarWidgetDataIns->selectedDate());
        m_oggArt->setDataEspansione(QDate::currentDate());

        m_oggArt->uploadOggetto();

        resetViewDettaglio();
        loadArticoloInView();
        ricaricaModelloTab1();
    }
}

void screenMainArtica::attivaDisattivaDettagli()
{
    ui->l_insertEspansione->setEnabled(false);
    ui->l_insertTipo->setEnabled(false);
    ui->l_InsertCodiceArticolo->setEnabled(false);
    ui->l_InsertData->setEnabled(false);
    if(statoView=="modifica")
    {
        ui->l_InsertCodiceArticolo->setEnabled(false);
        ui->l_InsertDescrizioneArticolo->setEnabled(true);
        ui->comboInsertTipo->setEnabled(true);
        ui->comboInsertEspansione->setEnabled(true);
        ui->l_InsertRarezza->setEnabled(true);
        ui->l_InsertLingua->setEnabled(true);
        ui->calendarWidgetDataIns->setEnabled(true);
    }
    else if (statoView=="inserimento")
    {
        ui->l_InsertCodiceArticolo->setEnabled(true);
        ui->l_InsertDescrizioneArticolo->setEnabled(true);
        ui->comboInsertTipo->setEnabled(true);
        ui->comboInsertEspansione->setEnabled(true);
        ui->l_InsertRarezza->setEnabled(true);
        ui->l_InsertLingua->setEnabled(true);
        ui->calendarWidgetDataIns->setEnabled(true);
    }
    else if (statoView=="visualizza")
    {
        ui->l_InsertCodiceArticolo->setEnabled(false);
        ui->l_InsertDescrizioneArticolo->setEnabled(false);
        ui->comboInsertTipo->setEnabled(false);
        ui->comboInsertEspansione->setEnabled(false);
        ui->l_InsertRarezza->setEnabled(false);
        ui->l_InsertLingua->setEnabled(false);
        ui->calendarWidgetDataIns->setEnabled(false);
    }
}

void screenMainArtica::loadArticoloInView()
{
    ui->l_InsertCodiceArticolo->setText(m_oggArt->getCodiceArticolo());
    ui->l_InsertDescrizioneArticolo->setText(m_oggArt->getDescrizione());
    ui->l_InsertRarezza->setText(m_oggArt->getRarezza());
    ui->l_InsertLingua->setText(m_oggArt->getLingua());
    ui->l_InsertData->setText(m_oggArt->getData1().toString("dd/MM/yy"));
    ui->calendarWidgetDataIns->setSelectedDate(m_oggArt->getData1());
    ui->comboInsertEspansione->setCurrentText(m_oggArt->getEspansione());
    ui->comboInsertTipo->setCurrentText(m_oggArt->getTipologia());
}

bool screenMainArtica::controlloCampiPrimaDiInsUp()
{
    QString err;
    bool isErr=false;
    if(!isErr && (ui->l_InsertCodiceArticolo->text()==""))
    {
        err="Codice articolo vuoto";
        isErr=true;
    }
    else if(!isErr && (ui->l_InsertCodiceArticolo->text().length()>=50))
    {
            err="Codice articolo troppo lungo";
            isErr=true;
    }
    else if(!isErr && (ui->l_InsertDescrizioneArticolo->text()==""))
    {
            err="Descrizione vuota";
            isErr=true;
    }
    else if(!isErr && (ui->l_InsertDescrizioneArticolo->text().length()>=100))
    {
            err="Descrizione troppo lunga";
            isErr=true;
    }
    else if(!isErr && (ui->l_InsertRarezza->text().length()>=20))
    {
            err="Rarita' troppa lunga";
            isErr=true;
    }
    else if(!isErr && (ui->l_InsertLingua->text().length()>=20))
    {
            err="Lingua troppa lunga";
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

void screenMainArtica::popolaComboBox()
{
    QString qryToEx=QString("select * from tabvab where tipotabella='tipoOgg'");
    QSqlQuery querySelAll(qryToEx,*m_dataBase->dataBase());
    ui->comboInsertTipo->clear();
    ui->comboCercaTipo->clear();
    ui->comboInsertTipo->addItem("");
    ui->comboCercaTipo->addItem("");
    while (querySelAll.next())
    {
        QString codiceTipoOgg=querySelAll.value(querySelAll.record().indexOf("codicevab")).toString();
        ui->comboInsertTipo->addItem(codiceTipoOgg);
        ui->comboCercaTipo->addItem(codiceTipoOgg);
    }

    QString qryToExEspa=QString("select * from tabvab where tipotabella='tipoEspa'");
    QSqlQuery querySelAllE(qryToExEspa,*m_dataBase->dataBase());
    ui->comboInsertEspansione->clear();
    ui->comboCercaEspansione->clear();
    ui->comboInsertEspansione->addItem("");
    ui->comboCercaEspansione->addItem("");
    while (querySelAllE.next())
    {
        QString codiceTipoOgg=querySelAllE.value(querySelAll.record().indexOf("codicevab")).toString();
        ui->comboInsertEspansione->addItem(codiceTipoOgg);
        ui->comboCercaEspansione->addItem(codiceTipoOgg);
    }
}

void screenMainArtica::testoComboCambiato(const QString & testo)
{
    if(testo=="comboInsertTipo")
    {
        tabellaTabvab * oggTab=new tabellaTabvab(m_dataBase);
        oggTab->setTpTab("tipoOgg");
        oggTab->setCdtab(ui->comboInsertTipo->currentText());
        oggTab->loadOggetto();
        ui->l_insertTipo->setText(oggTab->getDescr());
    }
    if(testo=="comboCercaTipo")
    {
        tabellaTabvab * oggTab=new tabellaTabvab(m_dataBase);
        oggTab->setTpTab("tipoOgg");
        oggTab->setCdtab(ui->comboCercaTipo->currentText());
        oggTab->loadOggetto();
        ui->l_espansione_2->setText(oggTab->getDescr());
    }
    if(testo=="comboInsertEspa")
    {
        tabellaTabvab * oggTab=new tabellaTabvab(m_dataBase);
        oggTab->setTpTab("tipoEspa");
        oggTab->setCdtab(ui->comboInsertEspansione->currentText());
        oggTab->loadOggetto();
        ui->l_insertEspansione->setText(oggTab->getDescr());
    }
    if(testo=="comboCercaEspa")
    {
        tabellaTabvab * oggTab=new tabellaTabvab(m_dataBase);
        oggTab->setTpTab("tipoEspa");
        oggTab->setCdtab(ui->comboCercaEspansione->currentText());
        oggTab->loadOggetto();
        ui->l_espansione->setText(oggTab->getDescr());
    }
}

/*quando cambio il tab disattivo attivo campi*/
void screenMainArtica::tabWidgetCambio(int index)
{
    ui->l_espansione->setEnabled(false);
    ui->l_espansione_2->setEnabled(false);
    if(index==0)
    {
        ui->l_cercaDescrizioneArticolo->setEnabled(true);
        ui->comboCercaEspansione->setEnabled(true);
        ui->comboCercaTipo->setEnabled(true);
    }
    if(index==1)
    {
        ui->l_cercaDescrizioneArticolo->setText("");
        ui->l_cercaDescrizioneArticolo->setEnabled(false);
        ui->comboCercaEspansione->setCurrentText("");
        ui->comboCercaEspansione->setEnabled(false);
        ui->comboCercaTipo->setCurrentText("");
        ui->comboCercaTipo->setEnabled(false);
    }
}

void screenMainArtica::creaModello()
{
    /*se esite un model lo elimino*/
    if(m_modelTab1) m_modelTab1->deleteLater();
    if(m_sortTab1) m_sortTab1->deleteLater();

    /*creo model nuovo*/
    m_modelTab1 = new QStandardItemModel();
    m_sortTab1=new QSortFilterProxyModel(this);
    m_sortTab1->setSourceModel(m_modelTab1);
    m_viewTab1->setSortingEnabled(true);
    m_sortTab1->setFilterCaseSensitivity(Qt::CaseSensitive);
    m_viewTab1->sortByColumn(0,Qt::AscendingOrder);

    /*prima riempio lista con stringa vuota*/
    QStringList ll;
    for (int xx =0; xx<=MAXCOLUMNVAL;xx++)
        ll.insert(xx,QString(""));

    /*inserico titolo colonna nella posizione che voglio precedentemente creata*/
    ll[p_codice]=               "Codice";
    ll[p_descrizione]=          "Descrizione";
    ll[p_espans]=               "Espansione";

    /*setto intestazione colonne*/
    m_modelTab1->setHorizontalHeaderLabels(ll);

    /*setto model alla tableview*/
    m_viewTab1->setModel(m_modelTab1);
}

void screenMainArtica::popolaModello()
{
    QString filtroCodice=QString("cdart like '%%1%'").arg(ui->l_cercaCodiceArticolo->text());
    QString filtroDescrizione=QString("descr like '%%1%'").arg(ui->l_cercaDescrizioneArticolo->text());
    QString filtroEspansione=QString("espansione like '%%1%'").arg(ui->comboCercaEspansione->currentText());
    QString filtroTipo=QString("tipologia like '%%1%'").arg(ui->comboCercaTipo->currentText());
    QString qryToEx=QString("select cdart,descr,espansione from artico where %1 and %2 and %3 and %4 order by cdart limit 100")
            .arg(filtroCodice)
            .arg(filtroDescrizione)
            .arg(filtroEspansione)
            .arg(filtroTipo);
    QSqlQuery querySelAll(qryToEx,*m_dataBase->dataBase());
    while(querySelAll.next())
    {

        QList<QStandardItem *>lItms;

        for(int xx=0; xx<=MAXCOLUMNVAL;xx++)
        {
            lItms.insert(xx,new QStandardItem(QString("")));
                (lItms)[xx]->setEditable(false);
        }

        QList<QStandardItem *> li;
        lItms[p_codice]->       setData(querySelAll.value(querySelAll.record().indexOf("cdart")).toString(),Qt::EditRole);
        lItms[p_descrizione]->  setData(querySelAll.value(querySelAll.record().indexOf("descr")).toString(),Qt::EditRole);
        lItms[p_espans]->       setData(querySelAll.value(querySelAll.record().indexOf("espansione")).toString(),Qt::EditRole);

        /*tooltip*/
        lItms[p_codice]->       setToolTip(querySelAll.value(querySelAll.record().indexOf("cdart")).toString());
        lItms[p_descrizione]->  setToolTip(querySelAll.value(querySelAll.record().indexOf("descr")).toString());
        lItms[p_espans]->       setToolTip(querySelAll.value(querySelAll.record().indexOf("espansione")).toString());


        /*appendo al modello la riga*/
        m_modelTab1->appendRow(lItms);
    }

    foreach( int dn, m_colonneNascosteMainView)
            m_viewTab1->setColumnHidden(dn,true);

    /*sistemo le colonne adatte al testo*/
    for(int xx=0; xx<=MAXCOLUMNVAL;xx++)
    {
        m_viewTab1->resizeColumnToContents(xx);
    }

    /*setto la riga dove sono alla prima riga del modello*/
    if(m_modelTab1->rowCount()>=1)
        m_rigaDoveSonoViewTab1=0;
}

void screenMainArtica::ricaricaModelloTab1()
{
    int rigaArrivoPrimaDiAggiornare=m_rigaDoveSonoViewTab1;
    creaModello();
    popolaModello();
    if(m_viewTab1->model()->index(rigaArrivoPrimaDiAggiornare,p_codice).isValid())
    {
        m_rigaDoveSonoViewTab1=rigaArrivoPrimaDiAggiornare;
        m_viewTab1->setCurrentIndex(m_modelTab1->index(rigaArrivoPrimaDiAggiornare,p_codice));
    }
}

void screenMainArtica::doppioTableViewTab1(const QModelIndex &index)
{
    if(index.isValid())
    {
        int rigaSel=m_viewTab1->selectionModel()->currentIndex().row();
        QString cdart=m_modelTab1->index(rigaSel,p_codice).data().toString();
        m_oggArt->resetOggetto();
        m_oggArt->setCodiceArticolo(cdart);
        if(!m_oggArt->loadOggetto())
        {
            resetViewDettaglio();
            return;

        }
        resetViewDettaglio();
        loadArticoloInView();
        ui->tabWidget->setCurrentIndex(1);
    }
}

void screenMainArtica::clickSx()
{
    int nuovaRiga=m_rigaDoveSonoViewTab1;
    nuovaRiga--;
    if(m_viewTab1->model()->index(nuovaRiga,p_codice).isValid())
    {
        m_rigaDoveSonoViewTab1--;
        m_viewTab1->setCurrentIndex(m_modelTab1->index(nuovaRiga,p_codice));
        int rigaSel=m_viewTab1->selectionModel()->currentIndex().row();
        QString cdart=m_modelTab1->index(rigaSel,p_codice).data().toString();
        m_oggArt->resetOggetto();
        m_oggArt->setCodiceArticolo(cdart);
        if(!m_oggArt->loadOggetto())
        {
            resetViewDettaglio();
            return;

        }
        resetViewDettaglio();
        loadArticoloInView();
    }
}

void screenMainArtica::clickDx()
{
    int nuovaRiga=m_rigaDoveSonoViewTab1;
    nuovaRiga++;
    if(m_viewTab1->model()->index(nuovaRiga,p_codice).isValid())
    {
        m_rigaDoveSonoViewTab1++;
        m_viewTab1->setCurrentIndex(m_modelTab1->index(nuovaRiga,p_codice));
        int rigaSel=m_viewTab1->selectionModel()->currentIndex().row();
        QString cdart=m_modelTab1->index(rigaSel,p_codice).data().toString();
        m_oggArt->resetOggetto();
        m_oggArt->setCodiceArticolo(cdart);
        if(!m_oggArt->loadOggetto())
        {
            resetViewDettaglio();
            return;

        }
        resetViewDettaglio();
        loadArticoloInView();
    }
}

void screenMainArtica::pressioneElimina()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Richiesta conferma", "Vuoi eliminare l'articolo?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_oggArt->deleteOggetto();
        m_oggArt->resetOggetto();
        resetViewDettaglio();
        ricaricaModelloTab1();
    } else {
        qDebug() << "Yes was *not* clicked";
    }
}

