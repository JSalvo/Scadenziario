#ifndef CURRENCYDELEGATE_H
#define CURRENCYDELEGATE_H

#include <QStyledItemDelegate>

class CurrencyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CurrencyDelegate(QObject *parent = 0);

signals:

public slots:

};

#endif // CURRENCYDELEGATE_H
