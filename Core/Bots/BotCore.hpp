#ifndef BOTCORE_HPP
#define BOTCORE_HPP

#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QWaitCondition>

class BotCore
{

public:
    BotCore(QThread *controllingThread, QString ipRaw = NULL, QString portRaw = NULL);
    ~BotCore();

private:
    QTcpSocket *socket;

    QMutex mutex;
    QWaitCondition condition;
    bool abort = false;
    int ls_lastx;
    int ls_lasty;
    int rs_lastx;
    int rs_lasty;
    QString ip;
    QString port;
    QThread *thread;

    void sendCommand(QString content);
    void configure();
    void detach();
    void close(bool exitapp = true);
    void click(QString button);
    void press(QString button);
    void release(QString button);
    void moveStick(QString button, int x, int y);
    void moveLeftStick(int x = NULL, int y = NULL);
    void moveRightStick(int x = NULL, int y = NULL);
    QByteArray read(QString address, QString size, QString fileName = NULL);
    void write(QString address, QString data);
    void getSystemLanguage();
    void pause(int duration);
};

#endif // BOTCORE_HPP
