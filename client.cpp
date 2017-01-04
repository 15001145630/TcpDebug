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
QTcpSocket* Client::getClientTcpSocket()//��ȡÿ���ͻ��˵ĵ�ַ
{
    return client;
}
uint Client::getIPv4AddressInt()//��ȡint���͵�ip��ַ
{
    return client->peerAddress().toIPv4Address();
}
QString Client::getIPV4AddressString()
{
    return QHostAddress(client->peerAddress().toIPv4Address()).toString();
}
int Client::state() //��ȡ״̬
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

    //������û�����ݣ�ֱ������
    if( client->bytesAvailable() <= 0 )
    {
        return;
    }

    //��ʱ��ôӻ�����ȡ���������ݣ����ǲ�ȷ��ÿ��ȡ�������Ƕ��١�
    QByteArray buffer;
    //������ź�readyRead�����ģ�ʹ��readAllʱ��һ�ΰ���һ�ο��õ�����ȫ�ܶ�ȡ����
    //����ʹ��while(m_tcpClient->bytesAvailable())���岻����ʵִֻ��һ�Ρ�
    buffer = client->readAll();


    //�ϴλ�������������
    /*
   �����н���������������������A��B�����ǹ���ʱ�п�����A+B��B��ʾA��+B����һ�������ݣ�
   Ȼ����B��ʣ�µ����ݣ�������A��A+B��ʾA��һ�������ݣ�Ȼ����A��ʣ�µ�������B����ϡ�
   ���ʱ�����ǽ���ʱ�϶��������һ�������ݣ������ⲿ�����ݶ�����һ������Ч����������
   Ҫ����һ�����������
  */
    m_buffer.append(buffer);

    ushort type_id;
    ushort mesg_len =0;

    char *tempData = NULL;
    int totalLen = m_buffer.size();

    while( totalLen )
    {
        //��QDataStream�󶨣����������
        //QDataStream packet(m_buffer);
        //packet.setByteOrder(QDataStream::BigEndian);

        //������ͷ������ֱ�ӾͲ�����14+2
        if( totalLen < MINSIZE )
        {
            break;
        }
        tempData = m_buffer.data();
       // packet >> type_id >> mesg_len;

        qDebug() << m_buffer;
        //����������ȵȹ����������� +
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

        //�����㹻�࣬���������Ƕ���İ�ͷ�ļ�������

        //������������
        buffer = m_buffer.right(totalLen - mesg_len);

        //���³���
        totalLen = buffer.size();

        //���¶�������
        m_buffer = buffer;

    }
 //   emit readClient(getIPV4AddressString() , temp);

}
