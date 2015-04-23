#include "formbankbalance.h"
#include "ui_formbankbalance.h"

FormBankBalance::FormBankBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormBankBalance)
{
    ui->setupUi(this);
}

FormBankBalance::~FormBankBalance()
{
    delete ui;
}
