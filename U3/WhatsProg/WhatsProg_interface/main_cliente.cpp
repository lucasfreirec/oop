#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include "whatsprog_dados_cliente.h"
#include "whatsprog_main.h"
#include "../MySocket/mysocket.h"
#include "whatsprog_dados_cliente.h"

/// A variavel global que contem todas as msgs de todas as conversas
/// Falta fazer
WhatsProgDadosCliente DC;

/// A variavel global do socket do cliente, a ser utilizado por todas as threads
/// Falta fazer
tcp_mysocket sock;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  WhatsProgMain w;

  mysocket_status iResult = mysocket::init();

  if (iResult != mysocket_status::SOCK_OK) {
      std::cerr << "Biblioteca mysocket nao pode ser inicializada\n";
      exit(-1);
  }
/*
  QApplication::setStyle(QStyleFactory::create("Fusion"));

  // Define uma paleta de cores personalizada
  QPalette palette;
  palette.setColor(QPalette::Window, QColor(Qt::black));
  palette.setColor(QPalette::WindowText, QColor(Qt::white));

  // Aplica a paleta personalizada
  a.setPalette(palette);
*/
  w.show();
  int result = a.exec();
  mysocket::end();

  return result;
}
