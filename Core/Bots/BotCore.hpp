#ifndef BOTCORE_HPP
#define BOTCORE_HPP

#include <QThread>
#include <QTcpSocket>

class BotCore : QObject
{
    Q_OBJECT
    Q_ENUMS(SystemLanguage)
public:
    BotCore(QThread *controllingThread, QString *ipRaw = nullptr, QString *portRaw = nullptr);
    ~BotCore();

    enum SystemLanguage {
        JA = 0,
        ENUS = 1,
        FR = 2,
        DE = 3,
        IT = 4,
        ES = 5,
        ZHCN = 6,
        KO = 7,
        NL = 8,
        PT = 9,
        ZHTW = 11,
        ENGB = 12,
        FRCA = 13,
        ES419 = 14,
        ZHHANS = 15,
        ZHHANT = 16
    };

    void sendCommand(QString content);
    void configure();
    void detach();
    void close();
    void closeNoThread();
    void click(QString button);
    void press(QString button);
    void release(QString button);
    void moveStick(QString button, int x, int y);
    void moveLeftStick(int x = NULL, int y = NULL);
    void moveRightStick(int x = NULL, int y = NULL);
    QByteArray read(QString address, QString size, QString fileName = NULL);
    void write(QString address, QString data);
    int getSystemLanguage();
    void pause(int duration);

private:
    QTcpSocket *socket;

    int ls_lastx;
    int ls_lasty;
    int rs_lastx;
    int rs_lasty;
    QString ip;
    QString port;
    QThread *thread;

};

#endif // BOTCORE_HPP
