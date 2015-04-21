#include "formmanagecontacts.h"
#include "ui_formmanagecontacts.h"

FormManageContacts::FormManageContacts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormManageContacts)
{
    ui->setupUi(this);

}

FormManageContacts::~FormManageContacts()
{
    delete ui;
}

void FormManageContacts::on_pushButton_modifyContact_clicked()
{


    QAbstractItemModel *model = this->ui->tableView_contacts->model();

    if (model)
    {
        QItemSelectionModel *itemSelectionModel = this->ui->tableView_contacts->selectionModel();

        if (itemSelectionModel)
        {
            if (itemSelectionModel->selectedRows().size() > 0)
            {
                QModelIndex midx = itemSelectionModel->selectedRows().at(0);

                if (midx.isValid())
                {
                    int row = midx.row();
                    int idPerson = model->data(model->index(row, 0)).toInt();

                    this->fp = new FormPerson(this);

                    this->fp->setPersonId(idPerson);
                    this->fp->loadDb();
                    this->fp->loadAllLayouts();

                    this->fp->exec();
                    delete(this->fp);
                    this->loadContactsFromDb();

                }
                else
                {
                    QMessageBox msg;
                    msg.setText("Nessun contatto/azienda selezionato");
                    msg.exec();

                }
            }
            else
            {
                QMessageBox msg;
                msg.setText("Nessun contatto/azienda selezionato");
                msg.exec();
                this->loadContactsFromDb();

            }
        }
        else
        {
            QMessageBox msg;
            msg.setText("Impossibile caricare i dati dal database");
            msg.exec();

        }
    }
}


// NOTA: Rendere in sola lettura la tabella
void FormManageContacts::loadContactsFromDb()
{
    // Recupero link al database
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    // Creo un modello sche si riferirà alla tabella delle mail
    QSqlTableModel *model= new QSqlTableModel(this, db);
    QAbstractItemModel *oldModel;

    oldModel = this->ui->tableView_contacts->model();

    model->setTable("persons");
    model->setFilter("id<>0");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Nome"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Azienda"));
    model->select();

    this->ui->tableView_contacts->setModel(model);
    this->ui->tableView_contacts->setColumnHidden(0, true);
    this->ui->tableView_contacts->setColumnHidden(1, true);
    this->ui->tableView_contacts->setColumnHidden(2, true);
    this->ui->tableView_contacts->setColumnHidden(4, true);
    this->ui->tableView_contacts->setColumnHidden(5, true);
    this->ui->tableView_contacts->setColumnHidden(7, true);

    this->ui->tableView_contacts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    this->ui->tableView_contacts->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->tableView_contacts->setSelectionBehavior(QAbstractItemView::SelectRows);

    if (oldModel)
        delete (oldModel);
}

void FormManageContacts::on_pushButton_newContact_clicked()
{
     QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
     QSqlQuery query(db);

     query.prepare("INSERT INTO persons(name) values('')");

     if (!query.exec())
     {
         QMessageBox msg;
         msg.setText("Non è stato possibile inserire il nuovo contatto/azienda");
         msg.exec();
     }
     else
     {
         this->loadContactsFromDb();
         this->ui->tableView_contacts->selectRow(this->ui->tableView_contacts->model()->rowCount()-1);
         this->ui->pushButton_modifyContact->click();
     }

}

// Elimina il contatto / azienda. In concatenazione biogna eliminare i movimenti associati.
void FormManageContacts::on_pushButton_deleteContact_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Avviso!!!"), "Vuoi Rimuovere veramente questo contatto/azienda?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        return;

    }



    QAbstractItemModel *model = this->ui->tableView_contacts->model();
    if (model)
    {
        QItemSelectionModel *itemSelectionModel = this->ui->tableView_contacts->selectionModel();

        if (itemSelectionModel)
        {
            int nr = itemSelectionModel->selectedRows().count();

            if (nr>0)
            {
                QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
                QSqlQuery query(db);

                int row = itemSelectionModel->selectedRows().at(0).row();
                int idPerson = model->data(model->index(row, 0)).toInt();

                query.prepare("SELECT * FROM statements WHERE id_company=:idPerson");
                query.bindValue(":idPerson", idPerson);

                if (query.exec())
                {

                    if (query.next())
                    {
                        QMessageBox msg;
                        msg.setText("Sono ancora presenti dei movimenti che si riferiscono a questo contatto, "
                                    "eliminali e poi procedi nuovamente alla cancellazione del contatto");
                        msg.exec();
                    }
                    else
                    {
                        model->removeRow(row);
                        ((QSqlTableModel *) model)->submitAll();
                        //loadContactsFromDb();
                    }

                }
                else
                    qDebug() << "Non è stato possibile interrogare la tabella statements: " << query.lastError().text();
            }
            else
            {
                QMessageBox msg;

                msg.setText("Nessun contatto selezionato per la rimozione");
                msg.exec();
            }


        }
        else
        {
            QMessageBox msg;

            msg.setText("Nessun contatto selezionato per la rimozione");
            msg.exec();
        }
    }
    else
    {
        QMessageBox msg;

        msg.setText("Impossibile rimuovere il contatto/azienda selezionato/a");
        msg.exec();
    }
}

void FormManageContacts::on_pushButton_close_clicked()
{
    this->close();
}
