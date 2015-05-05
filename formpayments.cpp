#include "formpayments.h"
#include "ui_formpayments.h"

#include <QDebug>

FormPayments::FormPayments(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formpayments)
{


    ui->setupUi(this);
}

FormPayments::~FormPayments()
{
    delete ui;
}

// Produce una lista contenente le frazioni per la generazione dei pagamenti splittati
QList<float> FormPayments::getRatioPayment(QString s)
{
    // Espressione regolare per riconoscere espressioni tipo "1/3, 1/3, 1/3" usate ad
    // esempio per indicare come un pagamento debba essere suddiviso in 3 tranche
    QRegExp rx("^\\s*[0-9]+\\s*/\\s*[0-9]+\\s*(,\\s*[0-9]+\\s*/\\s*[0-9]\\s*)*\\s*$");

    // Espressione regolare per riconoscere espressioni tipo "25%, 25%, 25%, 25%" usate ad
    // esempio per indicare come un pagamento debba essere suddiviso in 4 tranche
    QRegExp rx2("^\\s*[0-9]+\\s*%\\s*(,\\s*[0-9]+\\s*%)*\\s*$");

    QList<float> result;


    // Se la stringa passata è della forma "a/b, c/d, ..."
    if (rx.exactMatch(s))
    {
        QStringList sl;

        qDebug() << "Match esatto ";

        sl = s.split(","); // Eseguo uno split sulla stringa in base alle virgole

        qDebug() << sl;

        for (int i=0; i<sl.length(); i++)
        {
            float num, den;
            QStringList numden = sl[i].split("/");

            num = numden[0].toFloat();
            den = numden[1].toFloat();


            qDebug() << num << " " << den;

            result.append(num/den);

        }
    } // Altrimenti se la stringa passata è della forma "ab%, cd%, ..."
    else if (rx2.exactMatch(s))
    {
        QStringList sl;

        sl = s.split(",");

        for (int i=0; i<sl.length(); i++)
        {
            QStringList per = sl[i].split("%");

            qDebug() << per[0];

            result.append(per[0].toFloat()/100.0);
        }
    }

    return result;
}

QList<int> FormPayments::getDelayForPayment(QString s)
{
    QList<int> result;

    QRegExp rx("^\\s*\\d+\\s*(,\\s*\\d+\\s*)*$");

    if (rx.exactMatch(s))
    {
        QStringList sl = s.split(",");

        for (int i=0; i<sl.count(); i++)
        {
            result.append(sl[i].toInt());
        }
    }

    return(result);
}

void FormPayments::on_pushButton_generateStatements_clicked()
{
    QList<float> r1;
    QList<int> r2;

    r1 = getRatioPayment(this->ui->lineEdit_splitPayment->text());
    r2 = getDelayForPayment(this->ui->lineEdit_delayPayment->text());

    qDebug() << r1;
    qDebug() << r2;
}
