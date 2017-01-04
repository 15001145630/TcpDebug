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
    QTcpSocket* getClientTcpSocket();//��ȡÿ���ͻ��˵ĵ�ַ
    uint getIPv4AddressInt();//��ȡint���͵�ip��ַ
    QString getIPV4AddressString();
    int state(); //��ȡ״̬
    void sendData(QByteArray data);
};

#endif // CLIENT_H
