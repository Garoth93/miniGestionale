#include "screendettagliodocumenti.h"
#include "ui_screendettagliodocumenti.h"

#include "../widgetsUtility/dialoginfoalert.h"
#include "../backEndUtility/tabellaarticoli.h"
#include "../widgetsUtility/dialogcercaarticolo.h"
#include "../widgetsUtility/dialogcercatipodoc.h"

#include <QMessageBox>

screenDettaglioDocumenti::screenDettaglioDocumenti(QWidget *parent, dataBaseConnection * db) :
    screenBase(parent,db),
    ui(new Ui::screenDettaglioDocumenti)
{
    ui->setupUi(this);
}

screenDettaglioDocumenti::~screenDettaglioDocumenti()
{
    delete ui;
}

void screenDettaglioDocumenti::init()
{
    /*richiamo init base*/
    screenBase::init();

    ui->l_codiceTestata->setReadOnly(true);
    ui->l_dataTestata->setReadOnly(true);

    this->setWindowTitle("Gestione dettaglio documento");

    m_model=new QStandardItemModel();
    m_tableView=ui->tableView;
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setModel(m_model);

    ui->l_viewTotaleDoc->setReadOnly(true);
    ui->l_viewUsciteDoc->setReadOnly(true);
    ui->l_viewEntrateDoc->setReadOnly(true);

    m_oggTesDoc=new tabellaTestataDocumento(m_dataBase);

    /*imposta toolbar alla ui creata nella init base*/
    ui->layoyt_toolbar->addWidget(m_toolBar);

    impostaViewConStato();

    /*click sul widget calendario*/
    connect(ui->calendarWidgetTestata, &QCalendarWidget::clicked, this, [=](){
        ui->l_dataTestata->setText(ui->calendarWidgetTestata->selectedDate().toString("dd/MM/yy"));
    });

    /*connect tasto + e - modello dettaglio doc*/
    ui->tb_aggRiga->setIcon(QIcon((":/aggiungi.png")));
    ui->tb_delRiga->setIcon(QIcon((":/elimina.png")));
    connect(ui->tb_aggRiga, SIGNAL(clicked()), this, SLOT(pressioneAggRigaDettaglio()));
    connect(ui->tb_delRiga, SIGNAL(clicked()), this, SLOT(pressioneDelRigaDettaglio()));

    /*connect doppio click table view cella*/
    connect(m_tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(clickCellaTv_main(const QModelIndex &)));

    /*pressione elimina*/
    connect(m_actionElimina, &QAction::triggered, this, [=](){
        pressioneElimina();
    });

    /*tasto connessione*/
    connect(ui->button_cambiaLogin, &QPushButton::clicked, this, [=](){
        m_dataBase->chiamaConnesione();
        ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));
    });
    ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));

    /*creo menu tasto destro*/
    menu = new QMenu(this);
    menuGestTipoDoc=new QAction("Gestione tipo documento", this);
    menu->addAction(menuGestTipoDoc);
    screenGestioneTabVab = new screenTabvab(nullptr,m_dataBase);
    screenGestioneTabVab->init();
    connect(menuGestTipoDoc, &QAction::triggered, this, [=](){
        screenGestioneTabVab->setTipoTabella("tipoDoc");
        screenGestioneTabVab->popolaModello();
        screenGestioneTabVab->resetViewDettaglio();
        screenGestioneTabVab->setWindowTitle("Gestione tipoDocumento");
        screenGestioneTabVab->show();
    });
    /*menu tasto destro*/
    m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tableView,&QTableView::customContextMenuRequested,this,&screenDettaglioDocumenti::menuGestioneTipoDoc);
}

void screenDettaglioDocumenti::creaMenu()
{
    m_actionConferma = new QAction("Conferma",this);
    m_actionConferma->setIcon(QIcon(":/conferma.png"));

    m_actionElimina = new QAction("Elimina",this);
    m_actionElimina->setIcon(QIcon(":/elimina.png"));

    /*pressione elimina*/
    connect(m_actionElimina, &QAction::triggered, this, [=](){
        //pressioneElimina();
    });

    /*pressione conferma*/
    connect(m_actionConferma, &QAction::triggered, this, [=](){
        pressioneConferma();
    });

    m_actionEsci = new QAction("Esci",this);
    m_actionEsci->setIcon(QIcon(":/esci.png"));
    connect(m_actionEsci, &QAction::triggered, this, [=](){
        this->close();
    });

    m_toolBar->addAction(m_actionConferma);
    m_toolBar->insertAction(m_actionElimina,m_actionConferma);
    m_toolBar->insertAction(m_toolBar->addSeparator(),m_actionElimina);
    m_toolBar->addAction(m_actionEsci);
}

