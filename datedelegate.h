#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include <QItemDelegate>
#include <QDateTimeEdit>
#include <QPainter>
#include <QDebug>

class DateDelegate : public QItemDelegate
{
    public:
        DateDelegate(QWidget *parent = 0);
        virtual ~DateDelegate(void);

        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const;
        void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // DATEDELEGATE_H



