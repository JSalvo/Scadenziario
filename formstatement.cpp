#include "currencydelegate.h"
#include "formstatement.h"
#include "ui_formstatement.h"

FormStatement::FormStatement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormStatement)
{
    ui->setupUi(this);
    this->filter = "1";
    this->formBankBalance = new FormBankBalance(this);
}

FormStatement::~FormStatement()
{
    delete formBankBalance;
    delete ui;
}

// Aggiungo un record ai movimenti
void FormStatement::addRecord()
{
    QAbstractItemModel *model = this->ui->tableView_statement->model();

    if(model)
    {
        // Per prima cosa salvo tutte le modifiche pendenti
        ((QSqlRelationalTableModel *) model)->submitAll();

        QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
        QSqlQuery query(db);

        // Inserisco una nuovo record nel database statements
        query.prepare("INSERT INTO statements(deadline, description, value, inout, id_company) VALUES(:deadline, '', 0.0, 0, 0)");

        query.bindValue(":deadline", QDate::currentDate().toString("yyyy-MM-dd"));

        // Se l'inserimento va a buno fine ...
        if (query.exec())
        {
            // Ricarico nuovamente la tabella, in modo da visualizzare il nuovo record inserito
            loadStatementsFromDb();
        }
        else        
            qDebug() << __FILE__ << " " << __LINE__ << "Impossibile inserire un nuovo movimento" << query.lastError().text();
    }
}


// Carico i movimenti e li visualizzo nel QTableView
void FormStatement::loadStatementsFromDb()
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this, db);
    QAbstractItemModel *oldModel;
    DateDelegate *customDateDelegate = new DateDelegate();
    CurrencyDelegate *currencyDelegate = new CurrencyDelegate();

    model->setTable("statements");

    // Imposto i riferimenti alle altre tabelle
    model->setRelation(4, QSqlRelation("statementtypes", "id", "value"));
    model->setRelation(5, QSqlRelation("persons", "id", "name"));
    model->setRelation(6, QSqlRelation("yesno", "id", "value"));

    // Definisco l'ordinamento della tabella
    model->setSort(1, Qt::AscendingOrder);

    // Imposto un filtro per ridurre la selezione
    model->setFilter(this->filter);
    model->select();

    // Aggiornerò il database manualmente in modo da avere un maggiore controllo
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);

    // Memorizzo il vecchio model
    oldModel = this->ui->tableView_statement->model();

    // Imposto il nuovo model
    this->ui->tableView_statement->setModel(model);

    // Cancello il vecchio model
    if (oldModel)    
        delete oldModel;

    // Nascondo la colonna degli id
    this->ui->tableView_statement->setColumnHidden(0, true);

    // Imposto i delegati per le rispettive colonne
    this->ui->tableView_statement->setItemDelegate(new QSqlRelationalDelegate());
    this->ui->tableView_statement->setItemDelegateForColumn(1, customDateDelegate);
    this->ui->tableView_statement->setItemDelegateForColumn(3, currencyDelegate);
    this->ui->tableView_statement->setItemDelegateForColumn(7, currencyDelegate);

    // Imposto i nomi degli attributi della tabella
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Scadenza"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Descrizione"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Valore"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tipo"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Riferimento"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Pagato/Incassato"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Previsione Saldo C.C."));

    // Definisco come le colonne della tabella si ridimensioneranno in funzione del contenuto
    this->ui->tableView_statement->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    this->ui->tableView_statement->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    this->ui->tableView_statement->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    this->ui->tableView_statement->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);

    // Indico che la selezione sarà singola e per riga
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

    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    query.prepare("SELECT date, value FROM bankbalance WHERE id=0");

    if(query.exec())
    {
        if (query.next())
        {
            QDate bankBalanceDate = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
            float bankBalance  = query.value(1).toFloat();

            this->updateEstimatedBankBalance(bankBalanceDate,bankBalance);
        }

    }
    else
        qDebug() << __FILE__ << " " << __LINE__ << " Non posso recuperare i dati relativi al saldo bancario";

    this->on_pushButton_save_clicked();
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

                    model->removeRow(row);

                    if (!((QSqlRelationalTableModel *) model)->submitAll())                    
                        qDebug() << __FILE__ << " " << __LINE__ << "Non è stato possibile eliminare il record!!!";
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

void FormStatement::fillIncomingOutcomingTotal()
{
    float incoming = getIncoming();
    float outcoming = getOutcoming();
    float total = incoming - outcoming;
    QLocale l;

    this->ui->lineEdit_incoming->setText(l.toString(incoming, 'f', 2).append(QChar(8364)));
    this->ui->lineEdit_outcoming->setText(l.toString(outcoming, 'f', 2).append(QChar(8364)));
    this->ui->lineEdit_total->setText(l.toString(total, 'f', 2).append(QChar(8364)));
}