void screenDettaglioDocumenti::preparaInsertiNewDoc()
{
    QString queryToEx=QString("select coalesce(max(idtestata),0)+1 from testatadoc");
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    if(queryRes.next())
        ui->l_codiceTestata->setText(queryRes.value(0).toString());
    ui->l_dataTestata->setText(ui->calendarWidgetTestata->selectedDate().toString("dd/MM/yy"));
    creaModello();
}

void screenDettaglioDocumenti::pressioneConferma()
{
    if(statoView=="inserimento")
    {
        if(controlloCampiPrimaDiInsUp()) return;
        m_oggTesDoc->resetOggetto();
        m_oggTesDoc->setIdTestata(ui->l_codiceTestata->text().toInt());
        m_oggTesDoc->setNote(ui->l_noteTestata->toPlainText());
        m_oggTesDoc->setDataTesDoc(ui->calendarWidgetTestata->selectedDate());

        if(!m_oggTesDoc->insertOggetto())
        {
            return;
        }

        /*proseguo inserendo le righe*/
        int righeTotali=0;
        righeTotali=m_model->rowCount();
        if(righeTotali==0)
        {
            this->close();
            return;
        }
        for(int x=0;x<righeTotali;x++)
        {
            tabellarigadocumento* m_oggRigaDoc=new tabellarigadocumento(m_dataBase);
            int idTestataToIns=m_model->index(x,detDoc_idTestata).data().toInt();
            QString cdartToIns=m_model->index(x,detDoc_codiceArticolo).data().toString();
            QString tipoDocToIns=m_model->index(x,detDoc_tipoDoc).data().toString();
            QString NoteToIns=m_model->index(x,detDoc_note).data().toString();
            double prezzoToIns=m_model->index(x,detDoc_prezzo).data().toDouble();
            int numeroRigaToIns=x+1;
            qDebug() << idTestataToIns << cdartToIns << tipoDocToIns << NoteToIns << prezzoToIns << numeroRigaToIns;
            m_oggRigaDoc->setIdTestata(idTestataToIns);
            m_oggRigaDoc->setNumeroRiga(numeroRigaToIns);
            m_oggRigaDoc->setCodiceArticolo(cdartToIns);
            m_oggRigaDoc->setTipoDoc(tipoDocToIns);
            m_oggRigaDoc->setNote(NoteToIns);
            m_oggRigaDoc->setPrezzo(prezzoToIns);
            m_oggRigaDoc->insertOggetto();
            delete m_oggRigaDoc;
            /*sistema la giacenza in magazzino in caso sia un cdart*/
            if(cdartToIns!="")
            {
                /*se maggiore di zero elimino da mago un quanto lo vendo*/
                if(tipoDocToIns=="VEN")
                {
                    if(oggetoInDeposito(cdartToIns))
                    {
                        tabellaDeposito oggDep(m_dataBase);
                        oggDep.setCodiceArticolo(cdartToIns);
                        oggDep.loadOggetto();
                        oggDep.setQta(oggDep.getQta()-1);
                        oggDep.uploadOggetto();
                    }
                }
                else if(tipoDocToIns=="ACQ")
                {
                    if(oggetoInDeposito(cdartToIns))
                    {
                        tabellaDeposito oggDep(m_dataBase);
                        oggDep.setCodiceArticolo(cdartToIns);
                        oggDep.loadOggetto();
                        oggDep.setQta(oggDep.getQta()+1);
                        oggDep.uploadOggetto();
                    }
                }
            }
        }
        this->close();
    }
    else if(statoView=="modifica")
    {
        if(controlloCampiPrimaDiInsUp()) return;
        m_oggTesDoc->resetOggetto();
        m_oggTesDoc->setIdTestata(ui->l_codiceTestata->text().toInt());
        m_oggTesDoc->setNote(ui->l_noteTestata->toPlainText());
        m_oggTesDoc->setDataTesDoc(ui->calendarWidgetTestata->selectedDate());

        if(!m_oggTesDoc->uploadOggetto())
        {
            return;
        }

        /*elimino tutto rigdoc e poi lo rimetto dentro*/
        QString queryToEx=QString("select * from rigadoc where idtestata=%1 order by numeroriga")
                .arg(m_oggTesDoc->getIdTestata());
        QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
        while (queryRes.next())
        {
            tabellarigadocumento* m_oggRigaDel=new tabellarigadocumento(m_dataBase);
            m_oggRigaDel->setIdTestata(queryRes.value(0).toInt());
            m_oggRigaDel->setNumeroRiga(queryRes.value(1).toInt());
            if(!m_oggRigaDel->loadOggetto())
            {
                delete m_oggRigaDel;
                continue;
            }
            /*sistemo magazzino back*/
            if(m_oggRigaDel->getTipoDoc()!="")
            {
                QString tipoDoc=m_oggRigaDel->getTipoDoc();
                if(tipoDoc=="VEN")
                {
                    if(oggetoInDeposito(m_oggRigaDel->getCodiceArticolo()))
                    {
                        tabellaDeposito oggDep(m_dataBase);
                        oggDep.setCodiceArticolo(m_oggRigaDel->getCodiceArticolo());
                        oggDep.loadOggetto();
                        oggDep.setQta(oggDep.getQta()+1);
                        oggDep.uploadOggetto();
                    }
                }
                else if(tipoDoc=="ACQ")
                {
                    if(oggetoInDeposito(m_oggRigaDel->getCodiceArticolo()))
                    {
                        tabellaDeposito oggDep(m_dataBase);
                        oggDep.setCodiceArticolo(m_oggRigaDel->getCodiceArticolo());
                        oggDep.loadOggetto();
                        oggDep.setQta(oggDep.getQta()-1);
                        oggDep.uploadOggetto();
                    }
                }
            }
            m_oggRigaDel->deleteOggetto();
            delete m_oggRigaDel;
        }

        /*proseguo inserendo le righe*/
        int righeTotali=0;
        righeTotali=m_model->rowCount();
        if(righeTotali==0)
        {
            return;
        }
        for(int x=0;x<righeTotali;x++)
        {
            tabellarigadocumento* m_oggRigaDoc=new tabellarigadocumento(m_dataBase);
            int idTestataToIns=m_model->index(x,detDoc_idTestata).data().toInt();
            QString cdartToIns=m_model->index(x,detDoc_codiceArticolo).data().toString();
            QString tipoDocToIns=m_model->index(x,detDoc_tipoDoc).data().toString();
            QString NoteToIns=m_model->index(x,detDoc_note).data().toString();
            double prezzoToIns=m_model->index(x,detDoc_prezzo).data().toDouble();
            int numeroRigaToIns=x+1;
            qDebug() << idTestataToIns << cdartToIns << tipoDocToIns << NoteToIns << prezzoToIns << numeroRigaToIns;
            m_oggRigaDoc->setIdTestata(idTestataToIns);
            m_oggRigaDoc->setNumeroRiga(numeroRigaToIns);
            m_oggRigaDoc->setCodiceArticolo(cdartToIns);
            m_oggRigaDoc->setTipoDoc(tipoDocToIns);
            m_oggRigaDoc->setNote(NoteToIns);
            m_oggRigaDoc->setPrezzo(prezzoToIns);
            m_oggRigaDoc->insertOggetto();
            delete m_oggRigaDoc;
            /*sistema la giacenza in magazzino in caso sia un cdart*/
            if(cdartToIns!="")
            {
                /*se maggiore di zero elimino da mago un quanto lo vendo*/
                if(tipoDocToIns=="VEN")
                {
                    if(oggetoInDeposito(cdartToIns))
                    {
                        tabellaDeposito oggDep(m_dataBase);
                        oggDep.setCodiceArticolo(cdartToIns);
                        oggDep.loadOggetto();
                        oggDep.setQta(oggDep.getQta()-1);
                        oggDep.uploadOggetto();
                    }
                }
                else if(tipoDocToIns=="ACQ")
                {
                    if(oggetoInDeposito(cdartToIns))
                    {
                        tabellaDeposito oggDep(m_dataBase);
                        oggDep.setCodiceArticolo(cdartToIns);
                        oggDep.loadOggetto();
                        oggDep.setQta(oggDep.getQta()+1);
                        oggDep.uploadOggetto();
                    }
                }
            }
        }
    }
}

