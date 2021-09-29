#ifndef DIALOGCONNESIONEDB_H
#define DIALOGCONNESIONEDB_H

#include <QDialog>

namespace Ui {
class dialogConnesioneDB;
}

class dialogConnesioneDB : public QDialog
{
    Q_OBJECT

public:
    explicit dialogConnesioneDB(QWidget *parent = nullptr);
    ~dialogConnesioneDB();

private:
    Ui::dialogConnesioneDB *ui;
};

#endif // DIALOGCONNESIONEDB_H
