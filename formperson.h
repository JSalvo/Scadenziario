#ifndef FORMPERSON_H
#define FORMPERSON_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDataWidgetMapper>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "removerowfrommodel.h"


namespace Ui {
class FormPerson;
}

class FormPerson : public QDialog
{
    Q_OBJECT

public:
    explicit FormPerson(QWidget *parent = 0);
    ~FormPerson();
    void setPersonId(int id);


    void loadDb();

    void loadAllLayouts();



    void loadLayout(QString tableName);

    // 1
    //void loadVerticalLayout_emailAddresses();

    // 2
    //void loadVerticalLayout_phoneNumbers();

private slots:
    void on_pushButton_addEmailAddress_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_clicked();

    // 1
   // void refreshEmailAddressesLayout();

    // 2
    //void refreshPhoneNumbersLayout();


    void refreshLayout(QString tableName);

    void on_pushButton_addPhoneNumber_clicked();

    void on_pushButton_addAddress_clicked();

    void on_pushButton_addDate_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::FormPerson *ui;
    void ReadFromDatabase();
    QDataWidgetMapper *mapperPersonData;
    int personId;


    QList<QDataWidgetMapper *> qdatawidgetmapperemail_list;
    QList<QDataWidgetMapper *> qdatawidgetmapperphonenumbers_list;
    QList<QDataWidgetMapper *> qdatawidgetmapperaddresses_list;
    QList<QDataWidgetMapper *> qdatawidgetmapperdates_list;



    void emptyQDataWidgetMapper(QString tableName);




    void emptyLayout(QLayout *layout);

    // 1
    bool saveEmailAdresses();

    // 2
    bool savePhoneNumbers();

    bool saveAttribute(QString tableName);
    bool saveAttributes();


    void addAttributeToPerson(QString tableName, QString thirdAttribute);

    QList<QDataWidgetMapper *> * getQDataWidgetMapperList(QString tableName);
    QVBoxLayout * getVerticalLayout(QString tableName);
};

#endif // FORMPERSON_H
