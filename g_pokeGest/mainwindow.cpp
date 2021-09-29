#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include "./backEndUtility/dialogerrori.h"
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include "./widgetsUtility/dialogseparatore.h"
#include "./widgetsUtility/dialoginfoalert.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Menu'");

    m_viewMenu=ui->viewMenu;
    m_modelMenu= new QStandardItemModel();
    creaMenu();
    /*connect tableView*/
    connect(m_viewMenu, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doppioClickMenu(const QModelIndex &)));

    /*creo connessione al db*/
    m_dataBase=new dataBaseConnection();
    m_dataBase->init();

    p_artica = new screenMainArtica(nullptr,m_dataBase);
    p_artica->init();
//    connect(ui->pushButton, &QPushButton::clicked, this, [=]()
//    {
//        p_artica->show();
//    });

    p_magazzino = new screenMainDeposito(nullptr,m_dataBase);
    p_magazzino->init();
//    connect(ui->pushButton_2, &QPushButton::clicked, this, [=]()
//    {
//        p_magazzino->show();
//    });

    p_documenti = new screenDettaglioDocumenti(nullptr,m_dataBase);
    p_documenti->init();
//    connect(ui->pushButton_3, &QPushButton::clicked, this, [=]()
//    {
//        p_documenti->setStatoView("inserimento");
//        p_documenti->impostaViewConStato();
//        p_documenti->resetCampi();
//        p_documenti->preparaInsertiNewDoc();
//        p_documenti->show();
//    });

    p_esploraDoc = new screenEsploraDocumenti(nullptr,m_dataBase);
    p_esploraDoc->init();
//    connect(ui->pushButton_4, &QPushButton::clicked, this, [=]()
//    {
//        p_esploraDoc->show();
//    });

    m_viewMenu->verticalHeader()->setVisible(false);
    m_viewMenu->horizontalHeader()->setVisible(false);
    m_viewMenu->setStyleSheet(
                "QTableView {"
                "border: 1px solid gray; border-radius: 5px; background: rgb(208, 214, 211); padding: 3px"
                "}"
                "QTableView::item {"
                "border: 2px solid gray;  border-radius: 5px; background: transparent; padding: 3px"
                "}"
                "QTableView::item:selected {"
                "color : rgb(66,165,245); border: 2px solid rgb(66,165,245);  border-radius: 5px; background: rgb(220, 247, 246); padding: 3px"
                "}"
                "QTableView::item:hover {"
                "border: 2px solid gray;  border-radius: 5px; background: rgb(220, 247, 246); padding: 3px"
                "}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::creaMenu()
{
    /*se esite un model lo elimino*/
    if(m_modelMenu) m_modelMenu->deleteLater();

    m_modelMenu = new QStandardItemModel();

    QStringList ll;
    ll.insert(0,"MENU");
    m_modelMenu->setHorizontalHeaderLabels(ll);

    m_viewMenu->setModel(m_modelMenu);

    QList<QStandardItem *>lItms;
    lItms.insert(0,new QStandardItem(QString("ANAGRAFICA ARTICOLI")));
    (lItms)[0]->setEditable(false);
    m_modelMenu->appendRow(lItms);
    lItms.clear();

    lItms.insert(0,new QStandardItem(QString("GESTIONE DEPOSITI")));
    (lItms)[0]->setEditable(false);
    m_modelMenu->appendRow(lItms);
    lItms.clear();

    lItms.insert(0,new QStandardItem(QString("INSERISCI NUOVO DOCUMENTO")));
    (lItms)[0]->setEditable(false);
    m_modelMenu->appendRow(lItms);
    lItms.clear();

    lItms.insert(0,new QStandardItem(QString("ESPLORA DOCUMENTI")));
    (lItms)[0]->setEditable(false);
    m_modelMenu->appendRow(lItms);
    lItms.clear();

    lItms.insert(0,new QStandardItem(QString("STAMPA ANAGRAFICE GIACENZE")));
    (lItms)[0]->setEditable(false);
    m_modelMenu->appendRow(lItms);
    lItms.clear();

    m_viewMenu->horizontalHeader()->setStretchLastSection(true);
    //m_viewMenu->resizeColumnsToContents();
}

void MainWindow::doppioClickMenu(const QModelIndex &index)
{
    qDebug() << "asdasd" << m_viewMenu->selectionModel()->currentIndex().row();
    if(index.isValid())
    {
        int rigaSel=m_viewMenu->selectionModel()->currentIndex().row();
        if(rigaSel==0)
        {
            p_artica->show();
        }
        else if(rigaSel==1)
        {
            p_magazzino->show();
        }
        else if(rigaSel==2)
        {
            p_documenti->setStatoView("inserimento");
            p_documenti->impostaViewConStato();
            p_documenti->resetCampi();
            p_documenti->preparaInsertiNewDoc();
            p_documenti->show();
        }
        else if(rigaSel==3)
        {
            p_esploraDoc->show();
        }
        else if(rigaSel==4)
        {
            anagraficheToCsv();
        }
    }
}

void MainWindow::anagraficheToCsv()
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
    }
    else
    {
        return;
    }
    QFile file(QString("%1.csv").arg(pathFile));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }
    QString separatore=";";
    dialogSeparatore * dialSep = new dialogSeparatore(this);
    dialSep->init(&separatore);
    dialSep->exec();
    QTextStream stream(&file);
    QString queryToEx=QString("select * from artico");
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    stream << QString("CODICE ARTICOLO%1").arg(separatore)
           << QString("DESCRIZIONE%1").arg(separatore)
           << QString("ESPANSIONE%1").arg(separatore)
           << QString("RAREZZA%1").arg(separatore)
           << QString("LINGUA%1").arg(separatore)
           << QString("TIPOLOGIA%1").arg(separatore)
           << QString("GIACENZA")
           << "\n";
    while(queryRes.next())
    {
        stream << QString("%1%2").arg(queryRes.value(0).toString()).arg(separatore)
               << QString("%1%2").arg(queryRes.value(1).toString()).arg(separatore)
               << QString("%1%2").arg(queryRes.value(2).toString()).arg(separatore)
               << QString("%1%2").arg(queryRes.value(3).toString()).arg(separatore)
               << QString("%1%2").arg(queryRes.value(4).toString()).arg(separatore)
               << QString("%1%2").arg(queryRes.value(5).toString()).arg(separatore)
               << QString("%1").arg(getGiacenzaDatoCdart(queryRes.value(0).toString()))
               << "\n";
    }
    file.close();
    dialogInfoAlert tmpDialog;
    tmpDialog.init("Ottimo","Salvataggio riuscito",1);
    tmpDialog.exec();
}

int MainWindow::getGiacenzaDatoCdart(QString cdart)
{
    QString queryToEx=QString("select qta from deposito where cdart='%1'").arg(cdart);
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    if(queryRes.next())
        return queryRes.value(0).toInt();
    return 0;
}

