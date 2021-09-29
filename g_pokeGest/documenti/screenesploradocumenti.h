#ifndef SCREENESPLORADOCUMENTI_H
#define SCREENESPLORADOCUMENTI_H

#include <QWidget>
#include "../widgetsUtility/screenbase.h"
#include "screendettagliodocumenti.h"
#include <QStandardItemModel>
#include <QTableView>
#include "screendettagliodocumenti.h"

namespace Ui {
class screenEsploraDocumenti;
}

class screenEsploraDocumenti : public screenBase
{
    Q_OBJECT

public:
    explicit screenEsploraDocumenti(QWidget *parent = nullptr, dataBaseConnection * m_dataBase=nullptr);
    ~screenEsploraDocumenti();

    virtual void init();

    virtual void creaMenu();

    virtual void creaModello();

    virtual void creaModelloConFiltri();

    virtual void settaLabelPrezzo(QList<int> * lista);

    virtual bool salvaFileInCsv(QString path);

public slots:
    void clickOnTableView(const QModelIndex &index);
    void settaCercaUtilizzabiliRadio();

private:
    Ui::screenEsploraDocumenti *ui;

    screenDettaglioDocumenti * m_screenDettaglio;
    QStandardItemModel * m_model;
    QTableView * m_tableView;
};

#endif // SCREENESPLORADOCUMENTI_H
