#include "BotCore.hpp"
#include <QMetaEnum>
#include <QMetaObject>
#include <QFile>
#include <QApplication>

BotCore::BotCore(QThread *controllingThread, QString *ipRaw, QString *portRaw)
{
    socket = new QTcpSocket();
    ip = *ipRaw;
    port = *portRaw;
    thread = controllingThread;

    socket->connectToHost(ip, portRaw->toUShort());
    if(socket->waitForConnected(3000))
    {
        configure();
        //moveLeftStick(0, 0);
        //moveRightStick(0, 0);
    }
}

BotCore::~BotCore()
{
    closeNoThread();
}

void BotCore::sendCommand(QString content)
{
    QString toSend = content + "\r\n";
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(toSend.toUtf8());
        socket->flush();
    }
}

void BotCore::configure()
{
    sendCommand("configure echoCommands 0");
}

void BotCore::detach()
{
    sendCommand("detachController");
}

void BotCore::close()
{
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        pause(500);
        detach();
        socket->close();
    }
}

void BotCore::closeNoThread()
{
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        detach();
        socket->close();
    }

}

void BotCore::click(QString button)
{
    sendCommand("click " + button);
}

void BotCore::press(QString button)
{
    sendCommand("press " + button);
}

void BotCore::release(QString button)
{
    sendCommand("release " + button);
}

void BotCore::moveStick(QString button, int x, int y)
{
    sendCommand("setStick " + button + " " + QString("0x%1").arg(x, 0, 16) + " " + QString("0x%1").arg(y, 0, 16));
}

void BotCore::moveLeftStick(int x, int y)
{
    if(x != NULL)
        ls_lastx = x;
    if(y != NULL)
        ls_lasty = y;
    moveStick("LEFT", ls_lastx, ls_lasty);
}

void BotCore::moveRightStick(int x, int y)
{
    if(x != NULL)
        rs_lastx = x;
    if(y != NULL)
        rs_lasty = y;
    moveStick("RIGHT", rs_lastx, rs_lasty);
}

QByteArray BotCore::read(QString address, QString size, QString fileName)
{
    sendCommand("peek " + address + " " + size);
    QByteArray temp;
    while(QByteArray::fromHex(QString(temp).toLatin1()).count() < size.toInt(nullptr, 16))
    {
        if(!socket->waitForReadyRead())
        {
            break;
        }
        while(socket->bytesAvailable() > 0)
        {
            temp.append(socket->readAll());
            socket->flush();
        }
    }

    QString read = QString(temp);
    read.truncate(read.length() - 1);
    QByteArray bytes = QByteArray::fromHex(read.toLatin1());
    if(fileName != "")
    {
        QFile f(fileName);
        if (f.open(QIODevice::WriteOnly))
        {
            f.write(bytes);
            f.close();
        }
    }
    return bytes;
}

void BotCore::write(QString address, QString data)
{
    sendCommand("poke " + address + " " + data);
}

void BotCore::pause(int duration)
{
    thread->msleep(duration);
}

int BotCore::getSystemLanguage()
{
    sendCommand("getSystemLanguage");
    QByteArray temp;
    while(QByteArray::fromHex(QString(temp).toLatin1()).count() < 1)
    {
        if(!socket->waitForReadyRead())
        {
            break;
        }
        while(socket->bytesAvailable() > 0)
        {
            temp.append(socket->readAll());
            socket->flush();
        }
    }
    QString read = QString(temp);
    read.truncate(read.length() - 1);
    return read.toInt(nullptr, 16);

}