float FormStatement::getIncoming()
{
    QAbstractItemModel *model = this->ui->tableView_statement->model();
    float result=0;

    if (model)
    {
        QSqlTableModel *m = (QSqlTableModel *) model;

        for (int i=0; i<model->rowCount(); i++)
        {
            if (m->data(m->index(i, 4)).toString().compare("Entrata") == 0)
                result += m->data(m->index(i, 3)).toFloat();
        }
    }

    return result;
}

float FormStatement::getOutcoming()
{
    QAbstractItemModel *model = this->ui->tableView_statement->model();
    float result=0;

    if (model)
    {
        QSqlTableModel *m = (QSqlTableModel *) model;

        for (int i=0; i<model->rowCount(); i++)
        {
            if (m->data(m->index(i, 4)).toString().compare("Uscita") == 0)
                result += m->data(m->index(i, 3)).toFloat();
        }
    }
    return result;
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

void FormStatement::buildDataForGraph(float bankBalance)
{
    QAbstractItemModel *m = this->ui->tableView_statement->model();

    if (m)
    {
        QSqlTableModel *model = (QSqlTableModel *)  m;
        int limit = model->rowCount();
        QVector<double> y(limit);
        QVector<double> x(limit);
        QVector<double> yneg(limit);
        QVector<QString> labels;
        QVector<double> ticks;

        qDebug() << model->rowCount();

        for (int i=0; i < limit; i++)
        {
            QString description = model->data(model->index(i, 2)).toString();
            float estimatedBankBalance = model->data(model->index(i, 7)).toFloat();

            x[i] = i;
            ticks << (i);

            labels << description;

            if (estimatedBankBalance > 0)
            {
                y[i] = estimatedBankBalance;
                yneg[i] = 0;
            }
            else
            {
                y[i] = 0;
                yneg[i] = estimatedBankBalance;
            }
        }

        qDebug() << "Grafici rimossi" << this->ui->widget_graph->clearGraphs();
        this->ui->widget_graph->clearItems();
        this->ui->widget_graph->clearPlottables();

        this->ui->widget_graph->addGraph();

        this->ui->widget_graph->xAxis->setAutoTicks(false);
        this->ui->widget_graph->xAxis->setAutoTickLabels(false);
        this->ui->widget_graph->xAxis->setTickLabelRotation(-60);

        this->ui->widget_graph->xAxis->setLabel("Movimenti");
        this->ui->widget_graph->yAxis->setLabel("Euro");

        this->ui->widget_graph->xAxis->setTickVector(ticks);
        this->ui->widget_graph->xAxis->setTickVectorLabels(labels);
        this->ui->widget_graph->xAxis->setSubTickCount(0);

        QCPBars *bars1 = new QCPBars(this->ui->widget_graph->xAxis, this->ui->widget_graph->yAxis);
        this->ui->widget_graph->addPlottable(bars1);
        bars1->setData(x, y);
        bars1->setPen(Qt::NoPen);
        bars1->setBrush(QColor(10, 140, 70, 160));

        this->ui->widget_graph->addGraph();

        QCPBars *bars2 = new QCPBars(this->ui->widget_graph->xAxis, this->ui->widget_graph->yAxis);
        this->ui->widget_graph->addPlottable(bars2);
        bars2->setData(x, yneg);
        bars2->setPen(Qt::NoPen);
        bars2->setBrush(QColor(255, 0, 0, 160));

        this->ui->widget_graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                          QCP::iSelectLegend | QCP::iSelectPlottables);
        this->ui->widget_graph->rescaleAxes();
        this->ui->widget_graph->replot();

        qDebug() << "Numero oggetti: " << this->ui->widget_graph->itemCount();
    }
}



