#include "vaccin.h"
#include "ui_vaccin.h"

vaccin::vaccin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::vaccin)
{
    ui->setupUi(this);
}

vaccin::~vaccin()
{
    delete ui;
}
