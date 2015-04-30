#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "formperson.h"
#include "formstatement.h"
#include "formfincancialplanner.h"
#include "formmanagecontacts.h"
#include "dialogtest.h"
#include "formpayments.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    FormPerson *fp;
    FormStatement *fs;
    FormFincancialPlanner *fplan;
    FormManageContacts *fm;

    DialogTest *dialogTest;
    FormPayments *fpay;

};

#endif // MAINWINDOW_H
