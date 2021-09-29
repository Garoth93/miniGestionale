#include "screenesploradocumenti.h"
#include "ui_screenesploradocumenti.h"
#include "../widgetsUtility/dialogseparatore.h"
#include <QFileDialog>
#include <QFile>
#include "../widgetsUtility/dialoginfoalert.h"
#include "../widgetsUtility/dialogcercatipodoc.h"

screenEsploraDocumenti::screenEsploraDocumenti(QWidget *parent, dataBaseConnection * db) :
    screenBase(parent,db),
    ui(new Ui::screenEsploraDocumenti)
{
    ui->setupUi(this);
}

screenEsploraDocumenti::~screenEsploraDocumenti()
{
    delete ui;
}

void screenEsploraDocumenti::init()
{
    /*richiamo init base*/
    screenBase::init();

    setWindowTitle("Esplora documenti");

    m_model=new QStandardItemModel();
    m_tableView=ui->tableViewEsploraDoc;
    m_tableView->setModel(m_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->r_testata->setChecked(true);
//    settaCercaUtilizzabiliRadio();
//    connect(ui->r_testata, &QRadioButton::pressed, this, [=](){
//        settaCercaUtilizzabiliRadio();
//    });
//    connect(ui->r_righe, &QRadioButton::pressed, this, [=](){
//        settaCercaUtilizzabiliRadio();
//    });

    connect(m_tableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(clickOnTableView(const QModelIndex &)));

    /*imposta toolbar alla ui creata nella init base*/
    ui->layoyt_toolbar->addWidget(m_toolBar);

    this->creaModello();

    ui->l_viewTotaleDoc->setReadOnly(true);
    ui->l_viewTotaleEntrate->setReadOnly(true);
    ui->l_viewTotaleSpese->setReadOnly(true);


    QDate toInsDa(QDate::currentDate().year(),01,01);
    QDate toInsA(QDate::currentDate().year()+1,01,01);
    ui->dataEditCercaDa->setDate(toInsDa);
    ui->dataEditCercaDa_2->setDate(toInsA);

    m_screenDettaglio=new screenDettaglioDocumenti(nullptr,m_dataBase);
    m_screenDettaglio->init();
    m_screenDettaglio->setStatoView("modifica");
    m_screenDettaglio->impostaViewConStato();
    m_screenDettaglio->resetCampi();
    m_screenDettaglio->creaModello();

    /*addo dettaglio all view*/
    ui->layoutDettaglioDoc->addWidget(m_screenDettaglio);

    /*di default non editabile il dettaglio*/
    m_screenDettaglio->setEnabled(false);

    ui->tb_cercaDett->setIcon(QIcon((":/cerca.png")));
    connect(ui->tb_cercaDett, &QToolButton::clicked, this, [=](){
        creaModelloConFiltri();
    });

    /*quando premo elimino ripolo queste*/
    connect(m_screenDettaglio->actionElimina(), &QAction::triggered, this, [=](){
        creaModelloConFiltri();
        m_screenDettaglio->resetCampi();
        m_screenDettaglio->setEnabled(false);
    });

    /*quando premo conferma ripolo queste*/
    connect(m_screenDettaglio->actionConferma(), &QAction::triggered, this, [=](){
        creaModelloConFiltri();
    });

    /*bottone salva csv*/
    ui->tb_salvaCsv->setIcon(QIcon((":/save.png")));
    connect(ui->tb_salvaCsv, &QToolButton::clicked, this, [=]()
    {
        QFileDialog * dialogTake;
        dialogTake=new QFileDialog();
        dialogTake->exec();
        QStringList nomeFIle;
        nomeFIle = dialogTake->selectedFiles();
        delete dialogTake;
        QString pathFile="";
        if(nomeFIle.count()!=0)
        {
            pathFile=nomeFIle[0];
            salvaFileInCsv(pathFile);
        }
    });

    /*tasto connessione*/
    connect(ui->button_cambiaLogin, &QPushButton::clicked, this, [=](){
        m_dataBase->chiamaConnesione();
        ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));
    });
    ui->label_utenteLog->setText(QString("Ciao %1").arg(m_dataBase->utente()));

    ui->tb_tipoDoc->setIcon(QIcon((":/up.png")));
    connect(ui->tb_tipoDoc, &QToolButton::clicked, this, [=](){
        QString toSearch="";
        dialogCercaTipoDoc cercaDialog(this,m_dataBase,&toSearch);
        cercaDialog.exec();
        if(toSearch=="") return;
        ui->l_cercaTipoDoc->setText(toSearch);
    });
}

