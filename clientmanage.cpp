#include "clientmanage.h"
#include <QHostAddress>

ClientManage::ClientManage(QList<Client*> &clientList):clientList(clientList)
{

}

ClientManage::ClientManage()
{

}
ClientManage::~ClientManage()
{
    qDebug() << "delete ClientManage!";

}
void ClientManage::addClient(Client* client)
{
    qDebug()<<"ClientManage::addClient";

    clientList.append(client);
}
//void ClientManage::deleteClient(int number)
//{

//    qDebug()<< "delete ip:" << clientList[number]->getIPV4AddressString();
//    delete clientList[number];
//   // clientList.removeAt(number);
//}
int ClientManage::getClientSize()
{
    return clientList.size();
}

int ClientManage::deleteClient(QString ip)
{
    int count = 0;
    //找到与IP地址
    for ( int i=0; i < clientList.size(); )
    {
        if ( clientList[i]->getIPv4AddressInt() ==  QHostAddress(ip).toIPv4Address())
        {

            delete clientList[i];
            clientList.removeAt(i);
            count++;
            continue;
        }
        i++;
    }
    for ( int i=0; i < clientList.size(); i++ )
    {

    }
    return count;
}
 Client * ClientManage::findClient(int number)
 {
    if ( clientList.size() < number+1 )
        return NULL;
    return clientList[number];
 }
Client* ClientManage::findClient(QString ip)
{
   // int count = 0;
    //int res = 0;
    Client *client = NULL;
    //找到与IP地址
    for ( int i=0; i < clientList.size(); i++ )
    {
        if ( clientList[i]->getIPv4AddressInt() ==  QHostAddress(ip).toIPv4Address())
        {
            client = clientList[i];
            if (!client)
            {

                return client;

            }
        }
    }
     return NULL;
}
 QList<Client *> ClientManage:: findALL()
 {
    return clientList;
 }
 void ClientManage::deleteClientUnconnect()
 {
     for ( int i=0; i < clientList.size();  )
     {
         qDebug() << "客户端连接:" <<clientList[i]->getIPV4AddressString() <<""<<clientList[i]->state();
         if (QAbstractSocket::UnconnectedState ==
              clientList[i]->state())
         {
             delete clientList[i];
             clientList.removeAt(i);
             i = 0;
             continue;
         }
         i++;
     }
 }
void ClientManage::removeAllClient()
{
    qDebug() << "删除客户端:" <<clientList.size();
    for ( int i=0; i < clientList.size(); i++ )
    {
        delete clientList[i];
    }
    clientList.clear();
}
