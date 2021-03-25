#ifndef BOTWORKER_HPP
#define BOTWORKER_HPP

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class BotWorker : public QThread
{
    Q_OBJECT
public:
    BotWorker(QObject *parent = nullptr);
    ~BotWorker();

    void startScript(QString ipRaw, QString portRaw, int script, int denID, int rarity, int starsMin = 0, int starsMax = 0, int species = 0, bool gmax = false, int shinyLock = 0);
    void stopScript();

public slots:
    void generated(bool results);

protected:
    void run() override;


private:
    QMutex mutex;
    QWaitCondition condition;

    bool abort = false;
    bool genned = false;
    bool results = false;
    bool restart = false;

    QString ipRaw;
    QString portRaw;

    int script;

    int denType;
    int denID;
    int starsMin;
    int starsMax;
    int species;
    bool gmax;
    int shinyLock;
    void startRaidFinder();
    void startStarFinder(int rarity, int starsMin, int starsMax, int species, bool gmax, int shinyLock);
    void startWattFarmer();

    void raidFinder();
    void starFinder();
    void wattFarmer();

signals:
    void generate(QString seed);
    void log(QString log);

};

#endif // BOTWORKER_HPP
