#include "whatsprog_thread.h"
#include "../MySocket/mysocket.h"
#include "whatsprog_dados_cliente.h"

using namespace std;

/// A variavel externa (global) que contem todas as msgs de todas as conversas
/// Falta fazer
extern WhatsProgDadosCliente DC;
/// A variavel externa (global) do socket do cliente, a ser utilizado por todas as threads
/// Falta fazer
extern tcp_mysocket sock;

WhatsProgThread::WhatsProgThread(QObject *parent) :
  QObject(parent), thr()
{
}

/// Esta eh a funcao principal da thread
/// Ela recebe as informacoes do socket e armazena na variavel apropriada,
/// de onde elas serao exibidas pela interface visual atraves da emissao de sinais
int WhatsProgThread::main_thread()
{
  // Falta fazer: (FEITO)
  // while (socket conectado) { ... }
  while (sock.connected())
  {
      mysocket_status iResult;
      int32_t cmd;
      int32_t id;
      string remetente;
      string texto;
      iterConversa it;
      int ind_msg;


      iResult = sock.read_int(cmd, 1000*TIMEOUT_WHATSPROG);

      switch (iResult)
      {
          case mysocket_status::SOCK_OK:

          switch(cmd)
          {
              case CMD_NEW_USER:
              case CMD_LOGIN_USER:
              case CMD_LOGIN_OK:
              case CMD_LOGIN_INVALIDO:
              case CMD_LOGOUT_USER:
              default:
                  break;
              case CMD_NOVA_MSG:
                  try
                  {
                      iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
                      if (iResult != mysocket_status::SOCK_OK) throw 1;
                      iResult = sock.read_string(remetente, TIMEOUT_WHATSPROG*1000);
                      if (iResult != mysocket_status::SOCK_OK) throw 2;
                      iResult = sock.read_string(texto, TIMEOUT_WHATSPROG*1000);
                      if (iResult != mysocket_status::SOCK_OK) throw 3;

                      Mensagem M;

                      if (!M.setId(id) ||
                          !M.setRemetente(remetente) ||
                          !M.setDestinatario(DC.getMeuUsuario()) ||
                          !M.setTexto(texto) ||
                          !M.setStatus(MsgStatus::MSG_ENTREGUE)) throw 4;

                      it = DC.findConversa(remetente);

                      if (it == DC.end())
                      {
                          if (!DC.insertConversa(remetente)) throw 5;

                          it = DC.last();

                          signExibirConversas();
                      }

                      DC.pushMessage(it,M);

                      DC.moveConversaToBegin(it);

                      signExibirConversas(); // VERIFICAR
                      signExibirMensagens(); // VERIFICAR
                  }
                  catch (int i)
                  {
                      sock.close();

                      emit signExibirErroMensagem("Erro " + to_string(i) + " apos comando CMD_NOVA_MSG");
                  }
                  break;
              case CMD_MSG_RECEBIDA:
              case CMD_MSG_ENTREGUE:
                  try
                  {
                      iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
                      if (iResult != mysocket_status::SOCK_OK) throw 1;

                      DC.findMensagem(id, it, ind_msg);
                      if (it==DC.end() || ind_msg<0) throw 2;

                      if (it->at(ind_msg).getRemetente()!=DC.getMeuUsuario()) throw 3;

                      if (cmd==CMD_MSG_RECEBIDA)
                      {
                          if (it->at(ind_msg).getStatus()!=MsgStatus::MSG_ENVIADA) throw 4;

                          DC.setStatus(it,ind_msg,MsgStatus::MSG_RECEBIDA);
                      }

                      else {
                          if (it->at(ind_msg).getStatus()!=MsgStatus::MSG_RECEBIDA) throw 5;

                          DC.setStatus(it,ind_msg,MsgStatus::MSG_ENTREGUE);
                      }

                      if (DC.getConversaAtual() != DC.end()) signExibirMensagens();

                  }
                  catch (int i)
                  {
                      sock.close();

                      emit signExibirErroMensagem("Erro " + to_string(i) + " apos comando CMD_MSG_RECEBIDA ou CMD_MSG_ENTREGUE");
                  }
                  break;
              case CMD_MSG_INVALIDA:
                  try
                  {
                      iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
                      if (iResult != mysocket_status::SOCK_OK) throw 1;

                      DC.findMensagem(id, it, ind_msg);
                      if (it==DC.end() || ind_msg<0) throw 2;

                      if (it->at(ind_msg).getRemetente()!=DC.getMeuUsuario() ||
                          it->at(ind_msg).getStatus()!=MsgStatus::MSG_ENVIADA) throw 3;

                      DC.eraseMessage(it,ind_msg);

                      emit signExibirErroMensagem("Mensagem " + to_string(id) + " com erro");
                  }
                  catch (int i)
                  {
                      sock.close();

                      emit signExibirErroMensagem("Erro " + to_string(i) + " apos comando CMD_MSG_INVALIDA");
                  }
                  break;
          }
           break;

          case mysocket_status::SOCK_TIMEOUT:
            if (!DC.salvar())
            {
                 emit signExibirErroMensagem("Erro no salvamento do arquivo "+DC.getNomeArq());
            }
            break;

          case mysocket_status::SOCK_ERROR:
          case mysocket_status::SOCK_DISCONNECTED:
          default:
            sock.close();
            emit signExibirErroMensagem("Erro na leitura do comando no socket");
            break;
      }

  }
  // Coloca a interface em modo desconectado
  // Vai salvar as informacoes do cliente
  emit signDesconectarInterface();

  return 0;
}

/// Funcao que executa o metodo main_thread da classe WhatsProgThread
int main_thread(WhatsProgThread *MT)
{
  return MT->main_thread();
}

/// Iniciar a thread
void WhatsProgThread::slotIniciar()
{
  thr = std::thread(::main_thread, this);
}

/// Terminar a thread
void WhatsProgThread::slotEncerrar(void)
{
  // Encerra o socket
  // Falta fazer (FEITO)
  sock.close();

  // Espera o encerramento da thread
  if (thr.joinable()) thr.join();
  thr = std::thread(); // Thread invalida (nao em execucao)
}
