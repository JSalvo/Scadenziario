#include "formperson.h"
#include "ui_formperson.h"


/*
    CAMPI MULTIVALORE ASSOCIATI DELLA PERSONA

    Creazione:

    1 - Un modello per ogni valore del campo multivalore
    2 - Un QDataWidgetMapper per ogni valore del campo multivalore
    3 - Un oggetto lineEdit per visualizzare ed editare il dato
    4 - Un RemoveRowFromModel button per rimuovere il valore dal modello
    5 - Un QHBoxLayout per raggruppare lineEdit e button di rimozione

    Questi oggetti, vanno de-allocati, pertanto devo avere dei riferimenti per
    poterli recuperare e de-allocare

    Dei QDataWidgetMapper creo una lista per ciascuna tipologia di campo. Il QDataWidgetMapper
    memorizza inoltre un reference al modello corripondente, permettendomi di recuperarlo e
    di deallocare la memoria a cui questo reference si riferisce (Quindi 1 e 2 OK)

    Per ciascuna tipologia di campo, creo una lista di RemoveRowFromModel button che al loro
    interno hanno un riferimento al QLineEdit. La distruzione dei button, causa quindi la
    distruzione dei lineEdit. (3 e 4 OK).

    Dei QHBoxLayout non ho bisogno di reference in quanto contenuti in un altro layour, posso
    distruggerli da li.
*/


FormPerson::FormPerson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormPerson)
{
    personId = -1;

    ui->setupUi(this);
    this->mapperPersonData = new QDataWidgetMapper();
    this->ui->frame_custom->hide();
    this->ui->frame_image->hide();

}

FormPerson::~FormPerson()
{
    delete ui;
}

void FormPerson::setPersonId(int id)
{
    this->personId = id;
}

// Leggo dal database le infomazioni "principali" relative alla persona this->personId
// e le rappresento tramite dei QLineEdit
void FormPerson::loadDb()
{
    // De-alloco eventuali model precedentemente creati
    QAbstractItemModel *modelToDelete =  this->mapperPersonData->model();
    delete(this->mapperPersonData);
    delete(modelToDelete);
    this->mapperPersonData = new QDataWidgetMapper();

    // Reference al database
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    // Creazione di un modello ...
    QSqlTableModel *model = new QSqlTableModel(this, db);

    // ... che si riferisca alla tabella persons
    model->setTable("persons");
    // Imposto un filtro per lavorare su this->personId
    model->setFilter("id=" + QString::number(this->personId));
    // Eseguo la query
    model->select();


    // Associo al mapper il model
    this->mapperPersonData->setModel(model);

    // Eseguo il mapping dei campi del modello con i rispettivi campi di editing

    // Nome della Persona
    this->mapperPersonData->addMapping(this->ui->lineEdit_personName, 3);

    // Nickname della persona
    this->mapperPersonData->addMapping(this->ui->lineEdit_personNickname, 4);

    // Azienda in cui lavora la persona
    this->mapperPersonData->addMapping(this->ui->lineEdit_personComapny, 6 );

    // Titolo o ruolo svolto all'interno dell'azienda
    this->mapperPersonData->addMapping(this->ui->lineEdit_personTitle, 2);

    // Note associate alla persona
    this->mapperPersonData->addMapping(this->ui->textEdit_note, 7);

    this->mapperPersonData->toFirst();


    // Elementi da de-allocare:
    // model
}









void FormPerson::on_pushButton_clicked()
{
    emptyLayout(this->ui->verticalLayout_emailAddresses);
}



