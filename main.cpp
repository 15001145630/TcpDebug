#include "widget.h"
#include <QApplication>
#include "tcpserver.h"
#include <QThread>
#include <QObject>
#include <common.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // TcpServer tcpServer(NULL);
    TcpServer* tcpServer = new TcpServer(NULL);
    Widget *w = new Widget();
    QObject::connect(tcpServer, SIGNAL(sendClientAll(QList<Client*>)),
                     w, SLOT(setClient(QList<Client*>)));
    QObject::connect(tcpServer, SIGNAL(readClient(QString  ,QByteArray )),
                     w, SLOT(setRecvTextEditSlot(QString  ,QByteArray )));

    QObject::connect(w, SIGNAL(sendClientData(QByteArray,QList<QString>)),
                     tcpServer, SLOT(sendClientDataSlot(QByteArray,QList<QString>)));

    QObject::connect(w, SIGNAL(setServerListen(bool, int)),
                     tcpServer, SLOT(setServerListenSlot(bool, int)));
    w->show();

    return a.exec();
}
