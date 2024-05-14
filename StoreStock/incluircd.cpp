#include "incluircd.h"
#include "ui_incluircd.h"

IncluirCD::IncluirCD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirCD)
{
    ui->setupUi(this);
}

IncluirCD::~IncluirCD()
{
    delete ui;
}

void IncluirCD::clear()
{
    ui->nome_lineEdit->setText("");
    ui->preco_lineEdit->setText("");
    ui->faixas_lineEdit->setText("");
}

void IncluirCD::on_buttonBox_accepted()
{
    QString nome, preco, nfaixas;

    nome = ui->nome_lineEdit->text();
    preco = ui->preco_lineEdit->text();
    nfaixas = ui->faixas_lineEdit->text();

    emit signIncluirCD(nome, preco, nfaixas);
}
