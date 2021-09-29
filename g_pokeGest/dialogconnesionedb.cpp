#include "dialogconnesionedb.h"
#include "ui_dialogconnesionedb.h"

dialogConnesioneDb::dialogConnesioneDb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogConnesioneDb)
{
    ui->setupUi(this);
}

dialogConnesioneDb::~dialogConnesioneDb()
{
    delete ui;
}
