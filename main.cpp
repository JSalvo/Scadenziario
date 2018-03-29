#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Espressione regolare per riconoscere espressioni tipo "1/3, 1/3, 1/3" usate ad
    // esempio per indicare come un pagamento debba essere suddiviso in 3 tranche
    QRegExp rx("^\\s*[0-9]+\\s*/\\s*[0-9]+\\s*(,\\s*[0-9]+\\s*/\\s*[0-9]\\s*)*\\s*$");

    // Espressione regolare per riconoscere espressioni tipo "25%, 25%, 25%, 25%" usate ad
    // esempio per indicare come un pagamento debba essere suddiviso in 4 tranche
    QRegExp rx2("^\\s*[0-9]+\\s*%\\s*(,\\s*[0-9]+\\s*%)*\\s*$");

    qDebug() << "Percorso corrente: " << QDir::currentPath();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "ConnectionToDB");
    db.setDatabaseName("../Database/anagrafica.db");

    if (!db.open()) {
          qDebug("Error occurred opening the database.");
          qDebug() << db.lastError().text();
          return -1;
     }

    qDebug() << rx.exactMatch("1 /3,1/3,1/3");
    qDebug() << rx.exactMatch(" 1/3, 1/3 ,1/3 ");
    qDebug() << rx.exactMatch("   1/3,  1/3  ,1/3   ");
    qDebug() << rx.exactMatch("1  /  3,1/3,1/3");
    qDebug() << rx.exactMatch("1/3,1  /   3,1/3");
    qDebug() << rx.exactMatch("1/3,1/3,1   /   3");




    //qDebug() << rx2.exactMatch("33%, 34%, 35%");


    w.show();

    return a.exec();
}
