#include "incluirlivro.h"
#include "ui_incluirlivro.h"

IncluirLivro::IncluirLivro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirLivro)
{
    ui->setupUi(this);
}

IncluirLivro::~IncluirLivro()
{
    delete ui;
}

void IncluirLivro::clear()
{
    ui->nome_lineEdit->setText("");
    ui->preco_lineEdit->setText("");
    ui->autor_lineEdit->setText("");
}

void IncluirLivro::on_buttonBox_accepted()
{
    QString nome, preco, autor;

    nome = ui->nome_lineEdit->text();
    preco = ui->preco_lineEdit->text();
    autor = ui->autor_lineEdit->text();

    emit signIncluirLivro(nome, preco, autor);
}

