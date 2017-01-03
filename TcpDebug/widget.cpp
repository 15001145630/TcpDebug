#include "widget.h"
#include "ui_widget.h"
#include <QHBoxLayout>
#include <QRegExp>
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    sendTimer = new QTimer(this);

    serverFlag = true;
    serverFlag = false;
    //控件初始化
    //设置正则表达式

    ui->portLineEdit->setValidator( new QRegExpValidator(QRegExp("[0-9]+$"), this ) );
    ui->frameHeadLineEdit->setValidator(new QRegExpValidator(QRegExp("([0-9a-fA-F]*\\s*)*$"), this ) );
    //设置日志文本框为只读
   // ui->logTextEdit->setReadOnly ( true );
    ui->recvTextEdit->setReadOnly(true);
    //显示本机IP
    ui->ipLabel->setText(getLocalmachineIp());

    ui->clientTableWidget->rowCount();
    //设置clientTableWidget复选框


    connect(ui->checkAllCheckBox, SIGNAL(clicked(bool)), this,SLOT(setClientCheckAllSlot(bool)));
    connect(ui->sendButton, SIGNAL(clicked(bool)), this,SLOT(sendTextSlot()));
    connect(sendTimer, SIGNAL(timeout()), this, SLOT(sendTextSlot()));

    connect(ui->autoSendCheckBox, SIGNAL(clicked(bool)), this, SLOT(setAutoSend(bool) ));
    connect(ui->sendHexCheckBox, SIGNAL(clicked(bool)), this, SLOT(setSendHexRegExpSlot(bool) ));

    connect(ui->openButton, SIGNAL(clicked(bool)), this, SLOT(setOpenServerSlot() ));

    connect(ui->autoRecvSendButton, SIGNAL(clicked(bool)), this, SLOT(setAutoRecvSendSlot() ));

#if 0 //测试代码
    QString str;
    char data[10] ={0x12,0x3,0x55,0x53,0x12,0x34,0x14,0x56,0x93,0xF1} ;
    hexToQString( data,10, str);
    ui->recvTextEdit->setText(str);
    for ( int i=0; i < ui->clientTableWidget->rowCount(); i++ )
    {
        QCheckBox *ckb = new QCheckBox();
        ckb->setChecked(true);
        QHBoxLayout *hLayout = new QHBoxLayout();
        QWidget *widget = new QWidget(ui->clientTableWidget);
        hLayout->addWidget(ckb);
        hLayout->setMargin(0);                          // 必须添加, 否则CheckBox不能正常显示
        hLayout->setAlignment(ckb, Qt::AlignCenter);
        widget->setLayout(hLayout);
        ui->clientTableWidget->setCellWidget(i,1,widget);
    }
#endif

}

Widget::~Widget()
{
    delete ui;
}
void Widget::setClientCheckAllSlot(bool status)
{
    if ( status )
    {
        for ( int i=0; i < ui->clientTableWidget->rowCount(); i++ )
        {
            QWidget *widget = (QWidget *)ui->clientTableWidget->cellWidget(i, 1);
            QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
            ckb->setChecked(true);
        }

    }
    else
    {
        for ( int i=0; i < ui->clientTableWidget->rowCount(); i++ )
        {
            QWidget *widget = (QWidget *)ui->clientTableWidget->cellWidget(i, 1);
            QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
            ckb->setChecked(false);
        }
    }
}


void Widget::setLogSlot(const QString& log)
{
    QString time=  QDateTime::currentDateTime().toString("hh:mm:ss ");
    //ui->logTextEdit->append(time+":"+log);
}

void Widget::setAutoSend(bool autoSendFlag)
{
    if ( autoSendFlag )
    {
        int time = ui->sendTimeSpinBox->value();
        sendTimer->start(time);

        ui->sendButton->setEnabled(false);
    }
    else
    {
        sendTimer->stop();
        ui->autoSendCheckBox->setChecked(false);
        ui->sendButton->setEnabled(true);
    }

}
void Widget::sendTextSlot()
{
    QByteArray sendData;
    QList<QString> ipList;

  //  qDebug()<< "---------------";
    for ( int i=0; i < ui->clientTableWidget->rowCount(); i++ )
    {
       QWidget *widget = (QWidget *)ui->clientTableWidget->cellWidget(i, 1);
       QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
       if ( ckb->isChecked() )
       {
           ipList.append(ui->clientTableWidget->item(i,0)->text());

       }
    }
  //  qDebug()<< "---------------";
    if ( ipList.size() == 0 )
    {
        QMessageBox::information(this, "警告", "发送的客户端个数不能为0!");
        setAutoSend(false);
        return;
    }
    if ( ui->sendLineEdit->text().isEmpty() )
    {
        QMessageBox::information(this, "警告", "发送内容不能为空");
        //sendTimer->stop();
        setAutoSend(false);
        return;
    }

    if ( ui->sendHexCheckBox->isChecked() )//选择16进制发送
    {

        qStringToHex(ui->sendLineEdit->text(), sendData);
    }
    else
    {
        sendData = ui->sendLineEdit->text().toLatin1();
    }

    emit sendClientData(sendData, ipList);
    qDebug()<< "send:" << ipList;

//error:


}

