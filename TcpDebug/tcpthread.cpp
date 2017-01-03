#include "tcpthread.h"
#include <QDebug>
#include <QHostAddress>
#include <QDateTime>
TcpThread::TcpThread(QTcpSocket* client,QObject *parent) :
    QThread(parent),client(client)
{

   // qDebug() << "客户量:" << clientManage->getClientSize();
    connect(client, SIGNAL(readyRead()), this, SLOT(readClientSlot()));
    isrunning = 1;
}
TcpThread::~TcpThread()
{
    qDebug() << "线程销毁";
}
void TcpThread::run(){
    while(isrunning){
        sleep(1);
    }
}

void TcpThread::readClientSlot()
{

    char buf[1024] = {};
    qint64 size = 0;
    size =  client->read(buf, 1024);
    //server->socketDescriptor();
    QString read ;
    read += "[";
    read += QDateTime::currentDateTime().toString("hh:mm:ss ") ;
    read += " ";
    read += QHostAddress( client->peerAddress().toIPv4Address()).toString();
    read +="]";
    read += ":";
    read += QString(QLatin1String(buf,size));

    emit readClient(read);
   // clientConnection->write(buf,1024);
    qDebug()<<"read: "<< buf << endl;

}
void TcpThread:: quitClientThread()
{
    delete this;

}
