#ifndef DIALOGINFOALERT_H
#define DIALOGINFOALERT_H

#include <QDialog>

namespace Ui {
class dialogInfoAlert;
}

class dialogInfoAlert : public QDialog
{
    Q_OBJECT

public:
    explicit dialogInfoAlert(QWidget *parent = nullptr);
    ~dialogInfoAlert();

    virtual void init(QString titolo,QString messaggio, int tipologiaInfoAlert);

private:
    Ui::dialogInfoAlert *ui;
};

#endif // DIALOGINFOALERT_H
