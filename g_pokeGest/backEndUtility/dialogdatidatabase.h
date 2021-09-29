#ifndef DIALOGDATIDATABASE_H
#define DIALOGDATIDATABASE_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class dialogDatiDatabase;
}

class dialogDatiDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit dialogDatiDatabase(QWidget *parent = nullptr);
    ~dialogDatiDatabase();

    virtual void init(QSqlDatabase * db,
                      QString * hostName,
                      QString * databaseName,
                      QString * utente,
                      QString * password,
                      int * porta,
                      bool * isOk);

    virtual bool urlExists (QString url_string);

    void caricaDatiInizializzazione();
    void salvaDatiInizializzazione();

private:
    Ui::dialogDatiDatabase *ui;
    QSqlDatabase * m_dataBase;
    QString *m_hostName;
    QString *m_dataBaseName;
    QString *m_utente;
    QString *m_password;
    int *m_porta;
};

#endif // DIALOGDATIDATABASE_H
