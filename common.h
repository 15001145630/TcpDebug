#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QtNetwork>
#include <QList>

QString getLocalmachineIp();
int hexToQString(const char* data,int size, QString &hexStr);
char convertHexChar(char ch);
//字符串转Hex
void qStringToHex(const QString& str, QByteArray &senddata);

typedef enum{
    general,//普通的模式
    ret_recv_size,//返回接收到消息的大小

}AUTO_RECV_SEND_TYPES;

#endif // COMMON_H
