#ifndef FORMFINCANCIALPLANNER_H
#define FORMFINCANCIALPLANNER_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QDate>

namespace Ui {
class FormFincancialPlanner;
}

class FormFincancialPlanner : public QDialog
{
    Q_OBJECT

public:
    explicit FormFincancialPlanner(QWidget *parent = 0);
    ~FormFincancialPlanner();

    void loadFinancialPlanningFromDb();
     void computeAllMonthlyAvailability();
     void checkPlanning();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_planning_clicked();

    void on_pushButton_effectiveAvailability_clicked();

    void on_pushButton_checkPlanning_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::FormFincancialPlanner *ui;


    void computeMonthlyPlanning(int year, int month);

    void computeMonthlyAvailability(int year, int month);

    QString filter;

};

#endif // FORMFINCANCIALPLANNER_H
