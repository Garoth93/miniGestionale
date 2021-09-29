#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::init(QString *hostName, QString *databaseName, QString *utente, QString *password, int *porta)
{
    m_hostName=hostName;
    m_dataBaseName=databaseName;
    m_utente=utente;
    m_password=password;
    m_porta=porta;
}
