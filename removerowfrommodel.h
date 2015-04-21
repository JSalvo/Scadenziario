#ifndef REMOVEROWFROMMODEL_H
#define REMOVEROWFROMMODEL_H

#include <QPushButton>
#include <QSqlTableModel>
#include <QWidget>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>

class RemoveRowFromModel : public QPushButton
{
    Q_OBJECT
public:
    explicit RemoveRowFromModel(int rowToRemove, QSqlTableModel *tableModel, QString tableName, QWidget *parent = 0);

    ~RemoveRowFromModel();

    void setRowToRemove(int id);
    void setTableModel(QSqlTableModel *tableModel);
   //void setQLineEdit(QLineEdit *lineEdit);

signals:
    void rowRemoved(QString tableName);

public slots:
    void thisObjectClicked(bool);

private:
    int rowToRemove;
    QSqlTableModel *tableModel;
    QString tableName;
    //QLineEdit *lineEdit;

};

#endif // REMOVEROWFROMMODEL_H