bool screenDettaglioDocumenti::controlloCampiPrimaDiInsUp()
{
    QString error="";
    bool isErr=false;
    if(!isErr && ui->l_noteTestata->toPlainText().length()>250)
    {
        isErr=true;
        error="Note troppo lunghe";
    }
    if(isErr)
    {
        dialogInfoAlert tmpDialog;
        tmpDialog.init("Errore",error,2);
        tmpDialog.exec();
    }
    return isErr;
}

void screenDettaglioDocumenti::resetCampi()
{
    ui->l_codiceTestata->setText("");
    ui->calendarWidgetTestata->setSelectedDate(QDate::currentDate());
    ui->l_noteTestata->setPlainText("");
    ui->l_viewTotaleDoc->setText("");
    ui->l_viewUsciteDoc->setText("");
    ui->l_viewEntrateDoc->setText("");
}

void screenDettaglioDocumenti::impostaViewConStato()
{
    if(statoView=="modifica")
    {
        setModifica();
    }
    else if(statoView=="inserimento")
    {
        setInserimento();
    }
}

void screenDettaglioDocumenti::setModifica()
{
    m_actionElimina->setVisible(true);
    m_actionEsci->setVisible(false);
    ui->label_utenteLog->setVisible(false);
    ui->button_cambiaLogin->setVisible(false);
}

