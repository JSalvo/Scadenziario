#include "formfincancialplanner.h"
#include "ui_formfincancialplanner.h"

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>

FormFincancialPlanner::FormFincancialPlanner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormFincancialPlanner)
{
    ui->setupUi(this);
    this->filter = "1";
}

FormFincancialPlanner::~FormFincancialPlanner()
{
    delete ui;
}

void FormFincancialPlanner::loadFinancialPlanningFromDb()
{
    QDate currentdate = QDate::currentDate();
    QDate rd(currentdate.year(), currentdate.month(), 1);

    rd = rd.addDays(-1);
    qDebug() << rd.toString("yyyy-MM-dd");

    this->filter = QString("CAST(strftime('%J', yearmonth) as integer) >= ").append(QString::number(rd.toJulianDay()));
    qDebug() << this->filter;

    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this, db);
    QAbstractItemModel *oldModel;

    model->setTable("financialplanner");
    model->setRelation(1, QSqlRelation("monthname", "id", "name"));
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setFilter(this->filter);
    model->sort(4, Qt::AscendingOrder);


    oldModel = this->ui->tableView_planning->model();
    this->ui->tableView_planning->setModel(model);


    model->select();

    if (oldModel)
    {
        delete oldModel;
    }

    this->ui->tableView_planning->setItemDelegate(new QSqlRelationalDelegate());



    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Anno"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Mese"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Valore Pianificato"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Valore Atteso"));

    this->ui->tableView_planning->setColumnHidden(4, true);
    this->ui->tableView_planning->setColumnHidden(5, true);

    this->ui->tableView_planning->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //this->ui->tableView_planning->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);


    this->ui->tableView_planning->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->tableView_planning->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void FormFincancialPlanner::computeMonthlyPlanning(int year, int month)
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db), query2(db);

    query.prepare("SELECT valueplanned, value FROM financialplanner WHERE year=:year and month=:month");
    query.bindValue(":year", year);
    query.bindValue(":month", month);

    if (query.exec())
    {
        query2.prepare("SELECT invalue, outvalue, tot FROM monthlybalance WHERE year=:year and month=:month");
        query2.bindValue(":year", year);
        query2.bindValue(":month", month);

      //  if (query.size() == 0 || query.size() == 1)
       // {
            if (query2.exec())
            {
                if (query2.next())
                {
                    float valueplanned;

                    if (query2.value(2).isNull())
                    {
                        if(query2.value(1).isNull())
                            valueplanned = query2.value(0).toFloat();
                        else
                            valueplanned = query2.value(1).toFloat();

                    }
                    else
                        valueplanned = query2.value(2).toFloat();

                    if (query.next())
                    {
                        query.prepare("UPDATE financialplanner SET valueplanned=:valueplanned WHERE year=:year and month=:month");
                        query.bindValue(":year", year);
                        query.bindValue(":month", month);
                        query.bindValue(":valueplanned", valueplanned);

                        if (!query.exec())
                        {
                            qDebug() << "Impossibile eseguire la query: " << query.lastError().text() << query.lastQuery();
                        }
                    }
                    else
                    {

                        query.prepare("INSERT INTO financialplanner(year, month, valueplanned, value, yearmonth) VALUES(:year, :month, :valueplanned, :value, :yearmonth)");

                        query.bindValue(":year", year);
                        query.bindValue(":month", month);
                        query.bindValue(":valueplanned", valueplanned);
                        query.bindValue(":value", valueplanned);
                        query.bindValue(":yearmonth", QDate(year, month, 1).toString("yyyy-MM-dd"));

                        if (!query.exec())
                        {
                            qDebug() << "Impossibile creare la nuova pianificazione: " << query.lastError().text();

                        }
                    }
                }
                else
                {
                    qDebug() << "Dimensione del risultato della query non attesto: " << query.lastQuery();
                    qDebug() << "year: " << QString::number(year) << " month: " << QString::number(month);
                }
            }
            else
            {
                qDebug() << "Impossibile eseguire la query: " << query2.lastError().text();
            }
     /*   }
       else
        {
               qDebug() << "La dimensione del risultato della query è anomalo: " << query.lastQuery();
               qDebug() << "year: " << QString::number(year) << " month: " << QString::number(month);
               qDebug() << "Query size: " << QString::number(query.size());
        }*/

    }
    else
    {
        qDebug() << "Impossibile eseguire la query: " << query.lastError().text();

    }

    loadFinancialPlanningFromDb();
}

