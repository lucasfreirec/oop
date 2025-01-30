#include <string>
#include <QMessageBox>
#include "whatsprog_novaconversa.h"
#include "ui_whatsprog_novaconversa.h"
#include "whatsprog_dados_cliente.h"

extern WhatsProgDadosCliente DC;

WhatsProgNovaConversa::WhatsProgNovaConversa(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WhatsProgNovaConversa)
{
  ui->setupUi(this);
}

WhatsProgNovaConversa::~WhatsProgNovaConversa()
{
  delete ui;
}

void WhatsProgNovaConversa::on_buttonBox_accepted()
{
  // Falta fazer:
  // Recupera e testa nome da nova conversa (FEITO)
  std::string conversa = ui->lineEditConversa->text().toStdString();

  if (!testarNomeUsuario(conversa))
  {
      QMessageBox::critical(this, "Erro", "Nome de usuario invalido");
      return;
  }

  // Insere nova conversa na base de dados
  if (!DC.insertConversa(conversa))
  {
      QMessageBox::critical(this, "Erro", "Nova conversa "+ui->lineEditConversa->text()+" invalida");
      return;
  }
  // Faz a conversa recem inserida (a ultima) ser a conversa ativa (FEITO)
  if (!DC.setConversaAtual(conversa))
  {
      QMessageBox::critical(this, "Erro", "Erro ao fazer a conversa "+ui->lineEditConversa->text()+" ser a nova conversa ativa");
      return;
  }

  // Sinaliza que houve alteracao na janelas: Conversas e Mensagens (FEITO)
  emit signAtualizaConversas();
  emit signAtualizaMensagens();
}
