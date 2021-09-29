#ifndef DIALOGCERCAARTICOLO_H
#define DIALOGCERCAARTICOLO_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include "../backEndUtility/databaseconnection.h"

namespace Ui {
class dialogCercaArticolo;
}

class dialogCercaArticolo : public QDialog
{
    Q_OBJECT

public:
    explicit dialogCercaArticolo(QWidget *parent = nullptr, dataBaseConnection * db=nullptr, QString * cdartBack=nullptr);
    ~dialogCercaArticolo();

public slots:
    void cercaParole(const QString &);

    void doppioClickConferma(const QModelIndex &);

protected:
    dataBaseConnection * m_dataBase;

    QStandardItemModel * m_model;
    QString * toRet;
    QTableView * m_tableView;


private:
    Ui::dialogCercaArticolo *ui;
};

#endif // DIALOGCERCAARTICOLO_H
