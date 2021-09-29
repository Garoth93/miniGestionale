#ifndef SCREENDETTAGLIODOCUMENTI_H
#define SCREENDETTAGLIODOCUMENTI_H

#include <QWidget>
#include "../widgetsUtility/screenbase.h"
#include "../backEndUtility/tabellatestatadocumento.h"
#include "../backEndUtility/tabellarigadocumento.h"
#include "../backEndUtility/tabelladeposito.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QMenu>
#include "../widgetsUtility/screentabvab.h"

/*colonne nella main view*/
enum posColonneDettaglioDoc
{
    detDoc_idTestata = 0,
    detDoc_codiceArticolo = 1,
    detDoc_descrizione = 2,
    detDoc_tipoDoc = 3,
    detDoc_prezzo = 4,
    detDoc_note = 5,
    DETDOCMAXCOLUMNVAL=detDoc_note
};

/*se ci sono colonne da nascondere*/
static QList<int> m_colonneNascosteDettaglioDoc;

namespace Ui {
class screenDettaglioDocumenti;
}

class screenDettaglioDocumenti : public screenBase
{
    Q_OBJECT

public:
    explicit screenDettaglioDocumenti(QWidget *parent = nullptr, dataBaseConnection * m_dataBase=nullptr);
    ~screenDettaglioDocumenti();

    virtual void init();

    virtual void creaMenu();

    virtual void preparaInsertiNewDoc();

    virtual void pressioneConferma();

    virtual bool controlloCampiPrimaDiInsUp();

    virtual void resetCampi();

    virtual void impostaViewConStato();

    virtual void setModifica();

    virtual void setInserimento();

    virtual void creaModello();

    virtual void popolaDettaglioDatoId(QString id);

    virtual bool oggetoInDeposito(QString cdart);

    virtual void pressioneElimina();

    virtual void coloraRigheDoc();

    tabellaTestataDocumento *oggTesDoc() const;
    void setOggTesDoc(tabellaTestataDocumento *oggTesDoc);

public slots:
    virtual void pressioneAggRigaDettaglio();
    virtual void pressioneDelRigaDettaglio();
    virtual void calcolaPrezziAlVariareDelModello(QModelIndex a ,QModelIndex b);
    void menuGestioneTipoDoc(QPoint pos);

    /** gestore di click nella tabview tv_main*/
    void clickCellaTv_main(const QModelIndex &index);

private:
    Ui::screenDettaglioDocumenti *ui;
    tabellaTestataDocumento * m_oggTesDoc;
    QStandardItemModel * m_model;
    QTableView * m_tableView;

    screenTabvab * screenGestioneTabVab;
    QMenu *menu;
    QAction * menuGestTipoDoc;
};

#endif // SCREENDETTAGLIODOCUMENTI_H
