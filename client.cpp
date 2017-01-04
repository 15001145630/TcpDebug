#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    this->client = client;
      type[0] = 0xff;
      type[1] =0X00;
      type[2] =0XEE;
      type[3] = 0X11;
      type[4] =0XDD;
      type[5] =0X22;
      type[6] = 0XCC;
      type[7] =0X33;
      type[8] =0XBB;
      type[9] = 0X44;
      type[10] =0XAA;
      type[11] =0X55;
      type[12] =0X99;
      type[13] =0X66;
   connect(client, SIGNAL(readyRead()), this, SLOT(readClientSlot()));
}
Client::~Client()
{
}
QTcpSocket* Client::getClientTcpSocket()//获取每个客户端的地址
{
    return client;
}
uint Client::getIPv4AddressInt()//获取int类型的ip地址
{
    return client->peerAddress().toIPv4Address();
}
QString Client::getIPV4AddressString()
{
    return QHostAddress(client->peerAddress().toIPv4Address()).toString();
}
int Client::state() //获取状态
{
    return client->state();
}
void Client::sendData(QByteArray data)
{
    int size = client->write(data);
    qDebug() << getIPV4AddressString() << ":send Data:" << data;
}
void Client::readClientSlot()
{

    //缓冲区没有数据，直接无视
    if( client->bytesAvailable() <= 0 )
    {
        return;
    }

    //临时获得从缓存区取出来的数据，但是不确定每次取出来的是多少。
    QByteArray buffer;
    //如果是信号readyRead触发的，使用readAll时会一次把这一次可用的数据全总读取出来
    //所以使用while(m_tcpClient->bytesAvailable())意义不大，其实只执行一次。
    buffer = client->readAll();


    //上次缓存加上这次数据
    /*
   上面有讲到混包的三种情况，数据A、B，他们过来时有可能是A+B、B表示A包+B包中一部分数据，
   然后是B包剩下的数据，或者是A、A+B表示A包一部分数据，然后是A包剩下的数据与B包组合。
   这个时候，我们解析时肯定会残留下一部分数据，并且这部分数据对于下一包会有效，所以我们
   要和下一包组合起来。
  */
    m_buffer.append(buffer);

    ushort type_id;
    ushort mesg_len =0;

    char *tempData = NULL;
    int totalLen = m_buffer.size();

    while( totalLen )
    {
        //与QDataStream绑定，方便操作。
        //QDataStream packet(m_buffer);
        //packet.setByteOrder(QDataStream::BigEndian);

        //不够包头的数据直接就不处理。14+2
        if( totalLen < MINSIZE )
        {
            break;
        }
        tempData = m_buffer.data();
       // packet >> type_id >> mesg_len;

        qDebug() << m_buffer;
        //如果不够长度等够了在来解析 +
        mesg_len =  ((tempData[14]<<8)&0xff)+((tempData[15])&0xff);
        mesg_len = mesg_len+14+sizeof(ushort);
        //qDebug() << mesg_len << " | " << tempData[14] << "  " <<tempData[15];

        if( totalLen < mesg_len )
        {
            break;
        }
        int i;
        for (  i=0; i<14; i++ )
        {
            //m_buffer[i]
            if ( type[i] != tempData[i] )
            {
                break;
            }
        }

        if ( i == 14 )
        {

            tempData = tempData +14+2;
            QByteArray temp(tempData, mesg_len-16);
            qDebug() << temp;
            emit readClient(getIPV4AddressString() , temp);
        }
        //m_buffer.

        //数据足够多，且满足我们定义的包头的几种类型

        //缓存多余的数据
        buffer = m_buffer.right(totalLen - mesg_len);

        //更新长度
        totalLen = buffer.size();

        //更新多余数据
        m_buffer = buffer;

    }
 //   emit readClient(getIPV4AddressString() , temp);

}