void screenDettaglioDocumenti::setInserimento()
{
    m_actionElimina->setVisible(false);
    m_actionEsci->setVisible(true);
    ui->label_utenteLog->setVisible(true);
    ui->button_cambiaLogin->setVisible(true);
}

void screenDettaglioDocumenti::creaModello()
{
    /*se esite un model lo elimino*/
    if(m_model) m_model->deleteLater();

    /*creo model nuovo*/
    m_model = new QStandardItemModel();

    /*segnali sul modello*/
    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(calcolaPrezziAlVariareDelModello(QModelIndex,QModelIndex)));

    /*prima riempio lista con stringa vuota*/
    QStringList ll;
    for (int xx =0; xx<=DETDOCMAXCOLUMNVAL;xx++)
        ll.insert(xx,QString(""));

    /*inserico titolo colonna nella posizione che voglio precedentemente creata*/
    ll[detDoc_idTestata]=        "Id testata";
    ll[detDoc_codiceArticolo]=   "Codice articolo";
    ll[detDoc_descrizione]=      "Descrizione";
    ll[detDoc_tipoDoc]=          "Tipo doc.";
    ll[detDoc_prezzo]=           "Prezzo";
    ll[detDoc_note]=             "Note";

    /*setto intestazione colonne*/
    m_model->setHorizontalHeaderLabels(ll);

    /*setto model alla tableview*/
    m_tableView->setModel(m_model);

    m_tableView->resizeColumnsToContents();
    //m_tableView->horizontalHeader()->setStretchLastSection(true);
}

void screenDettaglioDocumenti::pressioneAggRigaDettaglio()
{
    QList<QStandardItem *>lItms;

    for(int xx=0; xx<=DETDOCMAXCOLUMNVAL;xx++)
    {
        lItms.insert(xx,new QStandardItem(QString("")));
        if(xx==detDoc_idTestata || xx==detDoc_codiceArticolo || xx==detDoc_descrizione || xx==detDoc_tipoDoc)
            (lItms)[xx]->setEditable(false);
        if(xx==detDoc_idTestata)
            lItms[detDoc_idTestata]->setData(ui->l_codiceTestata->text(),Qt::EditRole);
        QColor colorToIns(140, 245, 150);
        lItms[xx]->setBackground(colorToIns);
    }

    m_model->appendRow(lItms);
    m_tableView->resizeColumnsToContents();
    //m_tableView->horizontalHeader()->setStretchLastSection(true);
}

