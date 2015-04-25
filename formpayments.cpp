#include "formpayments.h"
#include "ui_formpayments.h"

formpayments::formpayments(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formpayments)
{
    ui->setupUi(this);
}

formpayments::~formpayments()
{
    delete ui;
}
