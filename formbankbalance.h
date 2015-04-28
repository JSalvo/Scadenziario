#ifndef FORMBANKBALANCE_H
#define FORMBANKBALANCE_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class FormBankBalance;
}

class FormBankBalance : public QDialog
{
    Q_OBJECT

public:
    explicit FormBankBalance(QWidget *parent = 0);
    ~FormBankBalance();

    void loadFromDb();


private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::FormBankBalance *ui;

    void saveToDb();

};

#endif // FORMBANKBALANCE_H
