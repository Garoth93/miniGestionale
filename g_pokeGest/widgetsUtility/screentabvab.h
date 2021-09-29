#ifndef SCREENTABVAB_H
#define SCREENTABVAB_H

#include <QWidget>
#include "screenbase.h"
#include "../backEndUtility/tabellatabvab.h"
#include <QStandardItemModel>
#include <QTableView>


namespace Ui {
class screenTabvab;
}

class screenTabvab : public screenBase
{
    Q_OBJECT

public:
    explicit screenTabvab(QWidget *parent = nullptr, dataBaseConnection * m_dataBase=nullptr);
    ~screenTabvab();

    virtual void init();

    virtual void creaMenu();

    virtual void impostaViewConStato();

    virtual void setVisualizza();

    virtual void setModifica();

    virtual void setInserimento();

    virtual void popolaModello();

    virtual void pressioneConferma();

    virtual bool controlloCampiPrimaDiInsUp();

    virtual void resetViewDettaglio();

    virtual void loadArticoloInView();

    virtual void attivaDisattivaDettagli();

    virtual void pressioneElimina();

public slots:
    void clickOnTableView(const QModelIndex &);

    QString getTipoTabella() const;
    void setTipoTabella(const QString &value);

private:
    Ui::screenTabvab *ui;
    QString tipoTabella="";
    tabellaTabvab * m_oggTabvab;
    QStandardItemModel * m_model;
    QTableView * m_tableView;
};

#endif // SCREENTABVAB_H
