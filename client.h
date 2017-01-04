#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QtNetwork>
#include <QList>
#define MINSIZE 14+2
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
signals:
    void readClient(QString ,QByteArray );
private:
    QTcpSocket *client;
    char type[14];
    QByteArray m_buffer;
//public slots:
//    void readClientSlot();
private slots:
    void readClientSlot();
public:
    QTcpSocket* getClientTcpSocket();//获取每个客户端的地址
    uint getIPv4AddressInt();//获取int类型的ip地址
    QString getIPV4AddressString();
    int state(); //获取状态
    void sendData(QByteArray data);
};

#endif // CLIENT_H
