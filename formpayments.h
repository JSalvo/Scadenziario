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

private slots:
    void on_pushButton_generateStatements_clicked();

private:
    Ui::formpayments *ui;

    QList<float> getRatioPayment(QString s);
    QList<int> getDelayForPayment(QString s);


};

#endif // FORMPAYMENTS_H
