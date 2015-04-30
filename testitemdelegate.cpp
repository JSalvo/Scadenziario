#include "testitemdelegate.h"

#include <QPainter>

TestItemDelegate::TestItemDelegate()
{
}

void TestItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    painter->drawText(option.rect, int(option.displayAlignment), QString("ciao"));

    //painter->drawText(option.rect,)

    // QTextOption

    //QStyledItemDelegate::paint(painter, option, index);
}