void Widget:: setClient(QList<Client*> clientList)
{
    qDebug() << clientList.size() ;

    ui->clientSizeLabel->setText(QString::number(clientList.size()));
    ui->clientTableWidget->clearContents();//清空列表
    ui->clientTableWidget->setRowCount(clientList.size());//重新设置列表的行个数
    for ( int i=0; i < clientList.size(); i++ )
    {
        QCheckBox *ckb = new QCheckBox();
        ckb->setChecked(true);
        QHBoxLayout *hLayout = new QHBoxLayout();
        QWidget *widget = new QWidget(ui->clientTableWidget);
        hLayout->addWidget(ckb);
        hLayout->setMargin(0);                          // 必须添加, 否则CheckBox不能正常显示
        hLayout->setAlignment(ckb, Qt::AlignCenter);
        widget->setLayout(hLayout);
        ui->clientTableWidget->setCellWidget(i,1,widget);

        ui->clientTableWidget->setItem(i,0,new QTableWidgetItem(QHostAddress( clientList[i]->getIPv4AddressInt()).toString()));
        //setLogSlot(QHostAddress( clientList[i]->peerAddress().toIPv4Address()).toString());
    }

}
void Widget::setRecvTextEditSlot(QString ip ,QByteArray readClientData)
{
    QString strTemp;
    strTemp += "[";
    strTemp += QDateTime::currentDateTime().toString("hh:mm:ss ") ;
    strTemp += " ";
    strTemp += ip;
    strTemp +="]";
    strTemp += ":";
    if ( ui->readhexCheckBox->checkState() )
    {
        QString hexTemp;
        char* str = readClientData.data();
        if ( hexToQString(str, readClientData.size(), hexTemp) < 0)
        {

          qDebug() << "data is error!";
        }
          strTemp += hexTemp;
    }
    else
    {
        strTemp += readClientData;
    }

    ui->recvTextEdit->append(strTemp);

    //判断是否需要自动处理并发送数据
    //autoRecvSendSlot();
    if ( autoRecvSendFlag )
    {
        QByteArray sendData;
        QList<QString> ipList;
        ipList.append(ip);//获取要发送的客户端
        qStringToHex(ui->frameHeadLineEdit->text(), sendData);//帧头转化为16进制
        switch( ui->autoTypeComboBox->currentIndex() )
        {

        case general:

            break;
        case ret_recv_size:
        {
            char sizeArray[4] = {0};
            int Length = readClientData.size();//接收的字节数
            sizeArray[0] = (Length>>24)&0xff;
            sizeArray[1] = (Length>>16)&0xff;
            sizeArray[2] = (Length>>8)&0xff;
            sizeArray[3] = (Length)&0xff;
            sendData.append(sizeArray, sizeof(sizeArray)/sizeof(sizeArray[0]));//在针头后面追加接收字节数
         }
            break;
        default:
            return;
        }
        emit sendClientData(sendData, ipList);
    }

}

void Widget:: setOpenServerSlot()
{

    qDebug() << serverFlag;

    if ( ui->portLineEdit->text().isEmpty() )
    {
        QMessageBox::information(this, "警告", "port不能为空");
        return ;
    }
    serverFlag = !serverFlag;
    if ( serverFlag )
    {
        ui->openButton->setText("关闭");

    }
    else
    {
        ui->openButton->setText("打开");
        ui->portLineEdit->setEnabled(true);
    }
    ui->portLineEdit->setEnabled(!serverFlag);
    emit setServerListen(serverFlag, ui->portLineEdit->text().toInt());
}
void Widget::setAutoRecvSendSlot()
{
    //if ( autoRecvSendFlag  )
    autoRecvSendFlag = !autoRecvSendFlag;
    if ( autoRecvSendFlag )
    {
        ui->autoRecvSendButton->setText("关闭");

    }
    else
    {
        ui->autoRecvSendButton->setText("打开");

    }
    ui->autoTypeComboBox->setEnabled(!autoRecvSendFlag);
    ui->frameHeadLineEdit->setEnabled(!autoRecvSendFlag);
}
void Widget:: setSendHexRegExpSlot(bool flag)
{
    if ( flag )
    {
        ui->sendLineEdit->setValidator(new QRegExpValidator(QRegExp("([0-9a-fA-F]*\\s*)*$"), this ) );
    }
    else
    {
        ui->sendLineEdit->setValidator(new QRegExpValidator(QRegExp("*$"), this ) );
    }
    ui->sendLineEdit->clear();
}
