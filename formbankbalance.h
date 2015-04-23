#ifndef FORMBANKBALANCE_H
#define FORMBANKBALANCE_H

#include <QDialog>

namespace Ui {
class FormBankBalance;
}

class FormBankBalance : public QDialog
{
    Q_OBJECT

public:
    explicit FormBankBalance(QWidget *parent = 0);
    ~FormBankBalance();

private:
    Ui::FormBankBalance *ui;
};

#endif // FORMBANKBALANCE_H
