#include "formpayments.h"
#include "ui_formpayments.h"

FormPayments::FormPayments(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formpayments)
{
    ui->setupUi(this);
}

FormPayments::~FormPayments()
{
    delete ui;
}
