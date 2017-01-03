#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "common.h"
#include <QTcpSocket>
#include <QList>
#include "tcpserver.h"
#include <QByteArray>
#include <QMessageBox>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    QTimer *sendTimer;

    Ui::Widget *ui;

    QByteArray SendData;
    bool autoRecvSendFlag;//自动接收并发送标志
    bool serverFlag;
signals:
    void sendClientData(QByteArray SendData,QList<QString> ipList);
    void setServerListen(bool,int );
private slots:
    void setClientCheckAllSlot(bool status);//设置客户全选单选
    //void setClientRecvSlot(int row, int column );
    void setLogSlot(const QString &log);
    void sendTextSlot();//给客户端发送数据
    void setAutoSend(bool );//自动发送设置
    void setOpenServerSlot();//打开服务器
   // void autoRecvSendSlot();
    void setAutoRecvSendSlot();//自动处理发送数据的开关
    void setSendHexRegExpSlot(bool);// 设置发送区的输入框匹配模式
public slots:
    void setClient(QList<Client*> clientList);
    void setRecvTextEditSlot(QString ip ,QByteArray readClientData);
};

#endif // WIDGET_H
