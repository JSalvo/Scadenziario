#include "removerowfrommodel.h"
#include "QPushButton"

/*********************************************************
Estendo la classe QPushButton per poter memorizzare
un link al modello dal quale QPushButton dovrà rimuovere
una riga. La rimozione viene avviata tramite la chiamata
allo slot thisObjectClicked, il quale a sua volta emette
un SIGNAL in modo da dare informazioni al mondo esterno del
completamento del task.
*********************************************************/


// COSTRUTTORE: Indico riga da rimuovere e modello da cui rimuovere la riga
// La rimozione avverrà solo dopo il click di questo pulsante
RemoveRowFromModel::RemoveRowFromModel(int rowToRemove, QSqlTableModel *tableModel, QString tableName, QWidget *parent):
    QPushButton(parent)
{
    this->rowToRemove = -1;
    this->tableName = tableName;
    QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(thisObjectClicked(bool)));

    setRowToRemove(rowToRemove);
    setTableModel(tableModel);

    this->setText("-");
    this->setFixedWidth(30);

    //setQLineEdit(lineEdit);
}

RemoveRowFromModel::~RemoveRowFromModel()
{
    // Distruzione del qlineedit
     //delete this->lineEdit;
    qDebug("RemoveRowFromModel Destroyed");

}



// Indico quale riga dovrà essere rimossa dal model al click di questo oggetto
void RemoveRowFromModel::setRowToRemove(int id)
{
    this->rowToRemove = id;
}

// Fornisco un reference al model da cui rimuovere la riga
void RemoveRowFromModel::setTableModel(QSqlTableModel *model)
{
    this->tableModel = model;
}

/*
void RemoveRowFromModel::setQLineEdit(QLineEdit *lineEdit)
{
    this->lineEdit = lineEdit;
}*/

// SLOT richiamato al click di questo oggetto.
void RemoveRowFromModel::thisObjectClicked(bool)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Avviso!!!"), "Vuoi Rimuovere veramente questa proprietà?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        // Rimuovo dal modello la riga corrispondente al button
        this->tableModel->removeRow(this->rowToRemove);
        // Emetto un segnale per comunicare il completamento del task
        emit rowRemoved(this->tableName);

        // Autodistruzione (non posso cancellare me stesso, quindi pianifico la mia morte)
         //this->deleteLater();

    }
    else
    {

    }
}
