#ifndef DIALOGCONNESIONEDB_H
#define DIALOGCONNESIONEDB_H

#include <QDialog>

namespace Ui {
class dialogConnesioneDb;
}

class dialogConnesioneDb : public QDialog
{
    Q_OBJECT

public:
    explicit dialogConnesioneDb(QWidget *parent = nullptr);
    ~dialogConnesioneDb();

private:
    Ui::dialogConnesioneDb *ui;
};

#endif // DIALOGCONNESIONEDB_H