void FormStatement::updateEstimatedBankBalance(QDate bankBalanceDate, float bankBalance)
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);
    float bb = bankBalance;

    qDebug() << __FILE__ << " " << __LINE__ << " " << bb;
    query.prepare("SELECT id, deadline, value, inout FROM statements WHERE 1 ORDER BY deadline");

    if (query.exec())
    {
        if (db.transaction())
        {
            QSqlQuery q(db);

            while(query.next())
            {

                int id = query.value(0).toInt();
                QDate deadLine = QDate::fromString(query.value(1).toString(), "yyyy-MM-dd");
                float value = query.value(2).toFloat();
                int inout = query.value(3).toInt();
                float nv=0.0;

                if (deadLine >= bankBalanceDate)
                {
                    //qDebug() << __FILE__ << " " << __LINE__ << " Aggiornamento di bilancio";

                    if (inout == 0)
                        bb = bb - value;
                    else
                        bb  = bb + value;

                    nv = bb;
                }
                else
                    nv = 0.0;

                q.prepare("UPDATE statements SET estimatedbankbalance=:nv where id=:id");
                q.bindValue(":nv", nv);
                q.bindValue(":id", id);

                if (!q.exec())
                    qDebug() << __FILE__ << " " << __LINE__ << tr(" Impossibile effettuare update.");

            }

            if (!db.commit())
                qDebug() << __FILE__ << " " << __LINE__ << tr(" Non è stato possibile effettuare il commit");
        }
        else
            qDebug() << __FILE__ << " " << __LINE__ << tr(" Impossibile avviare Transaction");

    }
    else
        qDebug() << __FILE__ << " " << __LINE__ << " ";
}

void FormStatement::drawGraph()
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    query.prepare("SELECT date, value FROM bankbalance WHERE id=0");

    if (query.exec())
    {
        if (query.next())
        {
            float bankBalance = query.value(1).toFloat();
            QDate bankBalanceDate = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");

            updateEstimatedBankBalance(bankBalanceDate, bankBalance);

            qDebug() << __FILE__ << " " << __LINE__ << " " << " OK";

            this->buildDataForGraph(bankBalance);
        }
        else
            qDebug() << __FILE__ << " " << __LINE__ << " " << "Impossibile recuperare i dati del saldo bancario.";
    }
    else
        qDebug() << __FILE__ << " " << __LINE__ << " ";

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

        msg.setText("Salvataggio avvenuto con successo!!!");
    }
    else
        msg.setText("Non è stato possibile salvare le modifiche apportate!!!");

     msg.exec();

     QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
     QSqlQuery query(db);

     query.prepare("SELECT date, value FROM bankbalance WHERE id=0");

     if(query.exec())
     {
         if (query.next())
         {
             QDate bankBalanceDate = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
             float bankBalance  = query.value(1).toFloat();

             this->updateEstimatedBankBalance(bankBalanceDate,bankBalance);
         }

     }
     else
         qDebug() << __FILE__ << " " << __LINE__ << " Non posso recuperare i dati relativi al saldo bancario";

      this->loadStatementsFromDb();
      drawGraph();
}

void FormStatement::on_comboBox_month_activated(int index)
{
    if (this->ui->radioButton_filterYearMonth->isChecked())
    {
        int y = this->ui->spinBox_year->value();

        setFilter(y, index+1);
        drawGraph();
        this->fillIncomingOutcomingTotal();
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
        drawGraph();
        this->fillIncomingOutcomingTotal();
    }
}

void FormStatement::on_spinBox_year_valueChanged(int arg1)
{
    if (this->ui->radioButton_filterYearMonth->isChecked())
    {
        int m = this->ui->comboBox_month->currentIndex() + 1;

        setFilter(arg1, m);
        drawGraph();
        this->fillIncomingOutcomingTotal();
    }
}

void FormStatement::on_radioButton_filterAll_clicked(bool checked)
{
    if (checked)
    {
        setFilter();
        drawGraph();
        this->fillIncomingOutcomingTotal();
    }
}

void FormStatement::on_radioButton_filterPresentFuture_toggled(bool checked)
{
    if (checked)
    {        
        this->setFilter(1);
        drawGraph();
        this->fillIncomingOutcomingTotal();
    }
}

void FormStatement::on_radioButton_filterPresentPast_toggled(bool checked)
{
    if (checked)
    {
        this->setFilter(0);
        drawGraph();
        this->fillIncomingOutcomingTotal();
    }

}

void FormStatement::on_pushButton_clicked()
{

}

void FormStatement::on_pushButton_bankBalance_clicked()
{
    this->formBankBalance->loadFromDb();
    this->formBankBalance->exec();

    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    query.prepare("SELECT date, value FROM bankbalance WHERE id=0");

    if(query.exec())
    {
        if (query.next())
        {
            QDate bankBalanceDate = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
            float bankBalance  = query.value(1).toFloat();

            this->updateEstimatedBankBalance(bankBalanceDate,bankBalance);
        }

    }
    else
        qDebug() << __FILE__ << " " << __LINE__ << " Non posso recuperare i dati relativi al saldo bancario";

    drawGraph();
}

void FormStatement::on_radioButton_filterAll_toggled(bool checked)
{

}

void FormStatement::on_comboBox_month_activated(const QString &arg1)
{

}

void FormStatement::on_radioButton_filterPresentFuture_clicked(bool checked)
{

}
