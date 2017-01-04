#ifndef TCPTHRAED_H
#define TCPTHRAED_H
#include <QThread>
#include <QTcpSocket>
class TcpThread: public QThread
{
    Q_OBJECT
public:
    TcpThread(QTcpSocket *client,QObject *parent = 0);
    ~TcpThread();
private:
    QTcpSocket *client;
    int isrunning;
protected:
    void run();
public slots:
    void quitClientThread();
private slots:
    void readClientSlot();
   // void disconnectedSlot();
signals:
    void readClient(QString readClientData);
};

#endif // SERVERTHREAD_H
