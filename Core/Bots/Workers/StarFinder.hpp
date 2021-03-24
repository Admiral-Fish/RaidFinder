#ifndef STARFINDER_HPP
#define STARFINDER_HPP

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class StarFinder : public QThread
{
    Q_OBJECT
public:
    StarFinder(QObject *parent = nullptr);
    ~StarFinder();

    void startScript(QString ipRaw, QString portRaw, int denID, int rarity, int starsMin, int starsMax, int species, bool gmax, int shinyLock);
    void stopScript();

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;

    bool abort = false;
    bool restart = false;

    int denID;
    int location;
    int rarity;
    int starsMin;
    int starsMax;
    int species;
    bool gmax;
    int shinyLock;

    QString ipRaw;
    QString portRaw;

signals:
    void log(QString log);
};

#endif // STARFINDER_HPP
