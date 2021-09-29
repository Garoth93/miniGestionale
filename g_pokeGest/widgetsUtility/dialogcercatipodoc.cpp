#include "dialogcercatipodoc.h"
#include "ui_dialogcercatipodoc.h"
#include <QDebug>

dialogCercaTipoDoc::dialogCercaTipoDoc(QWidget *parent,dataBaseConnection * db, QString * cdartBack) :
    QDialog(parent),
    ui(new Ui::dialogCercaTipoDoc)
{
    ui->setupUi(this);

    m_model=new QStandardItemModel();

    m_tableView=ui->tv_main;
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setModel(m_model);
    m_tableView->hide();
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->l_cerca,SIGNAL(textChanged(const QString &)),this,SLOT(cercaParole(const QString &)));
    connect(m_tableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(doppioClickConferma(const QModelIndex &)));

    m_dataBase=db;
    toRet=cdartBack;
    *toRet="";

    connect(ui->b_esci, &QPushButton::clicked, this, [=](){
        *toRet="";
        this->close();
    });

    this->setWindowTitle("Cerca articolo");
}

dialogCercaTipoDoc::~dialogCercaTipoDoc()
{
    delete ui;
}

void dialogCercaTipoDoc::cercaParole(const QString & testo)
{
    if(testo=="")
    {
        m_model->clear();
        m_tableView->hide();
        return;
    }
    if(!m_tableView->isVisible())
        m_tableView->show();
    QString queryToEx=QString("select codicevab,fill1 from tabvab where tipotabella='tipoDoc' and (codicevab like '%%1%' or fill1 like '%%2%') order by codicevab limit 20")
            .arg(testo)
            .arg(testo);
    QSqlQuery queryRes(queryToEx,*m_dataBase->dataBase());
    m_model->clear();
    m_model->setHorizontalHeaderLabels(QStringList() << "Tipo Doc." << "Descrizione");
    qDebug() << queryToEx;
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

void dialogCercaTipoDoc::doppioClickConferma(const QModelIndex & index)
{
    if(index.isValid())
    {
        *toRet=m_model->index(index.row(),0).data().toString();
        this->close();
    }
}