void screenDettaglioDocumenti::pressioneDelRigaDettaglio()
{
    if(m_tableView->selectionModel()->currentIndex().isValid())
    {
       int rigaSel=m_tableView->selectionModel()->currentIndex().row();
       m_model->removeRow(rigaSel);
    }
}

void screenDettaglioDocumenti::calcolaPrezziAlVariareDelModello(QModelIndex a,QModelIndex b)
{
    int righeTotali=0;
    righeTotali=m_model->rowCount();
    if(righeTotali==0)
        return;
    double totale=0;
    double totaleEntrare=0;
    double totaleUscite=0;
    for(int x=0;x<righeTotali;x++)
    {
        double prezzoToIns=m_model->index(x,detDoc_prezzo).data().toDouble();
        totale=totale+prezzoToIns;
        if(prezzoToIns<=0)
            totaleUscite=totaleUscite+prezzoToIns;
        else
            totaleEntrare=totaleEntrare+prezzoToIns;
    }
    ui->l_viewTotaleDoc->setText(QString("%1 euro").arg(totale));
    ui->l_viewUsciteDoc->setText(QString("%1 euro").arg(totaleUscite));
    ui->l_viewEntrateDoc->setText(QString("%1 euro").arg(totaleEntrare));
    coloraRigheDoc();
}

void screenDettaglioDocumenti::menuGestioneTipoDoc(QPoint pos)
{
    if(m_tableView->indexAt(pos).column()==detDoc_tipoDoc)
        menu->popup(m_tableView->mapToGlobal(pos));
}

void screenDettaglioDocumenti::clickCellaTv_main(const QModelIndex &index)
{
    if (index.isValid())
    {
        if(index.column()==detDoc_codiceArticolo)
        {
            QString toSearch="";
            dialogCercaArticolo cercaDialog(this,m_dataBase,&toSearch);
            cercaDialog.exec();
            if(toSearch=="") return;
            int rigaSel=m_tableView->selectionModel()->currentIndex().row();
            m_model->setData(m_model->index(rigaSel,detDoc_codiceArticolo),toSearch,Qt::EditRole);
            m_model->setData(m_model->index(rigaSel,detDoc_descrizione),getDescrizioneArticoloDaCodice(toSearch),Qt::EditRole);
        }
        if(index.column()==detDoc_tipoDoc)
        {
            QString toSearch="";
            dialogCercaTipoDoc cercaDialog(this,m_dataBase,&toSearch);
            cercaDialog.exec();
            if(toSearch=="") return;
            int rigaSel=m_tableView->selectionModel()->currentIndex().row();
            m_model->setData(m_model->index(rigaSel,detDoc_tipoDoc),toSearch,Qt::EditRole);
        }
    }
}

tabellaTestataDocumento *screenDettaglioDocumenti::oggTesDoc() const
{
    return m_oggTesDoc;
}

void screenDettaglioDocumenti::setOggTesDoc(tabellaTestataDocumento *oggTesDoc)
{
    m_oggTesDoc = oggTesDoc;
}

void screenDettaglioDocumenti::popolaDettaglioDatoId(QString id)
{
    creaModello();
    resetCampi();
    m_oggTesDoc->resetOggetto();
    m_oggTesDoc->setIdTestata(id.toInt());
    m_oggTesDoc->loadOggetto();
    ui->l_codiceTestata->setText(id);
    ui->l_dataTestata->setText(m_oggTesDoc->getDataTesDoc().toString("dd/MM/yy"));
    ui->calendarWidgetTestata->setSelectedDate(m_oggTesDoc->getDataTesDoc());
    ui->l_noteTestata->setPlainText(m_oggTesDoc->getNote());
    QString queryToEx=QString("select * from rigadoc where idtestata=%1 order by numeroriga")
            .arg(id);
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    double totale=0;
    double totaleEntrare=0;
    double totaleUscite=0;
    while(queryRes.next())
    {
        double prezzo=queryRes.value(4).toDouble();
        totale=totale+prezzo;
        if(prezzo<=0)
            totaleUscite=totaleUscite+prezzo;
        else
            totaleEntrare=totaleEntrare+prezzo;
        QList<QStandardItem*> ll;
        ll.push_back(new QStandardItem(queryRes.value(0).toString()));
        ll.push_back(new QStandardItem(queryRes.value(2).toString()));
        ll.push_back(new QStandardItem(getDescrizioneArticoloDaCodice(queryRes.value(2).toString())));
        ll.push_back(new QStandardItem(queryRes.value(3).toString()));
        ll.push_back(new QStandardItem(queryRes.value(4).toString()));
        ll.push_back(new QStandardItem(queryRes.value(5).toString()));
        ll[0]->setEditable(false);
        ll[1]->setEditable(false);
        ll[2]->setEditable(false);
        ll[3]->setEditable(false);
        m_model->appendRow(ll);
    }
    m_tableView->resizeColumnsToContents();
    //m_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->l_viewTotaleDoc->setText(QString("%1 euro").arg(totale));
    ui->l_viewUsciteDoc->setText(QString("%1 euro").arg(totaleUscite));
    ui->l_viewEntrateDoc->setText(QString("%1 euro").arg(totaleEntrare));

    coloraRigheDoc();
}

