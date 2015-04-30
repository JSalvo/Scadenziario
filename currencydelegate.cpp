#include "currencydelegate.h"

#include <QPainter>
#include <QLocale>

/*
    DELEGATO PER LA VISUALIZZAZIONE DI VALORI DI TIPO "Valuta".
*/


CurrencyDelegate::CurrencyDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}


/* Sovrascrivo solo il metodo paint */
void CurrencyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QChar euro = QChar(8364);
    QLocale local;

    // Converto il valore (incapsulato in index) nella stringa da visualizzare
    QString s = local.toString(index.data().toFloat(), 'f', 2).append(" ").append(euro).append(" ");

    painter->drawText(option.rect, (Qt::AlignRight | Qt::AlignVCenter), s);
}
