#include "datedelegate.h"



/*
    DELEGATO PER LA VISUALIZZAZIONE DI VALORI DI TIPO "Data".
*/


DateDelegate::DateDelegate(QWidget *parent)
  : QItemDelegate(parent)
{
}

DateDelegate::~DateDelegate(void)
{
}

// Sovrascrivo questo metodo per la creazione dell'editor
QWidget *DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QDateTimeEdit *editor = new QDateTimeEdit(parent);

  editor->setCalendarPopup(true);
  editor->setDateTime(QDateTime::fromString(index.data().toString(), "yyyy-MM-dd"));
  editor->setDisplayFormat("dd/MM/yyyy");
  return (editor);
}

// Sovrascrivo questo metodo per popolare l'editor
void DateDelegate::setEditorData(QWidget *paramEditor, const QModelIndex &index) const
{
  QDateTimeEdit *editor = static_cast<QDateTimeEdit *>(paramEditor);

  editor->setDateTime(QDateTime::fromString(index.data().toString(), "yyyy-MM-dd"));
  editor->setDisplayFormat("dd/MM/yyyy");
}

// Sovrascrivo questo metodo per riveversare i dati dall'editor al modello
void DateDelegate::setModelData(QWidget *paramEditor, QAbstractItemModel *model, const QModelIndex& index) const
{
  QDateTimeEdit *editor = static_cast<QDateTimeEdit *>(paramEditor);

  model->setData(index, editor->dateTime().toString("yyyy-MM-dd"));
}

// Sovrascrivo questo metodo per visualizzare la data nel formato desiderato
void DateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // La data nel database è memorizzata come testo nel seguente formato:
    // YYYY-MM-DD

    QString stringDate = index.data().toString();

    // Creo un oggetto date partendo dalla stringa rappresentate la data
    QDate date = QDate::fromString(stringDate, "yyyy-MM-dd");

    // Riformatto la data
    QString text = date.toString("dd/MM/yyyy");

    // Rappresento la data
    painter->drawText(option.rect, int(option.displayAlignment), text);
}
