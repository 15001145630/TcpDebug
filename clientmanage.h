#ifndef CLIENTMANAGE_H
#define CLIENTMANAGE_H
#include <QList>
#include "common.h"
#include "client.h"
#include <QTcpSocket>
class ClientManage
{
public:
    ClientManage(QList<Client*> &clientList);
    ClientManage();
    ~ClientManage();

    void addClient(Client* client);

    int deleteClient(QString ip);
    int getClientSize();
    Client *findClient(int number);
    Client *findClient(QString ip);
    QList<Client *> findALL();
    void deleteClientUnconnect();
    void removeAllClient();
private:
    QList<Client*> clientList;
};

#endif // CLIENTMANAGE_H
