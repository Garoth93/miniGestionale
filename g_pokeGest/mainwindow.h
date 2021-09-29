#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "./backEndUtility/databaseconnection.h"
#include "./anagraficaArticoli/screenmainartica.h"
#include "./magazzino/screenmaindeposito.h"
#include "./documenti/screendettagliodocumenti.h"
#include "./documenti/screenesploradocumenti.h"
#include <QStandardItemModel>
#include <QTableView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void creaMenu();

private:
    Ui::MainWindow *ui;
    //QSqlDatabase * m_dataBase;
    dataBaseConnection * m_dataBase;
    screenMainArtica * p_artica;
    screenMainDeposito * p_magazzino;
    screenDettaglioDocumenti * p_documenti;
    screenEsploraDocumenti * p_esploraDoc;
    QTableView * m_viewMenu;
    QStandardItemModel * m_modelMenu;

public slots:
    void doppioClickMenu(const QModelIndex &index);

    void anagraficheToCsv();
    int getGiacenzaDatoCdart(QString cdart);

};
#endif // MAINWINDOW_H
