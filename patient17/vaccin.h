#ifndef VACCIN_H
#define VACCIN_H

#include <QDialog>

namespace Ui {
class vaccin;
}

class vaccin : public QDialog
{
    Q_OBJECT

public:
    explicit vaccin(QWidget *parent = nullptr);
    ~vaccin();

private:
    Ui::vaccin *ui;
};

#endif // VACCIN_H
