#ifndef FORMMANAGECONTACTS_H
#define FORMMANAGECONTACTS_H

#include <QWidget>
#include "formperson.h"

namespace Ui {
class FormManageContacts;
}

class FormManageContacts : public QDialog
{
    Q_OBJECT

public:
    explicit FormManageContacts(QWidget *parent = 0);
    ~FormManageContacts();

    void loadContactsFromDb();

private slots:
    void on_pushButton_modifyContact_clicked();

    void on_pushButton_newContact_clicked();

    void on_pushButton_deleteContact_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::FormManageContacts *ui;
    FormPerson *fp;

};

#endif // FORMMANAGECONTACTS_H
