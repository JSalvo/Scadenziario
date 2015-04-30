#include "dialogtest.h"
#include "ui_dialogtest.h"

#include <QItemDelegate>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include "testitemdelegate.h"

DialogTest::DialogTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTest)
{
    ui->setupUi(this);
}

DialogTest::~DialogTest()
{
    delete ui;
}

void DialogTest::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("ConnectionToDB");
    QSqlTableModel *model = new QSqlTableModel(this, db);
    TestItemDelegate *delegate = new TestItemDelegate();

    model->setTable("test");
    this->ui->tableView->setModel(model);
    this->ui->tableView->setItemDelegateForColumn(1, delegate);

    model->select();

}