void screenEsploraDocumenti::creaMenu()
{
    m_actionEsci = new QAction("Esci",this);
    m_actionEsci->setIcon(QIcon(":/esci.png"));
    connect(m_actionEsci, &QAction::triggered, this, [=](){
        this->close();
    });

    m_toolBar->addAction(m_actionEsci);
}

void screenEsploraDocumenti::creaModello()
{
    /*se esite un model lo elimino*/
    if(m_model) m_model->deleteLater();

    /*creo model nuovo*/
    m_model = new QStandardItemModel();

    m_model->setHorizontalHeaderLabels(QStringList() << "Id testata" << "Data" << "Note");

    /*setto model alla tableview*/
    m_tableView->setModel(m_model);

    QString queryToEx=QString("select * from testatadoc");
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    QList<int> lsitaId;
    while(queryRes.next())
    {
        lsitaId.push_back(queryRes.value(0).toInt());
        QList<QStandardItem*> ll;
        ll.push_back(new QStandardItem(queryRes.value(0).toString()));
        ll.push_back(new QStandardItem(queryRes.value(1).toDate().toString("dd/MM/yy")));
        ll.push_back(new QStandardItem(queryRes.value(2).toString()));
        ll[0]->setEditable(false);
        ll[1]->setEditable(false);
        ll[2]->setEditable(false);
        m_model->appendRow(ll);
    }
    m_tableView->resizeColumnsToContents();
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    settaLabelPrezzo(&lsitaId);
}

