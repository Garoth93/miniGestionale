#include "dialogerrori.h"
#include "ui_dialogerrori.h"

dialogErrori::dialogErrori(QWidget *parent,QString testoErrore) :
    QDialog(parent),
    ui(new Ui::dialogErrori)
{
    ui->setupUi(this);
    testo=testoErrore;
    ui->la_text->setText(testo);

    connect(ui->b_esci, &QPushButton::clicked, this, [=]()
    {
        this->close();
    });
}

dialogErrori::~dialogErrori()
{
    delete ui;
}
