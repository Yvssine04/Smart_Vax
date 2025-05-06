#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QDialog>

namespace Ui {
class equipement;
}

class equipement : public QDialog
{
    Q_OBJECT

public:
    explicit equipement(QWidget *parent = nullptr);
    ~equipement();

private:
    Ui::equipement *ui;
};

#endif // EQUIPEMENT_H