void screenEsploraDocumenti::creaModelloConFiltri()
{
    /*se esite un model lo elimino*/
    if(m_model) m_model->deleteLater();

    /*creo model nuovo*/
    m_model = new QStandardItemModel();

    if(ui->r_testata->isChecked())
    {
        m_model->setHorizontalHeaderLabels(QStringList() << "Id testata" << "Data" << "Note");

        /*setto model alla tableview*/
        m_tableView->setModel(m_model);

        QDate dataIni=ui->dataEditCercaDa->date();
        QDate dataFine=ui->dataEditCercaDa_2->date();
        QString filtroData=QString("data1 >='%1' and data1 <='%2'").arg(dataIni.toString("yyyyMMdd")).arg(dataFine.toString("yyyyMMdd"));
        QString queryToEx=QString("select * from testatadoc where %1").arg(filtroData);
        if(ui->l_cercaIdTestata->text()!="")
            queryToEx=QString("select * from testatadoc where idtestata= %1").arg(ui->l_cercaIdTestata->text());
        QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
        qDebug() << queryToEx;
        QList<int> lsitaId;
        while(queryRes.next())
        {
            lsitaId.push_back(queryRes.value(0).toInt());
            QList<QStandardItem*> ll;
            ll.push_back(new QStandardItem(queryRes.value(0).toString()));
            ll.push_back(new QStandardItem(queryRes.value(1).toDate().toString("dd/MM/yy")));
            ll.push_back(new QStandardItem(queryRes.value(2).toString()));
            ll[0]->setEditable(false);
            ll[1]->setEditable(false);
            ll[2]->setEditable(false);
            m_model->appendRow(ll);
        }
        m_tableView->resizeColumnsToContents();
        m_tableView->horizontalHeader()->setStretchLastSection(true);
        settaLabelPrezzo(&lsitaId);
    }

    if(ui->r_righe->isChecked())
    {
        m_model->setHorizontalHeaderLabels(QStringList() << "Id testata" << "Data" << "Articolo" << "Descrizione" << "Prezzo" << "Tipo doc.");

        /*setto model alla tableview*/
        m_tableView->setModel(m_model);

        QDate dataIni=ui->dataEditCercaDa->date();
        QDate dataFine=ui->dataEditCercaDa_2->date();
        QString filtroData=QString("data1 >='%1' and data1 <='%2'").arg(dataIni.toString("yyyyMMdd")).arg(dataFine.toString("yyyyMMdd"));
        QString tipoDocToSrc="";
        tipoDocToSrc=ui->l_cercaTipoDoc->text();
        tipoDocToSrc=QString("'%%1%'").arg(tipoDocToSrc);
        QString queryToEx=QString("select * from rigadoc a join testatadoc b using(idtestata) where %1 and tipodocumento like %2 order by idtestata,numeroriga").arg(filtroData).arg(tipoDocToSrc);
        QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
        qDebug() << queryToEx;
        double totale=0;
        double totaleSpese=0;
        double totaleEntrate=0;
        while(queryRes.next())
        {
            double prezzoConta=queryRes.value(4).toDouble();
            totale=totale+prezzoConta;
            if(prezzoConta>=0)
                totaleEntrate=totaleEntrate+prezzoConta;
            else
                totaleSpese=totaleSpese+prezzoConta;
            QList<QStandardItem*> ll;
            ll.push_back(new QStandardItem(queryRes.value(0).toString()));
            ll.push_back(new QStandardItem(queryRes.value(1).toDate().toString("dd/MM/yy")));
            ll.push_back(new QStandardItem(queryRes.value(2).toString()));
            ll.push_back(new QStandardItem(getDescrizioneArticoloDaCodice(queryRes.value(2).toString())));
            ll.push_back(new QStandardItem(queryRes.value(4).toString()));
            ll.push_back(new QStandardItem(queryRes.value(3).toString()));
            ll[0]->setEditable(false);
            ll[1]->setEditable(false);
            ll[2]->setEditable(false);
            ll[3]->setEditable(false);
            ll[4]->setEditable(false);
            ll[5]->setEditable(false);
            m_model->appendRow(ll);
        }
        m_tableView->resizeColumnsToContents();
        m_tableView->horizontalHeader()->setStretchLastSection(true);
        ui->l_viewTotaleDoc->setText(QString("%1").arg(totale));
        ui->l_viewTotaleEntrate->setText(QString("%1").arg(totaleEntrate));
        ui->l_viewTotaleSpese->setText(QString("%1").arg(totaleSpese));
    }
}

void screenEsploraDocumenti::settaLabelPrezzo(QList<int> * lista)
{
    QList<int>::iterator it = lista->begin();
    double totale=0;
    double totaleSpese=0;
    double totaleEntrate=0;
    for(;it!=lista->end();it++)
    {
        QString queryToEx=QString("select prezzo from rigadoc where idtestata=%1").arg(*it);
        QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
        while(queryRes.next())
        {
            double prezzoConta=queryRes.value(0).toDouble();
            totale=totale+prezzoConta;
            if(prezzoConta>=0)
                totaleEntrate=totaleEntrate+prezzoConta;
            else
                totaleSpese=totaleSpese+prezzoConta;
        }
    }
    ui->l_viewTotaleDoc->setText(QString("%1").arg(totale));
    ui->l_viewTotaleEntrate->setText(QString("%1").arg(totaleEntrate));
    ui->l_viewTotaleSpese->setText(QString("%1").arg(totaleSpese));
}

