#ifndef FORMSTATEMENT_H
#define FORMSTATEMENT_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

#include <datedelegate.h>
#include "formbankbalance.h"


namespace Ui {
class FormStatement;
}

class FormStatement : public QDialog
{
    Q_OBJECT

public:
    explicit FormStatement(QWidget *parent = 0);
    ~FormStatement();

     void loadStatementsFromDb();
     void checkStatement();
     void drawGraph();
     void setInitConfig();

         void fillIncomingOutcomingTotal();

private slots:

    void on_pushButton_addRecord_clicked();

    void on_pushButton_removeRecord_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_check_clicked();

    void on_comboBox_month_activated(int index);

    void on_spinBox_valueChanged(int arg1);

    void on_radioButton_filterYearMonth_toggled(bool checked);

    void on_spinBox_year_valueChanged(int arg1);

    void on_radioButton_filterAll_clicked(bool checked);

    void on_radioButton_filterPresentFuture_toggled(bool checked);

    void on_radioButton_filterPresentPast_toggled(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_bankBalance_clicked();

    void on_radioButton_filterAll_toggled(bool checked);

    void on_comboBox_month_activated(const QString &arg1);

    void on_radioButton_filterPresentFuture_clicked(bool checked);

    void on_radioButton_filterYearMonth_clicked();

    void on_radioButton_filterYearMonth_clicked(bool checked);

    void on_lineEdit_filter_textChanged(const QString &arg1);

    void on_lineEdit_filter_textEdited(const QString &arg1);

    void on_radioButton_inOut_clicked(bool checked);

    void on_radioButton_in_clicked(bool checked);

    void on_radioButton_out_clicked();

private:
    Ui::FormStatement *ui;
    void addRecord();
    void removeRecord();

    void setFilter();
    void setFilter(int year, int month);
    void setFilter(int pastfuture);

    QString filter;



    float getIncoming();
    float getOutcoming();


    float getIncoming(int y, int m);
    float getOutcoming(int y, int m);

    float getIncomingPresentPast();
    float getOutcomingPresentPast();

    float getIncomingPresentFuture();
    float getOutcomingPresentFuture();

    void buildDataForGraph(float bankBalance);
    void updateEstimatedBankBalance(QDate bankBalanceDate, float bankBalance);
    FormBankBalance *formBankBalance;

    QString get_company_id_list_from_like_sql(QString filter);


};

#endif // FORMSTATEMENT_H
