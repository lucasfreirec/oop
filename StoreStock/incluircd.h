#ifndef INCLUIRCD_H
#define INCLUIRCD_H

#include <QDialog>

namespace Ui {
class IncluirCD;
}

class IncluirCD : public QDialog
{
    Q_OBJECT

public:
    explicit IncluirCD(QWidget *parent = nullptr);
    ~IncluirCD();

    void clear();

private:
    Ui::IncluirCD *ui;

private slots:
    void on_buttonBox_accepted();

signals:
    void signIncluirCD(QString n, QString p, QString nf);
};

#endif // INCLUIRCD_H
