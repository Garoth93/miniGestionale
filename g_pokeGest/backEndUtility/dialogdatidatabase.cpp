#include "dialogdatidatabase.h"
#include "ui_dialogdatidatabase.h"
#include "dialogerrori.h"
#include "../widgetsUtility/dialoginfoalert.h"
#include <QTcpSocket>
#include <QUrl>
#include <QProcess>
#include <QJsonDocument>
#include <QDir>
#include <QJsonObject>

dialogDatiDatabase::dialogDatiDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogDatiDatabase)
{
    ui->setupUi(this);

    setStyleSheet("QLineEdit { "
                        "border: 2px solid rgb(194, 207, 201); padding: 0 5px; border-radius: 5px;"
                    "}"
                    "QLineEdit:focus { "
                        "border: 2px solid rgb(66,165,245); padding: 0 5px; border-radius: 5px;"
                    "}"
                    "QLineEdit:hover {"
                        "background: rgb(220, 247, 246); border: none; border-radius: 5px;"
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

dialogDatiDatabase::~dialogDatiDatabase()
{
    delete ui;
}

void dialogDatiDatabase::init(QSqlDatabase * db,
                              QString *hostName,
                              QString *databaseName,
                              QString *utente,
                              QString *password,
                              int *porta,
                              bool * isOk)
{
    this->setWindowTitle("Imposta i dati di connessione al database");
    m_dataBase=db;
    m_hostName=hostName;
    m_dataBaseName=databaseName;
    m_utente=utente;
    m_password=password;
    m_porta=porta;

    ui->l_password->setEchoMode(QLineEdit::Password);

    caricaDatiInizializzazione();

    ui->l_host->setText(*m_hostName);
    ui->l_databaseName->setText(*m_dataBaseName);
    ui->l_porta->setText(QString::number(*m_porta));
    ui->l_utente->setText(*utente);

    qDebug() << *m_porta;

    connect(ui->b_esci_2, &QPushButton::clicked, this, [=]()
    {
        *isOk=false;
        this->close();
    });

    connect(ui->b_salva, &QPushButton::clicked, this, [=]()
    {
        salvaDatiInizializzazione();
    });

    connect(ui->b_conferma_2, &QPushButton::clicked, this, [=]()
    {
        *m_hostName=ui->l_host->text();
        *m_dataBaseName=ui->l_databaseName->text();
        *m_utente=ui->l_utente->text();
        *m_password=ui->l_password->text();
        *m_porta=ui->l_porta->text().toInt();

        m_dataBase->setHostName(*m_hostName);
        m_dataBase->setDatabaseName(*m_dataBaseName);
        m_dataBase->setUserName(*m_utente);
        m_dataBase->setPassword(*m_password);
        m_dataBase->setPort(*m_porta);

        if(!urlExists(ui->l_databaseName->text()))
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Database non raggiungibile",1);
            tmpDialog.exec();
            return;
        }
        bool ok = m_dataBase->open(*m_utente,*m_password);

        if(ok)
        {
            *isOk=true;
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Tutto ok","Connesione al db riuscita",1);
            tmpDialog.exec();
            this->close();
        }
        else
        {
            dialogInfoAlert tmpDialog;
            tmpDialog.init("Errore","Connesione al db non riuscita",2);
            tmpDialog.exec();
            *isOk=false;
        }

    });
}

bool dialogDatiDatabase::urlExists(QString url_string)
{
    QStringList parameters;
    parameters << "-n" << "1";
    parameters << url_string;
    int exitCode = QProcess::execute("ping", parameters);
       if (exitCode==0) {
           return true;
       } else {
           return false;
       }
}

void dialogDatiDatabase::caricaDatiInizializzazione()
{
    QDir provaDir;
    QString pathFileConf=QDir::rootPath();
    pathFileConf=QString("%1pokeBreFoo/").arg(pathFileConf);
    if(!QDir(pathFileConf).exists()) return;

    qDebug() << QString("%1fileConfLog.json").arg(pathFileConf);
    QFile m_fileJson(QString("%1fileConfLog.json").arg(pathFileConf));
    if( !m_fileJson.open( QIODevice::ReadWrite ) )
    {
        return;
    }

    //il file json ricevuto lo trasformo in jsonDocument
    QJsonDocument jsonOrg = QJsonDocument::fromJson( m_fileJson.readAll() );

    *m_hostName=jsonOrg.object().value("hostName").toString();
    *m_dataBaseName=jsonOrg.object().value("dbName").toString();
    *m_utente=jsonOrg.object().value("utente").toString();
    *m_porta=jsonOrg.object().value("porta").toInt();

//    QByteArray rawData = m_fileJson.readAll();
//    QJsonDocument doc2(QJsonDocument::fromJson(rawData));
//    QJsonObject json = doc.object();
//    foreach(const QString& key, json.keys()) {
//        QJsonValue value = json.value(key);
//        qDebug() << "Key = " << key << ", Value = " << value.toString();
//    }
    m_fileJson.close();
}

void dialogDatiDatabase::salvaDatiInizializzazione()
{
    QDir provaDir;
    QString pathFileConf=QDir::rootPath();
    pathFileConf=QString("%1pokeBreFoo").arg(pathFileConf);
    if(!QDir(pathFileConf).exists())
    {
        qDebug() << pathFileConf;
        QDir().mkdir(pathFileConf);
    }

    QJsonObject objToSabe;
    objToSabe["hostName"]=ui->l_host->text();
    objToSabe["dbName"]=ui->l_databaseName->text();
    objToSabe["utente"]=ui->l_utente->text();
    objToSabe["porta"]=ui->l_porta->text().toInt();

    //creo il documento per poi scriverlo
    QJsonDocument doc( objToSabe );

    QFile m_fileJson(QString("%1/fileConfLog.json").arg(pathFileConf));
    //apro json
    if( !m_fileJson.open( QIODevice::WriteOnly ) )
    {
        //errore in apertura
        return;
    }

    //scrivo il documento nel file json
    m_fileJson.write(doc.toJson());
    m_fileJson.close();
}
