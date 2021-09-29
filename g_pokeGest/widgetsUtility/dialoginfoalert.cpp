#include "dialoginfoalert.h"
#include "ui_dialoginfoalert.h"

dialogInfoAlert::dialogInfoAlert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogInfoAlert)
{
    ui->setupUi(this);

    setStyleSheet("QLineEdit:focus { "
                  "border: 2px solid rgb(66,165,245); padding: 0 5px; border-radius: 5px;"
                  "}"
                  "QPushButton {"
                  "border: 2px solid gray; border-radius: 5px; padding: 0 8px; background: rgb(208, 214, 211);"
                  "}"
                  "QPushButton:hover {"
                  "border: 2px solid gray; border-radius: 5px; padding: 0 8px; background: rgb(194, 207, 201);"
                  "}"
                  "QPushButton:pressed { "
                  "border: 2px solid gray; border-radius: 5px; padding: 0 8px; background: rgb(179, 186, 183);"
                  "}"
                  );
}

dialogInfoAlert::~dialogInfoAlert()
{
    delete ui;
}

void dialogInfoAlert::init(QString titolo, QString messaggio, int tipologiaInfoAlert)
{
    this->setWindowTitle(titolo);

    ui->label_mainText->setText(messaggio);

    QString image="";
    if(tipologiaInfoAlert==1)
        image="pikaFelice.png";
    if(tipologiaInfoAlert==2)
        image="pikaScocciato.png";
    QString testoImage=QString(":/%1").arg(image);

    QPixmap immagine(testoImage);
    QLabel *imageLabel = new QLabel();
    imageLabel->setPixmap(immagine);
    ui->layout_mid->addWidget(imageLabel);

    connect(ui->b_ok, &QPushButton::clicked, this, [=]()
    {
        this->close();
    });
}
