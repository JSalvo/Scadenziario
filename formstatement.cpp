#include "formstatement.h"
#include "ui_formstatement.h"

FormStatement::FormStatement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormStatement)
{
    ui->setupUi(this);
    this->filter = "1";
    this->ui->frame_incomingoutcoming->hide();
}

FormStatement::~FormStatement()
{
    delete ui;
}


void FormStatement::addRecord()
{
    QAbstractItemModel *model = this->ui->tableView_statement->model();

    if(model)
    {
        ((QSqlRelationalTableModel *) model)->submitAll();
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    query.prepare("INSERT INTO statements(deadline, description, value, inout, id_company) VALUES(:deadline, '', 0.0, 0, 0)");

    query.bindValue(":deadline", QDate::currentDate().toString("yyyy-MM-dd"));

    if (query.exec())
    {
        loadStatementsFromDb();
    }
    else
    {
        qDebug() << "Impossibile inserire un nuovo movimento" << query.lastError().text();
    }
    }

}



void FormStatement::loadStatementsFromDb()
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this, db);
    QAbstractItemModel *oldModel;
    DateDelegate *customDateDelegate = new DateDelegate();


    model->setTable("statements");
    model->setRelation(4, QSqlRelation("statementtypes", "id", "value"));
    model->setRelation(5, QSqlRelation("persons", "id", "name"));
    model->setRelation(6, QSqlRelation("yesno", "id", "value"));
    model->setSort(1, Qt::AscendingOrder);
    model->setFilter(this->filter);
    model->select();
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);



    oldModel = this->ui->tableView_statement->model();
    this->ui->tableView_statement->setModel(model);



    if (oldModel)
    {
        delete oldModel;
    }

    this->ui->tableView_statement->setColumnHidden(0, true);
    this->ui->tableView_statement->setItemDelegate(new QSqlRelationalDelegate());
    this->ui->tableView_statement->setItemDelegateForColumn(1, customDateDelegate);

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Scadenza")); // Scadenza, Descrizione, Valore,
    //tipo, riferimento, pagata

    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Descrizione"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Valore"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tipo"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Riferimento"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Pagato/Incassato"));

    /*
        Scadenza
        Descrizione
        Valore
        Tipo
        Riferimento
        Pagato/Incassato
    */


   // this->ui->visualizza_fatture->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->ui->tableView_statement->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    this->ui->tableView_statement->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);



    this->ui->tableView_statement->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->tableView_statement->setSelectionBehavior(QAbstractItemView::SelectRows);

}

void FormStatement::checkStatement()
{
    QAbstractItemModel *m = this->ui->tableView_statement->model();

    if (m)
    {
        QSqlRelationalTableModel * model = (QSqlRelationalTableModel *) m;

        for (int i=0; i<model->rowCount(); i++)
        {
            QModelIndex idxPayd = model->index(i, 6);
            QModelIndex idxDeadline = model->index(i, 1);



            QDate deadline = QDate::fromString(model->data(idxDeadline).toString(), "yyyy-MM-dd");
            QDate today = QDate::currentDate();


            if (today > deadline)
            {
                qDebug() << model->data(idxPayd).toString();

                if (model->data(idxPayd).toString().compare("No") == 0)
                {
                    QMessageBox msg;

                    msg.setText("Attenzione, il pagamento in data " + deadline.toString("dd-MM-yyyy") + " non è stato effettuato");
                    msg.exec();
                }
            }
        }
    }

    QMessageBox msg;
    msg.setText("Controllo sui pagamenti effettuato");
    //msg.exec();
}

void FormStatement::on_pushButton_addRecord_clicked()
{
    addRecord();
}

void FormStatement::on_pushButton_removeRecord_clicked()
{
    removeRecord();
}

void FormStatement::removeRecord()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Avviso!!!"), "Vuoi Rimuovere veramente questo record?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QAbstractItemModel *model = this->ui->tableView_statement->model();

        if (model)
        {
            ((QSqlRelationalTableModel) model).submitAll(); // ??? perchè funziona???

            if (QItemSelectionModel *selectionModel = this->ui->tableView_statement->selectionModel())
            {

                if (selectionModel->hasSelection())
                {

                    int row = selectionModel->selectedRows().at(0).row();
                    int idRow = model->data(model->index(row, 0)).toInt();

                    /*
                    qDebug() << "Riga: " << QString::number(row) << " idRow: " << QString::number(idRow);*/

                    model->removeRow(row);

                    if (!((QSqlRelationalTableModel *) model)->submitAll())
                    {
                        qDebug() << "Non è stato possibile eliminare il record!!!";
                    }
                }
            }
        }
    }
    else
    {

    }




}

void FormStatement::setFilter()
{
    this->filter = "1";

    QAbstractItemModel *model = this->ui->tableView_statement->model();


    if (model)
    {
        ((QSqlRelationalTableModel *) model)->submitAll();
        this->loadStatementsFromDb();

    }
}

void FormStatement::setFilter(int year, int month)
{
    QString y = "CAST(strftime('%Y', deadline) as integer)= ";
    QString m = "CAST(strftime('%m', deadline) as integer)= ";

    this->filter = y.append(QString::number(year)).append(" and ").append(m).append(QString::number(month));

    qDebug() << this->filter;

    QAbstractItemModel *model = this->ui->tableView_statement->model();

    if (model)
    {
        ((QSqlRelationalTableModel *) model)->submitAll();
        this->loadStatementsFromDb();

        float incoming = getIncoming(year, month);
        float outcoming = getOutcoming(year, month);
        float total = incoming - outcoming;

        qDebug() << incoming << " " << outcoming;

        this->ui->lineEdit_incoming->setText(QString::number(incoming));
        this->ui->lineEdit_outcoming->setText(QString::number(outcoming));
        this->ui->lineEdit_total->setText(QString::number(total));

    }
}

