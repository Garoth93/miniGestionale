#ifndef DIALOGERRORI_H
#define DIALOGERRORI_H

#include <QDialog>

namespace Ui {
class dialogErrori;
}

class dialogErrori : public QDialog
{
    Q_OBJECT

public:
    explicit dialogErrori(QWidget *parent = nullptr,QString testoErrore="");
    ~dialogErrori();

private:
    Ui::dialogErrori *ui;
    QString testo="";
};

#endif // DIALOGERRORI_H
