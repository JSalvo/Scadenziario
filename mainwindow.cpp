#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->fp = new FormPerson();

    this->fs = new FormStatement();
    this->fs->setWindowFlags(Qt::Window);
    //this->fs->setInitConfig();

    this->fplan = new FormFincancialPlanner();
    this->fm = new FormManageContacts();
    this->dialogTest = new DialogTest(this);
    this->fpay = new FormPayments(this);
    this->ui->pushButton_4->setVisible(false);
    //this->dialogTest->show();
}

MainWindow::~MainWindow()
{



    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //this->fp->setPersonId(1);
    //this->fp->loadDb();
    //this->fp->loadAllLayouts();

    //this->fp->show();


    this->fm->loadContactsFromDb();
    this->fm->exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->fs->loadStatementsFromDb();
    this->fs->checkStatement();
    this->fs->setInitConfig(); // Imposta i dati al mese corrente
    this->fs->drawGraph();
    this->fs->fillIncomingOutcomingTotal();
    this->fs->exec();

}

void MainWindow::on_pushButton_3_clicked()
{
    this->fplan->computeAllMonthlyAvailability();
    this->fplan->loadFinancialPlanningFromDb();
    this->fplan->exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    this->fpay->show();
}
