#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <stdio.h>
#include <QObject>
#include <QtNetwork>
#include <QList>
#include "common.h"

#include "clientmanage.h"

using namespace std;

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    virtual ~TcpServer();
private:
    QTcpServer *server;
    QTcpSocket *clientConnection;
    QList<Client*> clientList;
    //QList <QTcpSocket*>clientList;

    ClientManage *clientManage ;
signals:
    void sendClientAll(QList<Client*> clientList);
    void readClient(QString  ,QByteArray );

    //void quitClientThread();

public slots:
    void acceptConnectionSlot();
    void disconnectedSlot();
    void sendClientDataSlot(QByteArray SendData,QList<QString> ipList);
    void setServerListenSlot(bool, int );
};

#endif // TCPSERVER_H