void FormStatement::setFilter(int pastfuture)
{
    QDate currentDate = QDate::currentDate();
    QDate pd(currentDate.year(), currentDate.month(), 1);
    QDate fd(currentDate.year(), currentDate.month(), 1);

    pd = pd.addDays(-1);
    fd = fd.addMonths(1);

    if (pastfuture == 0)
    {
        QString j = "CAST(strftime('%J', deadline) as integer)<= ";

        this->filter = j.append(QString::number(fd.toJulianDay()));

        qDebug() << this->filter;

        QAbstractItemModel *model = this->ui->tableView_statement->model();

        if (model)
        {
            ((QSqlRelationalTableModel *) model)->submitAll();
            this->loadStatementsFromDb();
        }

    }
    else
    {
        QString j = "CAST(strftime('%J', deadline) as integer)>= ";


        this->filter = j.append(QString::number(pd.toJulianDay()));

        qDebug() << this->filter;

        QAbstractItemModel *model = this->ui->tableView_statement->model();

        if (model)
        {
            ((QSqlRelationalTableModel *) model)->submitAll();
            this->loadStatementsFromDb();
        }

    }
}

float FormStatement::getIncoming(int y, int m)
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    QString squery =
            "SELECT "
                "SUM(value) as incoming "
            "FROM "
                "statements "
            "WHERE "
                "inout=1 AND "
                "CAST(strftime('%Y', deadline) as integer)=:y AND "
                "CAST(strftime('%m', deadline) as integer)=:m";

    query.prepare(squery);
    query.bindValue(":y", y);
    query.bindValue(":m", m);

    if (query.exec())
    {
        query.next();

        return (query.value(0).toFloat());

    }
    else
        qDebug() << "Non riesco ad ottenere i totali dalla tabella statements" << query.lastError().text();

    return (0.0);
}

float FormStatement::getOutcoming(int y, int m)
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    QString squery =
            "SELECT "
                "SUM(value) as incoming "
            "FROM "
                "statements "
            "WHERE "
                "inout=0 AND "
                "CAST(strftime('%Y', deadline) as integer)=:y AND "
                "CAST(strftime('%m', deadline) as integer)=:m";

    query.prepare(squery);
    query.bindValue(":y", y);
    query.bindValue(":m", m);

    if (query.exec())
    {
        query.next();

        return (query.value(0).toFloat());

    }
    else
        qDebug() << "Non riesco ad ottenere i totali dalla tabella statements";

    return (0.0);

}

float FormStatement::getIncomingPresentPast()
{

}

float FormStatement::getOutcomingPresentPast()
{

}

float FormStatement::getIncomingPresentFuture()
{

}

float FormStatement::getOutcomingPresentFuture()
{

}

void FormStatement::buildDataForGraph(QDate startDate, float balance)
{
    QAbstractItemModel *m = this->ui->tableView_statement->model();

    if (m)
    {
        QSqlTableModel *model = (QSqlTableModel *)  m;
        QVector<float> y(model->rowCount());
        QVector<int> x(model->rowCount());


        for (int i; i < model->rowCount(); i++)
        {
            QDate deadLine = model->data(model->index(i, 0)).toDate();
            QString description = model->data(model->index(i, 1)).toString();
            float value = model->data(model->index(i, 2)).toFloat();
            QString type = model->data(model->index(i, 3)).toString();
            QString payed = model->data(model->index(i, 4)).toString();

            balance += value;

            y[i] = balance;
            x[i] = i;
        }


    }


    /*
        Scadenza
        Descrizione
        Valore
        Tipo
        Riferimento
        Pagato/Incassato
    */


}

void FormStatement::on_pushButton_check_clicked()
{
    this->checkStatement();
}

void FormStatement::on_pushButton_save_clicked()
{
    QAbstractItemModel *model = this->ui->tableView_statement->model();
    QMessageBox msg;

    if (model)
    {
        ((QSqlRelationalTableModel *) model)->submitAll();


        this->loadStatementsFromDb();

        msg.setText("Salvataggio avvenuto con successo!!!");

    }
    else
        msg.setText("Non è stato possibile salvare le modifiche apportate!!!");


     msg.exec();
}



void FormStatement::on_comboBox_month_activated(int index)
{
    if (this->ui->radioButton_filterYearMonth->isChecked())
    {
        int y = this->ui->spinBox_year->value();

        setFilter(y, index+1);
    }
}

void FormStatement::on_spinBox_valueChanged(int arg1)
{

}

void FormStatement::on_radioButton_filterYearMonth_toggled(bool checked)
{
    if (checked)
    {
        int y = this->ui->spinBox_year->value();
        int m = this->ui->comboBox_month->currentIndex() + 1;

        setFilter(y, m);
        this->ui->frame_incomingoutcoming->show();

    }

}

void FormStatement::on_spinBox_year_valueChanged(int arg1)
{
    if (this->ui->radioButton_filterYearMonth->isChecked())
    {
        int m = this->ui->comboBox_month->currentIndex() + 1;

        setFilter(arg1, m);
    }
}

void FormStatement::on_radioButton_filterAll_clicked(bool checked)
{
    if (checked)
    {
        setFilter();
        this->ui->frame_incomingoutcoming->hide();

    }
}

void FormStatement::on_radioButton_filterPresentFuture_toggled(bool checked)
{
    if (checked)
    {
        this->ui->frame_incomingoutcoming->hide();
        this->setFilter(1);
    }
}

void FormStatement::on_radioButton_filterPresentPast_toggled(bool checked)
{
    if (checked)
    {
        this->ui->frame_incomingoutcoming->hide();
        this->setFilter(0);
    }

}
