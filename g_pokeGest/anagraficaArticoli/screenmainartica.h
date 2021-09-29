#ifndef SCREENMAINARTICA_H
#define SCREENMAINARTICA_H

#include <QWidget>
#include "../widgetsUtility/screenbase.h"
#include "../backEndUtility/tabellaarticoli.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QSortFilterProxyModel>
#include "../widgetsUtility/screentabvab.h"

/*colonne nella main view*/
enum posColonneMainView
{
    p_codice = 0,
    p_descrizione = 1,
    p_espans = 2,
    MAXCOLUMNVAL=p_espans
};

/*se ci sono colonne da nascondere*/
static QList<int> m_colonneNascosteMainView;

namespace Ui {
class screenMainArtica;
}

class screenMainArtica : public screenBase
{
    Q_OBJECT

public:
    explicit screenMainArtica(QWidget *parent = nullptr,dataBaseConnection * m_dataBase=nullptr);
    ~screenMainArtica();

    virtual void init();
    virtual void impostaTitoloLabelDb();
    virtual void impostaViewConStato();
    virtual void resetViewDettaglio();
    virtual void setVisualizza();
    virtual void setModifica();
    virtual void setInserimento();
    virtual void pressioneConferma();
    virtual void attivaDisattivaDettagli();
    virtual void loadArticoloInView();
    virtual bool controlloCampiPrimaDiInsUp();
    virtual void popolaComboBox();
    virtual void pressioneElimina();

    virtual void creaModello();
    virtual void popolaModello();
    virtual void ricaricaModelloTab1();

public slots:
    virtual void tabWidgetCambio(int index);
    void doppioTableViewTab1(const QModelIndex &index);
    virtual void clickSx();
    virtual void clickDx();
    virtual void testoComboCambiato(const QString &);
protected:
    tabellaArticoli * m_oggArt;
    QTableView * m_viewTab1;
    QStandardItemModel * m_modelTab1;
    QSortFilterProxyModel * m_sortTab1;
    int m_rigaDoveSonoViewTab1=0;
    screenTabvab * screenGestioneTabVab;

private:
    Ui::screenMainArtica *ui;
};

#endif // SCREENMAINARTICA_H
