#include "datedelegate.h"

DateDelegate::DateDelegate(QWidget *parent)
  : QItemDelegate(parent)
{
}

DateDelegate::~DateDelegate(void)
{
}

QWidget *DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QDateTimeEdit *editor = new QDateTimeEdit(parent);

  editor->setCalendarPopup(true);
  editor->setDateTime(QDateTime::fromString(index.data().toString(), "yyyy-MM-dd"));
  editor->setDisplayFormat("yyyy-MM-dd");
  return (editor);
}

void DateDelegate::setEditorData(QWidget *paramEditor, const QModelIndex &index) const
{
  QDateTimeEdit *editor = static_cast<QDateTimeEdit *>(paramEditor);

  editor->setDateTime(QDateTime::fromString(index.data().toString(), "yyyy-MM-dd"));
  editor->setDisplayFormat("yyyy-MM-dd");
}

void DateDelegate::setModelData(QWidget *paramEditor, QAbstractItemModel *model, const QModelIndex& index) const
{
  QDateTimeEdit *editor = static_cast<QDateTimeEdit *>(paramEditor);

  model->setData(index, editor->dateTime().toString("yyyy-MM-dd"));
}

void DateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*
    QString stringDate = index.data().toString();
    qDebug() << stringDate;

    QDate date = QDate::fromString(stringDate, "dd/MM/yyyy");


    QString text = date.toString("dd/MM/yyyy");

    qDebug() << text;
    painter->drawText(option.rect, text);
    painter->drawText();*/

    QItemDelegate::paint(painter, option, index);
}
