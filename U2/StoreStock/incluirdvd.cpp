#include "incluirdvd.h"
#include "ui_incluirdvd.h"

IncluirDVD::IncluirDVD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirDVD)
{
    ui->setupUi(this);
}

IncluirDVD::~IncluirDVD()
{
    delete ui;
}

void IncluirDVD::clear()
{
    ui->nome_lineEdit->setText("");
    ui->preco_lineEdit->setText("");
    ui->duracao_lineEdit->setText("");
}

void IncluirDVD::on_buttonBox_accepted()
{
    QString nome, preco, duracao;

    nome = ui->nome_lineEdit->text();
    preco = ui->preco_lineEdit->text();
    duracao = ui->duracao_lineEdit->text();

    emit signIncluirDVD(nome, preco, duracao);
}

