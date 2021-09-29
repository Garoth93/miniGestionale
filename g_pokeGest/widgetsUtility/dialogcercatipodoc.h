#ifndef DIALOGCERCATIPODOC_H
#define DIALOGCERCATIPODOC_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include "../backEndUtility/databaseconnection.h"

namespace Ui {
class dialogCercaTipoDoc;
}

class dialogCercaTipoDoc : public QDialog
{
    Q_OBJECT

public:
    explicit dialogCercaTipoDoc(QWidget *parent = nullptr, dataBaseConnection * db=nullptr, QString * cdartBack=nullptr);
    ~dialogCercaTipoDoc();

public slots:
    void cercaParole(const QString &);

    void doppioClickConferma(const QModelIndex &);

private:
    Ui::dialogCercaTipoDoc *ui;

protected:
    dataBaseConnection * m_dataBase;

    QStandardItemModel * m_model;
    QString * toRet;
    QTableView * m_tableView;

};

#endif // DIALOGCERCATIPODOC_H
