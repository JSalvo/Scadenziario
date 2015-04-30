#include "formbankbalance.h"
#include "ui_formbankbalance.h"

/*
    Questo form, visualizza il saldo bancario con la relativa data di riferimento.
*/


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

// Recupero i dati dal database e li rappresento nel form
void FormBankBalance::loadFromDb()
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    // Memorizzo il saldo bancario in un solo record della tabella bankbalance
    query.prepare("SELECT date, value FROM bankbalance WHERE id=0");

    if (query.exec())
    {
        if (query.next())
        {
            QDate date = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
            float value = query.value(1).toFloat();

            this->ui->calendarWidget->setSelectedDate(date);
            this->ui->doubleSpinBox_bankBalance->setValue(value);
        }
    }
    else
    {
        QMessageBox msg;

        msg.setText("Non è stato possibile ottenere il saldo bancario");
        msg.exec();

        qDebug() << __FILE__ << " " << __LINE__ << " " << "Impossibile ottenere il saldo bancario";
    }
}

// Trasferisco i dati dai componenti del form al database
void FormBankBalance::saveToDb()
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    query.prepare("UPDATE bankbalance SET date=:date, value=:value WHERE id=0");
    query.bindValue(":date", this->ui->calendarWidget->selectedDate().toString("yyyy-MM-dd"));
    query.bindValue(":value", this->ui->doubleSpinBox_bankBalance->value());

    if (query.exec())
    {
        QMessageBox msg;

        msg.setText("Saldo bancario aggiornato");
        msg.exec();
    }
    else
    {
        QMessageBox msg;

        msg.setText("Non è stato possibile aggiornare il saldo bancario");
        msg.exec();

        qDebug() << __FILE__ << " " << __LINE__ << " " << "Impossibile aggiornare il saldo bancario";

    }
}



void FormBankBalance::on_pushButton_save_clicked()
{
    saveToDb();
}

void FormBankBalance::on_pushButton_cancel_clicked()
{
    this->hide();
}

void FormBankBalance::on_pushButton_clicked()
{

}

void FormBankBalance::on_pushButton_close_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Avviso!!!"), "Eventuali modifiche non verranno salvate",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->hide();
    }
    else
    {

    }
}
