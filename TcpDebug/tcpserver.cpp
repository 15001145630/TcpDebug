#include "tcpserver.h"
#include <iostream>
#include <QLatin1String>
TcpServer::TcpServer(QObject *parent) : QObject(parent)
{

    clientManage = new ClientManage();
    server = new QTcpServer;
     connect(server,SIGNAL(newConnection()), this, SLOT(acceptConnectionSlot()));
    qDebug() << "creat TcpServer success" << endl;
    qDebug() << "客户量:" << clientManage->getClientSize();
}

TcpServer::~TcpServer()
{
    delete clientManage;

}
void TcpServer::acceptConnectionSlot()
{

    clientConnection = server->nextPendingConnection();
    qDebug()<<clientConnection->state()<<"++" << clientConnection->peerAddress().toString();

    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));

    Client *client = new Client(clientConnection);
    clientManage->addClient(client);
    connect(client, SIGNAL(readClient(QString, QByteArray)), this, SIGNAL(readClient(QString,QByteArray)));

    emit sendClientAll(clientManage->findALL());
}

void TcpServer::disconnectedSlot()
{

    qDebug() << "断开连接";
    clientManage-> deleteClientUnconnect();
    emit sendClientAll(clientManage->findALL());
    qDebug() << "客户量:" << clientManage->getClientSize();
}
void TcpServer::sendClientDataSlot(QByteArray SendData,QList<QString> ipList)
{
    QList<Client*> clientList = clientManage->findALL();
    for ( int i=0; i < ipList.size(); i++ )
    {
        for (int j=0; j < clientManage->getClientSize(); j++ )
        {
            if ( !QString::compare(ipList[i], clientList[j]->getIPV4AddressString()) )
            {
                clientList[j]->sendData(SendData);
                break;
            }
        }

    }

}

void TcpServer::setServerListenSlot(bool serverFlag, int port)
{
    if ( serverFlag )
    {
        server->listen(QHostAddress::Any, port);
        server->setMaxPendingConnections(100);

    }
    else
    {

        clientManage->removeAllClient();
        server->close();
        emit sendClientAll(clientManage->findALL());
    }

}
