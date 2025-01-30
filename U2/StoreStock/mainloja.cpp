#include "mainloja.h"
#include "ui_mainloja.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>


MainLoja::MainLoja(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainLoja)
    , X()
    , inclLivro(nullptr)
    , inclCD(nullptr)
    , inclDVD(nullptr)
    , total_itens(nullptr)
{
    ui->setupUi(this);

    inclLivro = new IncluirLivro(this);
    inclCD = new IncluirCD(this);
    inclDVD = new IncluirDVD(this);
    total_itens = new QLabel(this);

    statusBar()->insertWidget(0, new QLabel("Total de itens: "));
    statusBar()->insertWidget(1, total_itens);

    connect(inclLivro, &IncluirLivro::signIncluirLivro, this, &MainLoja::slotIncluirLivro);
    connect(inclCD, &IncluirCD::signIncluirCD, this, &MainLoja::slotIncluirCD);
    connect(inclDVD, &IncluirDVD::signIncluirDVD, this, &MainLoja::slotIncluirDVD);

    ui->titleLIV->setStyleSheet("background-color: lightgray");
    ui->titleCDS->setStyleSheet("background-color: lightgray");
    ui->titleDVDS->setStyleSheet("background-color: lightgray");

    ui->tabelaLIV->setStyleSheet("QHeaderView::section { text-align:center }"
                                 "QHeaderView::section { background-color:lightgray }");

    ui->tabelaCD->setStyleSheet("QHeaderView::section { text-align:center }"
                                "QHeaderView::section { background-color:lightgray }");

    ui->tabelaDVD->setStyleSheet("QHeaderView::section { text-align:center }"
                                 "QHeaderView::section { background-color:lightgray }");

    for (int i=0; i < 3; ++i)
    {
        if(i==0)
        {
            ui->tabelaLIV->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
            ui->tabelaCD->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
            ui->tabelaDVD->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }
        else
        {
            ui->tabelaLIV->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
            ui->tabelaCD->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
            ui->tabelaDVD->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        }
    }

    QStringList headerLIV, headerCD, headerDVD, np;

    np << "NOME" << "PRECO";
    headerLIV << np << "AUTOR";
    headerCD << np << "N FAIXAS";
    headerDVD << np << "DURACAO";

    ui->tabelaLIV->setHorizontalHeaderLabels(headerLIV);
    ui->tabelaDVD->setHorizontalHeaderLabels(headerDVD);
    ui->tabelaCD->setHorizontalHeaderLabels(headerCD);
    ui->tabelaLIV->horizontalHeader()->setHighlightSections(false);
    ui->tabelaDVD->horizontalHeader()->setHighlightSections(false);
    ui->tabelaCD->horizontalHeader()->setHighlightSections(false);

    exibirLivros();
    exibirCDS();
    exibirDVDS();

    total_itens->setNum(0);
}

void MainLoja::exibirLivros()
{
    ui->tabelaLIV->clearContents();

    ui->tabelaLIV->setRowCount(X.getNumLivro());

    for(int i=0; i<X.getNumLivro(); ++i)
    {
        Livro L = X.getLivro(i);

        for(int j=0; j<3; ++j)
        {
            if(j == 0)
            {
                QLabel *prov = new QLabel(L.getNome().c_str());

                prov->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

                ui->tabelaLIV->setCellWidget(i, j ,prov);
            }

            if(j == 1)
            {
                QLabel *prov = new QLabel(QString::number(L.getPreco(), 'f', 2));

                prov->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

                ui->tabelaLIV->setCellWidget(i, j ,prov);
            }

            if(j==2)
            {
                QLabel *prov = new QLabel(L.getAutor().c_str());

                prov->setAlignment(Qt::AlignLeft);

                ui->tabelaLIV->setCellWidget(i, j ,prov);
            }
        }
    }
    total_itens->setNum(X.getNumLivro()+X.getNumCD()+X.getNumDVD());
}

void MainLoja::exibirCDS()
{
    ui->tabelaCD->clearContents();

    ui->tabelaCD->setRowCount(X.getNumCD());

    for(int i=0; i<X.getNumCD(); ++i)
    {
        CD C = X.getCD(i);

        for(int j=0; j<3; ++j)
        {
            if(j == 0)
            {
                QLabel *prov = new QLabel(C.getNome().c_str());

                prov->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

                ui->tabelaCD->setCellWidget(i, j ,prov);
            }

            if(j == 1)
            {
                QLabel *prov = new QLabel(QString::number(C.getPreco(), 'f', 2));

                prov->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

                ui->tabelaCD->setCellWidget(i, j ,prov);
            }

            if(j==2)
            {
                QLabel *prov = new QLabel(QString::number(C.getNumFaixas()));

                prov->setAlignment(Qt::AlignCenter);

                ui->tabelaCD->setCellWidget(i, j ,prov);
            }
        }
    }
    total_itens->setNum(X.getNumLivro()+X.getNumCD()+X.getNumDVD());
}

