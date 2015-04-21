#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "ConnectionToDB");
    db.setDatabaseName("../Database/anagrafica.db");

    if (!db.open()) {
          qDebug("Error occurred opening the database.");
          qDebug() << db.lastError().text();
          return -1;
     }



    w.show();

    return a.exec();
}
