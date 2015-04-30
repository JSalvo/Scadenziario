#ifndef FORMPAYMENTS_H
#define FORMPAYMENTS_H

#include <QDialog>

namespace Ui {
class formpayments;
}

class FormPayments : public QDialog
{
    Q_OBJECT

public:
    explicit FormPayments(QWidget *parent = 0);
    ~FormPayments();

private:
    Ui::formpayments *ui;
};

#endif // FORMPAYMENTS_H
