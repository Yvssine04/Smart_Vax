#include "equipement.h"
#include "ui_equipement.h"

equipement::equipement(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::equipement)
{
    ui->setupUi(this);
}

equipement::~equipement()
{
    delete ui;
}