void MainLoja::exibirDVDS()
{
    ui->tabelaDVD->clearContents();

    ui->tabelaDVD->setRowCount(X.getNumDVD());

    for(int i=0; i<X.getNumDVD(); ++i)
    {
        DVD D = X.getDVD(i);

        for(int j=0; j<3; ++j)
        {
            if(j == 0)
            {
                QLabel *prov = new QLabel(D.getNome().c_str());

                prov->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

                ui->tabelaDVD->setCellWidget(i, j ,prov);
            }

            if(j == 1)
            {
                QLabel *prov = new QLabel(QString::number(D.getPreco(), 'f', 2));

                prov->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

                ui->tabelaDVD->setCellWidget(i, j ,prov);
            }

            if(j==2)
            {
                QLabel *prov = new QLabel(QString::number(D.getDuracao()));

                prov->setAlignment(Qt::AlignCenter);

                ui->tabelaDVD->setCellWidget(i, j ,prov);
            }
        }
    }
    total_itens->setNum(X.getNumLivro()+X.getNumCD()+X.getNumDVD());
}
MainLoja::~MainLoja()
{
    delete ui;
}

void MainLoja::slotIncluirLivro(QString nome, QString preco, QString autor)
{
    std::string n = nome.toStdString(), a = autor.toStdString();
    float p = preco.toFloat();

    if(n == "" || p <= 0.0 || a == "")
    {
        QString mensagem;

        if(n == "") mensagem = "Voce precisa escolher um nome.";
        else if(p <= 0.0) mensagem = "Preco invalido.";
        else mensagem = "Voce precisa escolher um autor.";

        QMessageBox::critical(this, "Livro invalido", mensagem);

        return;
    }

    X.incluirLivro(Livro(n, p, a));

    exibirLivros();
}

void MainLoja::slotIncluirCD(QString nome, QString preco, QString numfaixas)
{
    std::string n = nome.toStdString();
    float p = preco.toFloat();
    int nf = numfaixas.toInt();

    if(n == "" || p <= 0.0 || nf <= 0)
    {
        QString mensagem;

        if(n == "") mensagem = "Voce precisa escolher um nome.";
        else if(p <= 0.0) mensagem = "Preco invalido.";
        else mensagem = "Numero de faixas invalido.";

        QMessageBox::critical(this, "CD invalido", mensagem);

        return;
    }

    X.incluirCD(CD(n, p, nf));

    exibirCDS();
}

void MainLoja::slotIncluirDVD(QString nome, QString preco, QString duracao)
{
    std::string n = nome.toStdString();
    float p = preco.toFloat();
    int d = duracao.toInt();

    if(n == "" || p <= 0.0 || d <= 0)
    {
        QString mensagem;

        if(n == "") mensagem = "Voce precisa escolher um nome.";
        else if(p <= 0.0) mensagem = "Preco invalido.";
        else mensagem = "Voce precisa escolher uma duracao.";

        QMessageBox::critical(this, "DVD invalido", mensagem);

        return;
    }

    X.incluirDVD(DVD(n, p, d));

    exibirDVDS();
}

void MainLoja::on_actionIncluir_livro_triggered()
{
    inclLivro->clear();
    inclLivro->show();
}


void MainLoja::on_actionIncluir_CD_triggered()
{
    inclCD->clear();
    inclCD->show();
}


void MainLoja::on_actionIncluir_DVD_triggered()
{
    inclDVD->clear();
    inclDVD->show();
}

void MainLoja::on_tabelaLIV_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= X.getNumLivro()) return;

    if (QMessageBox::question(this, "Confirmar exclusao", "Deseja excluir o item da tabela?") ==
        QMessageBox::StandardButton::Yes) {
        X.excluirLivro(row);
        exibirLivros();
    }
}

void MainLoja::on_tabelaCD_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= X.getNumCD()) return;

    if (QMessageBox::question(this, "Confirmar exclusao", "Deseja excluir o item da tabela?") ==
        QMessageBox::StandardButton::Yes) {
        X.excluirCD(row);
        exibirCDS();
    }
}

void MainLoja::on_tabelaDVD_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= X.getNumDVD()) return;

    if (QMessageBox::question(this, "Confirmar exclusao", "Deseja excluir o item da tabela?") ==
        QMessageBox::StandardButton::Yes) {
        X.excluirDVD(row);
        exibirDVDS();
    }
}


void MainLoja::on_actionLer_triggered()
{
    QString fileName = QFileDialog::
        getOpenFileName(this,
                        "Open File",
                        QDir::homePath(),
                        "Text Files (*.txt)");

    if (!fileName.isEmpty())
    {
        if(!X.ler(fileName.toStdString())) QMessageBox::critical(this, "Arquivo invalido", "Ocorreu um erro na leitura do arquivo.");

        exibirLivros();
        exibirCDS();
        exibirDVDS();
    }
}

void MainLoja::on_actionSalvar_triggered()
{
    QString fileName = QFileDialog::
        getSaveFileName(this,
                        "Save File",
                        QDir::homePath(),
                        "Text Files (*.txt)");

    if (!fileName.isEmpty())
    {
        if(!X.salvar(fileName.toStdString())) QMessageBox::critical(this, "Arquivo invalido", "Ocorreu um erro no salvamento do arquivo.");
    }
}

void MainLoja::on_actionSair_triggered()
{   
    if (QMessageBox::question(this, "Sair", "Tem certeza que deseja encerrar o programa?") ==
        QMessageBox::StandardButton::Yes) {
        QCoreApplication::quit();
    }
}
