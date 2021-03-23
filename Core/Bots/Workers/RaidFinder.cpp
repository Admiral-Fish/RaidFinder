#include "RaidFinder.hpp"
#include "../RaidBot.hpp"

RaidFinder::RaidFinder(QObject *parent) : QThread(parent)
{

}

RaidFinder::~RaidFinder()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void RaidFinder::startScript(QString ipRaw, QString portRaw, int denID, int location, int denType)
{
    QMutexLocker locker(&mutex);

    this->ipRaw = ipRaw;
    this->portRaw = portRaw;
    if(denID == 0)
    {
        this->denID = 1;
        this->denType = 2;
        this->location = 0;
    } else {
        this->denID = denID;
        this->denType = denType;
        this->location = location;
    }

    if(location == 1)
        this->denID = denID + 111;
    else if(location == 2)
        this->denID = denID + 222;

    if(!isRunning())
        start(LowPriority);
    else
    {
        restart = true;
        condition.wakeOne();
    }

}

void RaidFinder::generated(bool results)
{
    genned = true;
    this->results = results;
}

void RaidFinder::run()
{
    genned = false;
    RaidBot *raidBot = new RaidBot(this, &ipRaw, &portRaw);
    raidBot->setTargetDen(denID);
    QByteArray den = raidBot->getDenData();
    if((den.at(0x13) & 1) == 0)
        raidBot->getWatts();

    forever { 
        raidBot->pause(500);
        raidBot->throwPiece();
        den = raidBot->getDenData();
        qDebug() << den.mid(0x8, 8);
        QString seed = den.mid(0x8, 8).toHex();
        if(((den.at(0x12) == 2 || den.at(0x12) == 4) && (denType == 1)) || (((den.at(0x13) & 2) == 2) && (denType == 2)) || ((den.at(0x12) == 1 || den.at(0x12) == 3) && (denType == 0)))
        {
            emit generate(seed);
        } else {
            genned = true;
            results = false;
        }
        while(!genned)
            raidBot->pause(200);
        if(results == false)
        {
            raidBot->notFoundActions();
            raidBot->quitGame(false);
            raidBot->enterGame();
            raidBot->skipIntroAnimation();
        } else {
            //if(!raidBot->foundActions()) {
                return;
            //}
        }

    }
}
