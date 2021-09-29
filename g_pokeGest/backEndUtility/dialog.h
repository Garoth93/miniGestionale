#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    virtual void init(QString * hostName,
                      QString * databaseName,
                      QString * utente,
                      QString * password,
                      int * porta);

private:
    Ui::Dialog *ui;
    QString *m_hostName;
    QString *m_dataBaseName;
    QString *m_utente;
    QString *m_password;
    int *m_porta;
};

#endif // DIALOG_H
