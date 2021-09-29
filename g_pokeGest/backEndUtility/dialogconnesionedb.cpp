#include "dialogconnesionedb.h"
#include "ui_dialogconnesionedb.h"

dialogConnesioneDB::dialogConnesioneDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogConnesioneDB)
{
    ui->setupUi(this);
}

dialogConnesioneDB::~dialogConnesioneDB()
{
    delete ui;
}