/*Dato un layout, elimina il suo contenuto, "distruggendo" gli oggetti che contiene*/
void FormPerson::emptyLayout(QLayout * layout)
{
    QLayoutItem * item;
    QLayout * sublayout;
    QWidget * widget;

    // Per ciascun oggetto conenuto nel layout
    while ((item = layout->takeAt(0)))
    {
        // Se l'oggetto è un layout, ricorsivamente richiamo questa funzione
        if ((sublayout = item->layout()) != 0)
        {           
            emptyLayout(sublayout);
        }
        else if ((widget = item->widget()) != 0) // ...se invece l'oggetto è un widget ..
        {
            widget->hide(); // nascondo il widget
            layout->removeWidget(widget); // rimuovo il widget dal layout
            delete widget; // distruggo il widget
        }
        else //...in qualsiasi altro caso ...
        {
            // NOTA: in realtà non so quando avviene questo evento

            // rimuovo l'oggetto dal layout
            layout->removeItem(item);
            // distruggo l'oggetto
            delete item;

            qDebug() << "Perchè sono qui? Dovremo capirlo prima o poi!!!";
        }
    }
}


bool FormPerson::saveAttribute(QString tableName)
{
    QList<QDataWidgetMapper *> *list;

    list = getQDataWidgetMapperList(tableName);

    for (int i=0; i < list->length(); i++)
    {
        if (!((*list)[i]->submit()))
            return(false);
    }
    return(true);

}

void FormPerson::ReadFromDatabase()
{
    this->ui->verticalLayout_emailAddresses->addWidget(new QPushButton());
}

void FormPerson::loadLayout(QString tableName)
{
    // Recupero link al database
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    // Creo un modello sche si riferirà alla tabella delle mail
    QSqlTableModel *model = new QSqlTableModel(this, db);

    // Imposto la tabella a cui si riferirà il modello
    model->setTable(tableName);
    // Imposto un filtro sulla persona a cui la tabella è collegata
    model->setFilter("id_person=" + QString::number(this->personId));
    model->select();

    // Per ciascuna riga della tabella...
    for (int i=0; i<model->rowCount(); i++)
    {
        QHBoxLayout *hbl = new QHBoxLayout();

        // Creo un nuovo oggetto QLineEdit
        QLineEdit *qlineedit = new QLineEdit();
        // Creo un pulsante per la successiva rimozione dell'entry nel database
        RemoveRowFromModel *button = new RemoveRowFromModel(i, model, tableName);
        // Collego il pulante alla funzione di refresh
        QObject::connect(button, SIGNAL(rowRemoved(QString)), this, SLOT(refreshLayout(QString)));

        // Creo un mapper
        QDataWidgetMapper *mapperEmailPerson = new QDataWidgetMapper();

        // Collego al mapper il modello della tabella
        mapperEmailPerson->setModel(model);
        // Aggiungo un mapping tra l'oggetto QLineEdit e il modello
        mapperEmailPerson->addMapping(qlineedit, 1);
        // Posiziono il mapper sull'indice opportuno
        mapperEmailPerson->setCurrentIndex(i);

        // Inserisco il mapper nella lista dei mapper per le email
        getQDataWidgetMapperList(tableName)->append(mapperEmailPerson);



        // qlineedit e button, li visualizzo sulla stessa riga (li inserisco in un layout orizzontale)
        hbl->addWidget(qlineedit);
        hbl->addWidget(button);
        // Inserisco il layout orizzontale nel layout delle mail
        getVerticalLayout(tableName)->addLayout(hbl);

        // Collego il pulsante ad uno slot, in modo che venga gestita la rimozione di una riga dal
        // modello
    }
}

void FormPerson::refreshLayout(QString tableName)
{
    emptyQDataWidgetMapper(tableName);

    /*
    if (tableName.compare("emails"))
         emptyLayout(this->ui->verticalLayout_emailAddresses);
    else if (tableName.compare("phones"))
         emptyLayout(this->ui->verticalLayout_phoneNumbers);*/

    emptyLayout(this->getVerticalLayout(tableName));

    loadLayout(tableName);
}

