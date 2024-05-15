#include "whatsprog_login.h"
#include "ui_whatsprog_login.h"

WhatsProgLogin::WhatsProgLogin(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WhatsProgLogin),
  novoUsuario(false)
{
  ui->setupUi(this);

  ui->lineEditSenhaUsuario->setEchoMode( QLineEdit::Password );
}

WhatsProgLogin::~WhatsProgLogin()
{
  delete ui;
}

void WhatsProgLogin::slotLogin(bool NovoUsuario)
{
  novoUsuario = NovoUsuario;
  if (novoUsuario) setWindowTitle("Usuario - Criar");
  else setWindowTitle("Usuario - Conectar");
  ui->lineEditIpServidor->clear();
  ui->lineEditNomeUsuario->clear();
  ui->lineEditSenhaUsuario->clear();
  show();
}

void WhatsProgLogin::on_buttonBox_accepted()
{
  // Falta fazer (FEITO)
  // Envia sinal com os dados recuperados dos lineEdit e limpa os campos dos lineEdit
  QString IP, login, senha;

  IP = ui->lineEditIpServidor->text();
  login = ui->lineEditNomeUsuario->text();
  senha = ui->lineEditSenhaUsuario->text();

  emit signConectar(IP, login, senha, novoUsuario);

  ui->lineEditIpServidor->clear();
  ui->lineEditNomeUsuario->clear();
  ui->lineEditSenhaUsuario->clear();
}