void FormFincancialPlanner::computeMonthlyAvailability(int year, int month)
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db), query2(db);

    query.prepare("SELECT valueplanned, value FROM financialplanner WHERE year=:year and month=:month");
    query.bindValue(":year", year);
    query.bindValue(":month", month);

    if (query.exec())
    {
        query2.prepare("SELECT invalue, outvalue, tot FROM monthlybalance WHERE year=:year and month=:month");
        query2.bindValue(":year", year);
        query2.bindValue(":month", month);

      //  if (query.size() == 0 || query.size() == 1)
       // {
            if (query2.exec())
            {
                if (query2.next())
                {
                    float valueplanned;

                    if (query2.value(2).isNull())
                    {
                        if(query2.value(1).isNull())
                            valueplanned = query2.value(0).toFloat();
                        else
                            valueplanned = query2.value(1).toFloat();

                    }
                    else
                        valueplanned = query2.value(2).toFloat();

                    if (query.next())
                    {
                        query.prepare("UPDATE financialplanner SET value=:valueplanned WHERE year=:year and month=:month");
                        query.bindValue(":year", year);
                        query.bindValue(":month", month);
                        query.bindValue(":valueplanned", valueplanned);

                        if (!query.exec())
                        {
                            qDebug() << "Impossibile eseguire la query: " << query.lastError().text() << query.lastQuery();
                        }
                    }
                    else
                    {

                        query.prepare("INSERT INTO financialplanner(year, month, valueplanned, value, yearmonth) VALUES(:year, :month, :valueplanned, :value, :yearmonth)");

                        query.bindValue(":year", year);
                        query.bindValue(":month", month);
                        query.bindValue(":valueplanned", valueplanned);
                        query.bindValue(":value", valueplanned);
                        query.bindValue(":yearmonth", QDate(year, month, 1).toString("yyyy-MM-dd"));



                        if (!query.exec())
                        {
                            qDebug() << "Impossibile creare la nuova pianificazione: " << query.lastError().text();

                        }
                    }
                }
                else
                {
                    qDebug() << "Dimensione del risultato della query non attesto: " << query.lastQuery();
                    qDebug() << "year: " << QString::number(year) << " month: " << QString::number(month);
                }
            }
            else
            {
                qDebug() << "Impossibile eseguire la query: " << query2.lastError().text();
            }
     /*   }
       else
        {
               qDebug() << "La dimensione del risultato della query è anomalo: " << query.lastQuery();
               qDebug() << "year: " << QString::number(year) << " month: " << QString::number(month);
               qDebug() << "Query size: " << QString::number(query.size());
        }*/

    }
    else
    {
        qDebug() << "Impossibile eseguire la query: " << query.lastError().text();

    }

    loadFinancialPlanningFromDb();
}

void FormFincancialPlanner::computeAllMonthlyAvailability()
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    query.prepare("SELECT year, month FROM monthlybalance");

    if (query.exec())
    {
        while(query.next())
        {
            int year = query.value(0).toInt();
            int month = query.value(1).toInt();

            computeMonthlyAvailability(year, month);
        }
    }
    else
    {
        qDebug() << "Impossibile eseguire la query: " << query.lastQuery();
    }

    checkPlanning();
}

void FormFincancialPlanner::checkPlanning()
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    query.prepare("SELECT year, month, valueplanned, value FROM financialplanner ORDER BY year, month");

    if (query.exec())
    {
        while (query.next())
        {
            if (query.value(3).toFloat() < query.value(2).toFloat())
            {
                int year = query.value(0).toInt();
                int month = query.value(1).toInt();

                QMessageBox msg;

                msg.setText("ATTENZIONE!!! Per il mese " + QString::number(month) + " dell'anno " + QString::number(year) + " le entrate attese sono inferiori alle entrate pianificate!");
                msg.exec();

            }
            else
            {
                qDebug() << QString::number(query.value(3).toFloat()) << " " << QString::number(query.value(2).toFloat());

            }

        }

    }
    else
    {
        qDebug() << query.lastError().text() << query.lastQuery();

    }

    QMessageBox msg;

    msg.setText("Controllo sulla pianificazione eseguito con successo");
    msg.exec();

}

void FormFincancialPlanner::on_pushButton_clicked()
{
    this->loadFinancialPlanningFromDb();
}

void FormFincancialPlanner::on_pushButton_planning_clicked()
{
    this->computeMonthlyPlanning(this->ui->spinBox_year->value(), this->ui->comboBox_month->currentIndex() + 1);
}

void FormFincancialPlanner::on_pushButton_effectiveAvailability_clicked()
{
    this->computeMonthlyAvailability(this->ui->spinBox_year->value(), this->ui->comboBox_month->currentIndex() +1);
}

void FormFincancialPlanner::on_pushButton_checkPlanning_clicked()
{
    this->checkPlanning();
}

void FormFincancialPlanner::on_pushButton_delete_clicked()
{
     QAbstractItemModel *m = this->ui->tableView_planning->model();
     QItemSelectionModel *selection = this->ui->tableView_planning->selectionModel();
     QMessageBox msg;

     if (selection)
     {
        if (selection->selectedRows().count() > 0)
        {
            QSqlRelationalTableModel * model = (QSqlRelationalTableModel *) m;
            int row = selection->selectedRows().at(0).row();
            qDebug() << row;



            if (model->removeRow(row))
            {
                if (!model->submitAll())
                {
                    msg.setText(tr("Non è stato possibile rimuovere il record di pianificazione"));
                    qDebug() << model->lastError().text();
                }
                else
                    msg.setText(tr("Il record di pianificazione è stato rimosso"));
            }
            else
                msg.setText("Model.removerow non funziona");
        }
        else
            msg.setText(tr("E' necessario selezionare un elemento"));
     }
     else
     {
         msg.setText(tr("E' necessario selezionare un elemento-2"));
     }

     msg.exec();
}