bool screenEsploraDocumenti::salvaFileInCsv(QString path)
{
    QFile file(QString("%1.csv").arg(path));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return false;
    }
    QTextStream stream(&file);
    if(!m_model) return false;
    int righeModelo=0;
    righeModelo=m_model->rowCount();
    QString separatore=";";
    dialogSeparatore * dialSep = new dialogSeparatore(this);
    dialSep->init(&separatore);
    dialSep->exec();
    double totale=0;
    double totaleEntrare=0;
    double totaleUscite=0;
    for(int x=0;x<righeModelo;++x)
    {
        int key=m_model->index(x,0).data().toInt();
        tabellaTestataDocumento * oggTes=new tabellaTestataDocumento(m_dataBase);
        oggTes->setIdTestata(key);
        oggTes->loadOggetto();
        stream << QString("ID%1").arg(separatore)
               << QString("DATA%1").arg(separatore)
               << QString("NOTE")
               << "\n";
        stream << QString("%1%2").arg(oggTes->getIdTestata()).arg(separatore)
               << QString("%1%2").arg(oggTes->getDataTesDoc().toString("dd/MM/yy")).arg(separatore)
               << QString("%1").arg(oggTes->getNote())
               << "\n";
        delete oggTes;
        QString queryToEx=QString("select * from rigadoc where idtestata=%1 order by numeroriga")
                .arg(key);
        QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
        stream << QString("CODICE ARTICOLO%1").arg(separatore)
               << QString("DESCRIZIONE%1").arg(separatore)
               << QString("TIPO DOC.%1").arg(separatore)
               << QString("PREZZO%1").arg(separatore)
               << QString("NOTE")
               << "\n";
        while(queryRes.next())
        {
            double prezzo=queryRes.value(4).toDouble();
            totale=totale+prezzo;
            if(prezzo<=0)
                totaleUscite=totaleUscite+prezzo;
            else
                totaleEntrare=totaleEntrare+prezzo;
            stream << QString("%1%2").arg(queryRes.value(2).toString()).arg(separatore)
                   << QString("%1%2").arg(getDescrizioneArticoloDaCodice(queryRes.value(2).toString())).arg(separatore)
                   << QString("%1%2").arg(queryRes.value(3).toString()).arg(separatore)
                   << QString("%1%2").arg(queryRes.value(4).toString()).arg(separatore)
                   << QString("%1").arg(queryRes.value(5).toString())
                   << "\n";
        }
        stream << QString("TOTALE%1").arg(separatore)
               << QString("ENTRATE%1").arg(separatore)
               << QString("SPESE")
               << "\n";
        stream << QString("%1%2").arg(QString("%1").arg(totale)).arg(separatore)
               << QString("%1%2").arg(QString("%1").arg(totaleEntrare)).arg(separatore)
               << QString("%1").arg(QString("%1").arg(totaleUscite))
               << "\n";
        stream << "\n";
    }
    stream << QString("TOTALE%1").arg(separatore)
           << QString("ENTRATE%1").arg(separatore)
           << QString("SPESE")
           << "\n";
    stream << QString("%1%2").arg(QString("%1").arg(totale)).arg(separatore)
           << QString("%1%2").arg(QString("%1").arg(totaleEntrare)).arg(separatore)
           << QString("%1").arg(QString("%1").arg(totaleUscite))
           << "\n";
    file.close();
    dialogInfoAlert tmpDialog;
    tmpDialog.init("Ottimo","Salvataggio riuscito",1);
    tmpDialog.exec();
    return true;
}

void screenEsploraDocumenti::clickOnTableView(const QModelIndex & index)
{
    if(index.isValid())
    {
        int rigaSel=m_tableView->selectionModel()->currentIndex().row();
        QString codiceTestata=m_model->index(rigaSel,0).data().toString();
        m_screenDettaglio->setEnabled(true);
        m_screenDettaglio->popolaDettaglioDatoId(codiceTestata);
    }
}

void screenEsploraDocumenti::settaCercaUtilizzabiliRadio()
{
}
