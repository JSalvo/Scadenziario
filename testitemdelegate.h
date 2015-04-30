#ifndef TESTITEMDELEGATE_H
#define TESTITEMDELEGATE_H

#include <QStyledItemDelegate>

class TestItemDelegate : public QStyledItemDelegate
{
public:
    TestItemDelegate();


    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // TESTITEMDELEGATE_H
