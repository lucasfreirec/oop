#include <QWindow>
#include <QMessageBox>
#include <iostream>
#include "whatsprog_main.h"
#include "ui_whatsprog_main.h"
#include "../MySocket/mysocket.h"
#include "whatsprog_dados_cliente.h"

using namespace std;

/// Precisa criar ou declarar como "extern"
/// a variavel global que contem todas as msgs de todas as conversas
extern WhatsProgDadosCliente DC;

/// Precisa criar ou declarar como "extern"
/// o socket do cliente, a ser utilizado por todas as threads
extern tcp_mysocket sock;

iterConversa it;

/// Construtor da janela principal da interface
WhatsProgMain::WhatsProgMain(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::WhatsProgMain),
  thread(nullptr),
  loginDialog(nullptr),
  novaConversa(nullptr),
  msgStatus(nullptr),
  pixEnviada(),
  pixRecebida(),
  pixEntregue(),
  pixOther()
{
  ui->setupUi(this);

  // Cria novo objeto gerenciador da thread
  thread = new WhatsProgThread(this);

  // Cria caixas de dialogos de login e de nova conversa
  loginDialog = new WhatsProgLogin(this);
  novaConversa = new WhatsProgNovaConversa(this);

  // A lista da esquerda (conversas)
  ui->tableConversas->setColumnCount(2);
  ui->tableConversas->setShowGrid(false);
  ui->tableConversas->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableConversas->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->tableConversas->setTabKeyNavigation(false);
  ui->tableConversas->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
  ui->tableConversas->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
  ui->tableConversas->horizontalHeader()->setSectionsClickable(false);
  // O cabecalho
  ui->tableConversas->setStyleSheet("QHeaderView::section { text-align:center }"
                                    "QHeaderView::section { font: bold }"
                                    "QHeaderView::section { background-color:lightgray }");
  ui->tableConversas->setHorizontalHeaderLabels(QStringList() << "Num" <<  "Usuario");

  // A lista da direita (mensagens)
  ui->tableMensagens->setColumnCount(3);
  ui->tableMensagens->setShowGrid(true);
  ui->tableMensagens->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableMensagens->setSelectionMode(QAbstractItemView::NoSelection);
  ui->tableMensagens->setTabKeyNavigation(false);
  ui->tableMensagens->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
  ui->tableMensagens->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
  ui->tableMensagens->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
  ui->tableMensagens->horizontalHeader()->setSectionsClickable(false);
  // O cabecalho
  ui->tableMensagens->setStyleSheet("QHeaderView::section { text-align:center }"
                                    "QHeaderView::section { font: bold }"
                                    "QHeaderView::section { background-color:lightgray }");
  ui->tableMensagens->setHorizontalHeaderLabels(QStringList() << "Id" << "Mensagem" <<  "St");

  // Os icones do status das mensagens
  QString dir = ".\\";
  QString pixfile;

  pixfile = dir+"status0.png";
  if (!pixEnviada.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  pixfile = dir+"status1.png";
  if (!pixRecebida.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  pixfile = dir+"status2.png";
  if (!pixEntregue.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  pixfile = dir+"status_other.png";
  if (!pixOther.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }

  // O icone da aplicacao
  QPixmap pixIcon;
  pixfile = dir+"whatsprog_icon.png";
  if (!pixIcon.load(pixfile,"PNG"))
  {
    cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  else
  {
    setWindowIcon(QIcon(pixIcon));
  }

  // A conexao dos sinais e slots

  // Os sinais da WhatsProgMain
  connect(this, &WhatsProgMain::signLogin,
          loginDialog, &WhatsProgLogin::slotLogin);
  connect(this, &WhatsProgMain::signShowNovaConversa,
          novaConversa, &WhatsProgNovaConversa::show);
  connect(this, &WhatsProgMain::signIniciarThread,
          thread, &WhatsProgThread::slotIniciar);
  connect(this, &WhatsProgMain::signEncerrarThread,
          thread, &WhatsProgThread::slotEncerrar);

  // Os sinais da WhatsProgLogin
  connect(loginDialog, &WhatsProgLogin::signConectar,
          this, &WhatsProgMain::slotConectar);

  // Os sinais da WhatsProgNovaConversa
  connect(novaConversa, &WhatsProgNovaConversa::signAtualizaConversas,
          this, &WhatsProgMain::slotExibirConversas);
  connect(novaConversa, &WhatsProgNovaConversa::signAtualizaMensagens,
          this, &WhatsProgMain::slotExibirMensagens);

  // Os sinais da WhatsProgThread
  connect(thread, &WhatsProgThread::signExibirConversas,
          this, &WhatsProgMain::slotExibirConversas);
  connect(thread, &WhatsProgThread::signExibirMensagens,
          this, &WhatsProgMain::slotExibirMensagens);
  connect(thread, &WhatsProgThread::signExibirErroMensagem,
          this, &WhatsProgMain::slotExibirErroMensagem);
  connect(thread, &WhatsProgThread::signDesconectarInterface,
          this, &WhatsProgMain::on_actionDesconectar_triggered);

  // A barra de status
  msgStatus = new QLabel("?");
  statusBar()->addWidget(msgStatus);

  // Coloca a interface em modo desconectado
  on_actionDesconectar_triggered();
}

WhatsProgMain::~WhatsProgMain()
{
  delete ui;
}

/// Inicia a thread (emite o sinal apropriado)
void WhatsProgMain::iniciarThread()
{
  emit signIniciarThread();
}

/// Encerra a thread (emite o sinal apropriado)
void WhatsProgMain::encerrarThread()
{
  emit signEncerrarThread();
}

/// Redesenha a janela de conversas
void WhatsProgMain::slotExibirConversas()
{
  // Redesenha toda a tabela de conversas
  // O redesenho eh diferente caso o cliente esteja conectado ou nao.
  //
  // Testa se o socket estah conectado e
  // os dados de conexao estao corretamente definidos
  // Falta fazer: provisoriamente, teste sempre false (desconectado) (FEITO)
  if (sock.connected() && DC.connected())
  {
    // Habilita o menu Conversas
    ui->menuConversa->setEnabled(true);

    // Janela de conversas deve exibir o conteudo
    // Fixa o numero de linhas
    // Linhas que a tabela contem atualmente
    int oldRowCount = ui->tableConversas->rowCount();
    // Linhas que a tabela deve passar a ter
    // Falta fazer: provisoriamente, sempre zero (FEITO)
    int newRowCount = DC.size();
    // Variaveis auxiliares
    QLabel *prov;
    QFont font;
    int i,j;
    // Verifica se o tamanho da tabela estah correto
    // Se nao estiver, modifica
    if (oldRowCount != newRowCount)
    {
      ui->tableConversas->setRowCount(newRowCount);
      if (oldRowCount < newRowCount)
      {
        // Cria os itens das linhas adicionais da tabela
        for (i=oldRowCount; i<newRowCount; ++i) for (j=0; j<2; ++j)
        {
          prov = new QLabel;
          // primeira coluna (j==0) -> numero de mensagens da conversa
          // segunda coluna (j==1)  -> nome do correspondente da conversa
          if (j==0) prov->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          else prov->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
          ui->tableConversas->setCellWidget(i, j, prov);
        }
      }
    }
    // Fixa o conteudo de todas as linhas da tabela
    for (i=0, it = DC.begin(); i<newRowCount; ++i, ++it)
    {
      // Testa se a conversa eh a conversa selecionada. Se sim:
      // - faz a variavel DC
      // - coloca o texto em negrito
      // Falta fazer: provisoriamente, teste sempre false (nao eh a conversa selecionada)
      if (it == DC.getConversaAtual()) // ?????????????????????? VERIFICAR EM SALA COM O PROFESSOR ?????????????????
      {
        ui->tableConversas->setCurrentCell(i,1,QItemSelectionModel::ClearAndSelect);
        ui->tableConversas->setRangeSelected(QTableWidgetSelectionRange(i,0,i,1),true);
        font.setBold(true);
      }
      else
      {
        font.setBold(false);
      }
      // Atualiza as duas colunas da linha da tabela
      for (j=0; j<2; ++j)
      {
        // Recupera o label da celula da tabela
        prov = (QLabel*)ui->tableConversas->cellWidget(i,j);
        // Fixa o tipo de letra (negrito ou normal)
        prov->setFont(font);
        // Altera o conteudo do item
        // primeira coluna (j==0) -> numero de mensagens da conversa
        // segunda coluna (j==1)  -> nome do correspondente da conversa
        if (j==0)
        {
          // Numero de msgs da conversa
          // Falta fazer: provisoriamente, sempre 0 (FEITO)
        prov->setText(QString::number(it->size()));
        }
        else
        {
          // Correspondente da conversa
          // Falta fazer: provisoriamente, sempre "user" (FEITO)
        prov->setText(QString::fromStdString(it->getCorrespondente()));
        } // fim do if (j==0)
      }
    }
    // Atualizar a exibicao da tabela
    ui->tableConversas->viewport()->update();
  }  // fim do if socket connectado ...
  else // Cliente estah desconectado
  {
    // Desabilita o menu Conversas
    ui->menuConversa->setEnabled(false);

    // A janela de conversas deve ficar vazia
    ui->tableConversas->clearContents();
    ui->tableConversas->setRowCount(0);
  }  // fim do else socket connectado ...
}

/// Redesenha a janela de mensagens
void WhatsProgMain::slotExibirMensagens()
{
  // Redesenha toda a tabela de mensagens
  // O redesenho eh diferente caso o (cliente esteja conectado e
  // uma conversa esteja selecionada) ou nao.
  //
  // Testa se o socket estah conectado,
  // os dados de conexao estao corretamente definidos e
  // uma conversa estah selecionada
  // Falta fazer: provisoriamente, teste sempre false (desconectado) (FEITO????)
  if (sock.connected() && DC.connected() && DC.getConversaAtual() != DC.end()) // VERIFICAR A ULTIMA CONDICAO!!!!!!!!!!!!!!!!!!!
  {
    // Habilita o menu Mensagens
    ui->menuMensagens->setEnabled(true);

    // Habilita a janela de digitacao de mensagens
    ui->lineEditMensagem->setEnabled(true);

    // Titulo da janela de mensagens
    // Falta fazer: provisoriamente, nome da conversa sempre "user" (FEITO)
    string titulo = string("CONVERSA: ")+ DC.getConversaAtual()->getCorrespondente();
    ui->labelMensagens->setText(QString::fromStdString(titulo));

    // Janela de mensagens deve exibir o conteudo
    // Fixa o numero de linhas
    // Linhas que a tabela contem atualmente
    int oldRowCount = ui->tableMensagens->rowCount();
    // Linhas que a tabela deve passar a ter
    // Falta fazer: provisoriamente, sempre zero (FEITO)
    int newRowCount = DC.getConversaAtual()->size();
    QLabel *prov;
    int i;

    // Verifica se o tamanho da tabela estah correto
    // Se nao estiver, modifica
    if (oldRowCount != newRowCount)
    {
      ui->tableMensagens->setRowCount(newRowCount);
      if (oldRowCount < newRowCount)
      {
        // Cria os labels e preenche as linhas adicionais da tabela
        for (i=oldRowCount; i<newRowCount; ++i)
        {
          // Cria o label da coluna Id (esquerda)
          prov = new QLabel;
          prov->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          ui->tableMensagens->setCellWidget(i, 0, prov);
          // Cria o label da coluna de texto (meio)
          prov = new QLabel;
          // Nao fixa o alinhamento pq ele eh variavel
          ui->tableMensagens->setCellWidget(i, 1, prov);
          // Cria o label da coluna de status (direita)
          prov = new QLabel;
          prov->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
          ui->tableMensagens->setCellWidget(i, 2, prov);
        }
      }
    }
    // Fixa o conteudo dos labels de todas as linhas da tabela
    for (i=0; i<newRowCount; ++i)
    {
      // Recupera o label da celula esquerda da tabela
      prov = (QLabel*)ui->tableMensagens->cellWidget(i,0);
      // Id da mensagem
      // Falta fazer: provisoriamente, sempre zero (FEITO)
      prov->setText(QString::number(DC.getConversaAtual()->at(i).getId()));
      // Cor de fundo
      // Falta fazer: provisoriamente, sempre false (nao eh de minha autoria) (FEITO)
      if (DC.getConversaAtual()->at(i).getRemetente() == DC.getMeuUsuario())
      {
        // Mensagem minha: fundo verde
        prov->setStyleSheet("background-color: #EEFFEE");
      }
      else
      {
        // Mensagem do outro: fundo branco
        prov->setStyleSheet("background-color: white");
      }

      // Recupera o label da celula central da tabela
      prov = (QLabel*)ui->tableMensagens->cellWidget(i,1);
      // Texto da mensagem
      // Falta fazer: provisoriamente, sempre "txt" (FEITO)
      prov->setText(QString::fromStdString(DC.getConversaAtual()->at(i).getTexto()));
      // Cor de fundo e alinhamento da mensagem
      // Falta fazer: provisoriamente, sempre false (nao eh de minha autoria) (FEITO)
      if (DC.getConversaAtual()->at(i).getRemetente() == DC.getMeuUsuario())
      {
        // Mensagem minha: fundo verde
        prov->setStyleSheet("background-color: #EEFFEE");
        // Alinhamento aa direita
        prov->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
      }
      else
      {
        // Mensagem do outro: fundo branco
        prov->setStyleSheet("background-color: white");
        // Alinhamento aa esquerda
        prov->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
      }

      // Recupera o label da celula direita da tabela
      prov = (QLabel*)ui->tableMensagens->cellWidget(i,2);
      // Status da mensagem
      // Falta fazer: provisoriamente, sempre false (nao eh de minha autoria)
      if (DC.getConversaAtual()->at(i).getRemetente() == DC.getMeuUsuario())
      {
        // Mensagem minha: exibir status
        // Leva em conta o status da msg (MSG_ENVIADA, MSG_RECEBIDA, etc.)
        // Exibe (setPixmap) o pixmap correspondente (pixEnviada, pixRecebida, etc.)
        // Falta fazer: provisoriamente, sempre exibe "?" (FEITO)
        switch(DC.getConversaAtual()->at(i).getStatus())
        {
        case MsgStatus::MSG_ENVIADA:
          prov->setPixmap(pixEnviada); break;
        case MsgStatus::MSG_RECEBIDA:
          prov->setPixmap(pixRecebida); break;
        case MsgStatus::MSG_ENTREGUE:
          prov->setPixmap(pixEntregue); break;
        default:
        case MsgStatus::MSG_INVALIDA:
          prov->setText("?"); break;
        }
      }
      else
      {
        // Mensagem do outro
        prov->setPixmap(pixOther);;
      }
    }  // fim for
    ui->tableMensagens->viewport()->update();
  }  // fim do if socket conectado ...
  else // Cliente estah desconectado ou nao hah conversa selecionada
  {
    // Desabilita o menu Mensagens
    ui->menuMensagens->setEnabled(false);

    // Desabilita a janela de digitacao de mensagens
    ui->lineEditMensagem->setEnabled(false);

    // Titulo da janela de mensagens
    ui->labelMensagens->setText("NENHUMA CONVERSA SELECIONADA");

    // A janela de mensagens deve ficar vazia
    ui->tableMensagens->clearContents();
    ui->tableMensagens->setRowCount(0);
  }
}

/// Redesenha a barra de status
void WhatsProgMain::atualizaEstadoConexao()
{
  // Atualiza a msg na barra de status
  // A atualizacao o eh diferente caso o cliente esteja conectado ou nao.
  //
  // Testa se o socket estah conectado e
  // os dados de conexao estao corretamente definidos
  // Falta fazer: provisoriamente, teste sempre false (desconectado) (FEITO)
  if (sock.connected() && DC.connected())
  {
    // Atualiza a barra de status
    // Falta fazer: provisoriamente, usuario sempre "user" e servidor sempre "server" (FEITO)
    string msg = string("CONECTADO: ") + DC.getMeuUsuario() + "@" + DC.getServidor();
    msgStatus->setText(QString::fromStdString(msg));
    // Habilita a acao desconectar
    ui->actionDesconectar->setEnabled(true);
    // Desabilita as acoes Novo e Usuario existente
    ui->actionNovo_usuario->setEnabled(false);
    ui->actionUsuario_existente->setEnabled(false);
  }
  else
  {
    // Atualiza a barra de status
    msgStatus->setText("NAO CONECTADO");
    // Desabilita a acao desconectar
    ui->actionDesconectar->setEnabled(false);
    // Habilita as acoes Novo e Usuario existente
    ui->actionNovo_usuario->setEnabled(true);
    ui->actionUsuario_existente->setEnabled(true);
  }
}

/// Conecta-se ao servidor
void WhatsProgMain::slotConectar(const QString &IP, const QString &login,
                                 const QString &senha, bool novoUsuario )
{
  // Testa todos os parametros
  // Falta fazer (FEITO)
  string Ip = IP.toStdString(), Login = login.toStdString(), Senha = senha.toStdString();

  if (Ip.size() == 0 || !testarNomeUsuario(Login) || !testarSenha(Senha))
  {
    QString mensagem_erro;

        if (Ip.size() == 0) mensagem_erro = "IP: Endereco vazio";
        else if (!testarNomeUsuario(Login)) mensagem_erro = "Nome de usuario: Parametro invalido";
        else mensagem_erro = "Senha: Parametro invalido";

          QMessageBox::critical(this, "Erro", mensagem_erro);
        return;
  }


  // Coloca a interface em modo desconectado, se jah nao estiver
  on_actionDesconectar_triggered();

  // Depois faz a conexao com servidor em um bloco try:
  try
  {
    if(sock.connected()) throw 1;
        //if(thr.joinable()) throw 2; PRECISA TESTAR ISSO? SE SIM, COMO?

    int32_t cmd;

    if (sock.connect(Ip, PORTA_WHATSPROG) != mysocket_status::SOCK_OK) throw 3;
    // Envia comando, recebe resposta do servidor
    // Falta fazer (FEITO)
    if(novoUsuario)
    {
      if(sock.write_int(CMD_NEW_USER) != mysocket_status::SOCK_OK) throw 4;
    }

    else
    {
      if (sock.write_int(CMD_LOGIN_USER) != mysocket_status::SOCK_OK) throw 5;
    }

    if (sock.write_string(Login) != mysocket_status::SOCK_OK) throw 6;

    if (sock.write_string(Senha) != mysocket_status::SOCK_OK) throw 7;

    if (sock.read_int(cmd,1000*TIMEOUT_WHATSPROG) != mysocket_status::SOCK_OK) throw 8;

    if (cmd != CMD_LOGIN_OK) throw 9;

    // Armazena o novo nome do servidor e do usuario,
    // Falta fazer (FEITO)
    DC.setServidorUsuario(Ip,Login);
    // Leh o arquivo com os dados de conexao anterior, caso exista,
    // Falta fazer (FEITO)
    DC.ler();
    // Lanca a thread de leitura dos dados do socket
    emit signIniciarThread();

    // Se soh tiver uma conversa, seleciona-a como ativa
    // Falta fazer (FEITO)
    if (DC.size() == 1) DC.setConversaAtual(DC.begin());
    // Redesenha todas as janelas
    slotExibirConversas();
    slotExibirMensagens();
    atualizaEstadoConexao();

    // Informa que o novo usuario estah conectado
    QMessageBox::information(this, "Login", "Usuario conectado.");
  }
  catch (int i)
  {
    // Desconecta 0 socket
    // Falta fazer (FEITO)
    sock.close();
    // Mensagem de erro
    string msg = "Erro " + to_string(i) + " na conexao com servidor";
    QMessageBox::warning(this, "Erro de conexao", QString::fromStdString(msg));
  }
}

/// Exibe um pop-up com mensagem de erro
void WhatsProgMain::slotExibirErroMensagem(string S)
{
  QMessageBox::warning(this, "WhatsProg - Erro", QString::fromStdString(S));
}

void WhatsProgMain::on_actionNovo_usuario_triggered()
{
  // Exibe a janela de login para novo usuario (true)
  emit signLogin(true);
}

void WhatsProgMain::on_actionUsuario_existente_triggered()
{
  // Exibe a janela de login para usuario existente (false)
  emit signLogin(false);
}

/// Coloca a interface em modo desconectado
/// Desconecta o socket, limpa o servidor e usuario atuais,
/// limpa todas as conversas, redesenha todas as janelas
void WhatsProgMain::on_actionDesconectar_triggered()
{
  // Envia o comando de logout para o servidor, se for o caso
  // Falta fazer (FEITO)
  if (sock.connected()) sock.write_int(CMD_LOGOUT_USER);
  // Fecha o socket
  // Falta fazer (FEITO)
  sock.close();
  // Salva as informacoes do cliente
  // Falta fazer (FEITO)
  DC.salvar();
  // Encerra a thread de leitura de dados do socket
  emit signEncerrarThread();

  // Limpa os nomes do servidor e usuario
  // Falta fazer (FEITO)
  DC.unsetServidorUsuario();
  // Redesenha todas as janelas
  slotExibirConversas();
  slotExibirMensagens();
  atualizaEstadoConexao();
}

void WhatsProgMain::on_actionSair_triggered()
{
  QCoreApplication::quit();
}

void WhatsProgMain::on_actionNova_conversa_triggered()
{
  emit signShowNovaConversa();
}

void WhatsProgMain::on_actionRemover_conversa_triggered()
{
  // Testa se alguma conversa estah selecionada
  // Falta fazer
  if (DC.getConversaAtual() == DC.end()) return; // FEITO? O QUE PRECISA FAZER CASO NAO ESTEJA SELECIONADA?
  // Remove conversa atual
  // Falta fazer
  if (!DC.eraseConversa(DC.getConversaAtual())) // FEITO? PERGUNTAR
  {
    // EM CASO DE FALSE, FAZ O QUE?
  }
  // Sinaliza que houve alteracao na janela de Conversas
  slotExibirConversas();
  // Sinaliza que houve alteracao na janela de Mensagens
  slotExibirMensagens();
}

void WhatsProgMain::on_actionApagar_mensagens_triggered()
{
  // Testa se alguma conversa estah selecionada
  // Falta fazer
  if (DC.getConversaAtual() == DC.end()) return; // FEITO? O QUE PRECISA FAZER CASO NAO ESTEJA SELECIONADA?
  // Remove mensagens da conversa atual
  // Falta fazer (FEITO)
  DC.clearMessages(DC.getConversaAtual());
  // Sinaliza que houve alteracao no numero de msgs de uma conversa
  slotExibirConversas();
  // Sinaliza que houve alteracao na janela de Mensagens
  slotExibirMensagens();
}

void WhatsProgMain::on_tableConversas_activated(const QModelIndex &index)
{
  on_tableConversas_clicked(index);
}

void WhatsProgMain::on_tableConversas_clicked(
    const QModelIndex &index)
{
  // Fixa a nova conversa selecionada
  // Falta fazer (COMO ASSIM?)
  int i=0;
  int linha = index.row();
  if (linha < DC.size())
  {
    it = DC.begin();
    while (i != linha)
    {
      ++it;
      ++i;
    }

    if (it->getCorrespondente() != DC.getConversaAtual()->getCorrespondente())
    {
      DC.setConversaAtual(it);
    }
  }


  // Sinaliza que houve alteracao na conversa selecionada
  slotExibirConversas();
  // Exibe as mensagens da conversa selecionada
  slotExibirMensagens();
}

void WhatsProgMain::on_lineEditMensagem_returnPressed()
{
  // Testa se estah conectado
  // Falta fazer
  if (!sock.connected() || !DC.connected()) return; // QUAL O COMPORTAMENTO DESSE SLOT QUANDO NAO ESTIVER? PERGUNTAR!!!!
  // Testa se uma conversa estah selecionada
  // Falta fazer
  if (DC.getConversaAtual() == DC.end()) return; // MESMA COISA DO IF ACIMA
  // Leh e testa se o texto digitado eh valido para mensagem
  // Falta fazer
  string mensagem = (ui->lineEditMensagem->text()).toStdString();

  if (!testarTextoMensagem(mensagem)) return; // MESMA COISA DOS IFs ACIMA
  // Limpa o conteudo digitado
  ui->lineEditMensagem->clear();

  // Cria a mensagem para envio e armazenamento
  // Falta fazer (FEITO)
  Mensagem msg;

  try
  {
    // Define os campos da msg
    // Falta fazer (FEITO)
    if (!msg.setId(DC.getNovaIdMensagem()) ||
        !msg.setRemetente(DC.getMeuUsuario()) ||
        !msg.setDestinatario((*DC.getConversaAtual()).getCorrespondente()) ||
        !msg.setTexto(mensagem) ||
        !msg.setStatus(MsgStatus::MSG_ENVIADA)) throw 1;
    // Inclui a msg na base de dados local, acrescentando no final da conversa
    // Falta fazer (FEITO)
    DC.pushMessage(DC.getConversaAtual(), msg);
    // Move a conversa com a nova msg para o inicio da lista (begin)
    // Falta fazer (FEITO)
    DC.moveConversaToBegin(DC.getConversaAtual());
    // Sinaliza que houve alteracao no numero de msgs de uma conversa
    slotExibirConversas();

    // Sinaliza que houve alteracao na janela de Mensagens
    slotExibirMensagens();

    // Envia a msg via socket para o servidor
    // Falta fazer (FEITO)
    if (sock.write_int(CMD_NOVA_MSG) != mysocket_status::SOCK_OK) throw 2;
    if (sock.write_int(msg.getId()) != mysocket_status::SOCK_OK) throw 3;
    if (sock.write_string(msg.getDestinatario()) != mysocket_status::SOCK_OK) throw 4;
    if (sock.write_string(msg.getTexto()) != mysocket_status::SOCK_OK) throw 5;
  }
  catch (int i)
  {
    // Desconecta
    // Falta fazer (FEITO)
    sock.close();
    // Remove ultima msg da conversa atual
    // Falta fazer
    if (i>1) DC.popMessage(DC.getConversaAtual());

    slotExibirErroMensagem("Erro " + to_string(i) + " no envio da mensagem");
    // Coloca a interface em modo desconectado
    on_actionDesconectar_triggered();

    // Mensagem de erro
    string msg = "Erro " + to_string(i) + " no envio da mensagem";
    QMessageBox::warning(this, "Erro de envio", QString::fromStdString(msg));
  }
}