void FormPerson::addAttributeToPerson(QString tableName, QString thirdAttribute)
{
     this->saveAttributes();

    // Recupero link al database
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlQuery query(db);

    query.prepare("INSERT INTO " + tableName + "(id_person,value," + thirdAttribute + ") VALUES(:id_person,'',2)");


    query.bindValue(":id_person", this->personId);


    if (!query.exec())
    {
        qDebug() << tr("Non è stato possibile inserire il nuovo attributo: ") << query.lastError().text() << " " << query.lastQuery();
    }
    else
    {
        refreshLayout(tableName);
    }

}

// Svuota la lista corrispondente a tableName contenente
// oggetti QDataWidgetMapper. Inoltre de-alloca la memoria dei modelli
// a cui questi mapper si riferiscono
void FormPerson::emptyQDataWidgetMapper(QString tableName)
{
    QList<QDataWidgetMapper *> *list;

    list = getQDataWidgetMapperList(tableName);

    while (!list->isEmpty())
    {
        QDataWidgetMapper *mapper = (QDataWidgetMapper *) list->takeFirst();

        delete mapper->model();
        delete mapper;
    }
}


/*
 *
 *
 *
 *
 *
 *
 *
 * Modificare le funzioni SEGUENTI per aggiungere attributi al form
 *
 *
 *
 *
 *
 *
*/

bool FormPerson::saveAttributes()
{
    bool r1, r2, r3, r4;

    r1 = saveAttribute("emails");
    r2  =saveAttribute("phones");
    r3 = saveAttribute("addresses");
    r4 = saveAttribute("dates");



    return (r1 && r2 && r3 && r4);
}


void FormPerson::loadAllLayouts()
{
    this->loadLayout("emails");
    this->loadLayout("phones");
    this->loadLayout("addresses");
    this->loadLayout("dates");

}

QList<QDataWidgetMapper *> *FormPerson::getQDataWidgetMapperList(QString tableName)
{
    if (tableName.compare("emails") == 0)
        return &(this->qdatawidgetmapperemail_list);
    else if (tableName.compare("phones") == 0)
        return &(this->qdatawidgetmapperphonenumbers_list);
    else if (tableName.compare("addresses") == 0)
        return &(this->qdatawidgetmapperaddresses_list);
    else if (tableName.compare("dates") == 0)
        return &(this->qdatawidgetmapperdates_list);


}

QVBoxLayout *FormPerson::getVerticalLayout(QString tableName)
{
    if (tableName.compare("emails") == 0)
        return (this->ui->verticalLayout_emailAddresses);
    else if (tableName.compare("phones") == 0)
        return (this->ui->verticalLayout_phoneNumbers);
    else if (tableName.compare("addresses") == 0)
        return (this->ui->verticalLayout_addresses);
    else if (tableName.compare("dates") == 0)
        return (this->ui->verticalLayout_dates);
}


void FormPerson::on_pushButton_addEmailAddress_clicked()
{
    addAttributeToPerson("emails", "id_emailType");
}

void FormPerson::on_pushButton_addPhoneNumber_clicked()
{
    addAttributeToPerson("phones", "id_phoneType");
}

void FormPerson::on_pushButton_addAddress_clicked()
{
     addAttributeToPerson("addresses", "id_addressType");
}

void FormPerson::on_pushButton_addDate_clicked()
{
    addAttributeToPerson("dates", "id_dateType");
}

/*
 *
 *
 *
 *
 *
 *
 *
 * Modificare le funzioni PRECEDENTI per aggiungere attributi al form
 *
 *
 *
 *
 *
 *
*/



void FormPerson::on_pushButton_cancel_clicked()
{

}

void FormPerson::on_pushButton_save_clicked()
{
    QMessageBox msgBox;

    if (this->mapperPersonData->submit())
    {
        msgBox.setText(tr("Modifiche salvate con successo!!!"));

        if (saveAttributes())
        {
            msgBox.exec();

            this->close();
        }


    }
    else
    {
        msgBox.setText(tr("Non è stato possibile salvare le modifiche effettuate!!!"));
        msgBox.exec();
    }
}









void FormPerson::on_pushButton_close_clicked()
{
    this->close();
}
