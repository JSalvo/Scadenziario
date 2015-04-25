#ifndef FORMPAYMENTS_H
#define FORMPAYMENTS_H

#include <QDialog>

namespace Ui {
class formpayments;
}

class formpayments : public QDialog
{
    Q_OBJECT

public:
    explicit formpayments(QWidget *parent = 0);
    ~formpayments();

private:
    Ui::formpayments *ui;
};

#endif // FORMPAYMENTS_H