bool screenDettaglioDocumenti::oggetoInDeposito(QString cdart)
{
    QString queryToEx=QString("select count(*) from deposito where cdart='%1'")
            .arg(cdart);
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    if(queryRes.next())
    {
        if(queryRes.value(0).toInt()>0)
            return true;
    }
    return false;
}

void screenDettaglioDocumenti::pressioneElimina()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Richiesta conferma", "Vuoi eliminare l'articolo?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        /*elimino tutto rigdoc sistemando magazzino*/
        QString queryToEx=QString("select * from rigadoc where idtestata=%1")
                .arg(m_oggTesDoc->getIdTestata());
        QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
        while (queryRes.next())
        {
            tabellarigadocumento* m_oggRigaDel=new tabellarigadocumento(m_dataBase);
            m_oggRigaDel->setIdTestata(queryRes.value(0).toInt());
            m_oggRigaDel->setNumeroRiga(queryRes.value(1).toInt());
            if(!m_oggRigaDel->loadOggetto())
            {
                delete m_oggRigaDel;
                continue;
            }
            /*sistemo magazzino back*/
            if(m_oggRigaDel->getCodiceArticolo()!="")
            {
                double prezzoControllo=0;
                prezzoControllo=m_oggRigaDel->getPrezzo();
                if(prezzoControllo>0)
                {
                    if(oggetoInDeposito(m_oggRigaDel->getCodiceArticolo()))
                    {
                        tabellaDeposito oggDep(m_dataBase);
                        oggDep.setCodiceArticolo(m_oggRigaDel->getCodiceArticolo());
                        oggDep.loadOggetto();
                        oggDep.setQta(oggDep.getQta()+1);
                        oggDep.uploadOggetto();
                    }
                }
                else
                {
                    if(oggetoInDeposito(m_oggRigaDel->getCodiceArticolo()))
                    {
                        tabellaDeposito oggDep(m_dataBase);
                        oggDep.setCodiceArticolo(m_oggRigaDel->getCodiceArticolo());
                        oggDep.loadOggetto();
                        oggDep.setQta(oggDep.getQta()-1);
                        oggDep.uploadOggetto();
                    }
                }
            }
            m_oggRigaDel->deleteOggetto();
            delete m_oggRigaDel;
        }
        queryToEx=QString("delete from testatadoc where idtestata=%1").arg(m_oggTesDoc->getIdTestata());
        queryRes.clear();
        queryRes.prepare(queryToEx);
        queryRes.exec();
        m_oggTesDoc->resetOggetto();
        creaModello();
    } else {
        qDebug() << "Yes was *not* clicked";
    }
}

void screenDettaglioDocumenti::coloraRigheDoc()
{
    /*proseguo inserendo le righe*/
    int righeTotali=0;
    righeTotali=m_model->rowCount();
    if(righeTotali==0)
    {
        return;
    }
    for(int x=0;x<righeTotali;x++)
    {
        double prezzo=m_model->index(x,detDoc_prezzo).data().toDouble();
        QColor colorToIns;
        if(prezzo>=0)
            colorToIns=QColor(140, 245, 150);
        else
            colorToIns=QColor(255, 105, 105);
        for(int y=0; y<=DETDOCMAXCOLUMNVAL; y++)
            m_model->item(x,y)->setBackground(colorToIns);
    }
}
