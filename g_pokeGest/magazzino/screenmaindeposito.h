#ifndef SCREENMAINDEPOSITO_H
#define SCREENMAINDEPOSITO_H

#include <QWidget>
#include "../widgetsUtility/screenbase.h"
#include "../backEndUtility/tabelladeposito.h"
#include <QStandardItemModel>
#include <QTableView>

namespace Ui {
class screenMainDeposito;
}

class screenMainDeposito : public screenBase
{
    Q_OBJECT

public:
    explicit screenMainDeposito(QWidget *parent = nullptr, dataBaseConnection * m_dataBase=nullptr);
    ~screenMainDeposito();

    virtual void init();

    virtual void creaMenu();

    virtual void impostaViewConStato();

    virtual void setVisualizza();

    virtual void setModifica();

    virtual void setInserimento();

    virtual void attivaDisattivaDettagli();

    virtual void resetViewDettaglio();

    virtual void pressioneConferma();

    virtual bool controlloCampiPrimaDiInsUp();

    virtual void loadArticoloInView();

    virtual void popolaModello();

    virtual void pressioneElimina();

    virtual QString getDescrizioneArticoloDaCodice(QString codiceArt);

public slots:
    void clickOnTableView(const QModelIndex &);

private:
    Ui::screenMainDeposito *ui;
    tabellaDeposito * m_oggMagazzino;
    QStandardItemModel * m_model;
    QTableView * m_tableView;
};

#endif // SCREENMAINDEPOSITO_H
