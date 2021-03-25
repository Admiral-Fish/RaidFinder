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

void RaidFinder::stopScript()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
    emit log("Bot stopped.");
}

void RaidFinder::startScript(QString ipRaw, QString portRaw, int denID, int denType)
{
    QMutexLocker locker(&mutex);

    abort = false;

    this->ipRaw = ipRaw;
    this->portRaw = portRaw;
    if(denID == 65535)
    {
        this->denID = 65535;
        this->denType = 2;
    } else {
        this->denType = denType;
        if(denID > 189)
            this->denID = denID + 32;
        else if(denID > 99)
            this->denID = denID + 11;
        else
            this->denID = denID;
    }

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
    RaidBot raidBot = RaidBot(this, &ipRaw, &portRaw);
    raidBot.setTargetDen(denID == 65535 ? 1 : denID + 1);

    forever { 
        genned = false;
        results = false;
        QByteArray den = raidBot.getDenData();
        if(abort)
            break;
        if((den.at(0x13) & 1) == 0)
        {
            emit log("Den has watts - Getting them...");
            raidBot.getWatts();
        } else
            emit log("No watts in Den");
        if(abort)
            break;
        raidBot.pause(500);
        raidBot.throwPiece();
        if(abort)
            break;
        den = raidBot.getDenData();
        QByteArray seedBE = den.mid(0x8, 8);
        std::reverse(seedBE.begin(), seedBE.end());
        QString seed = seedBE.toHex();

        emit log("Seed: " + seed.toUpper());

        if((den.at(0x12) == 2 || den.at(0x12) == 4))
            emit log("Rare Raid");
        else if ((den.at(0x13) & 2) == 2)
            emit log("Event Raid");
        else
            emit log("Normal Raid");

        if(((den.at(0x12) == 2 || den.at(0x12) == 4) && (denType == 1)) || (((den.at(0x13) & 2) == 2) && (denType == 2)) || ((den.at(0x12) == 1 || den.at(0x12) == 3) && (denType == 0)))
        {
            emit generate(seed);
        } else {
            genned = true;
            results = false;
        }
        while(!genned)
            raidBot.pause(200);
        if(abort)
            break;
        if(results == false)
        {
            raidBot.notFoundActions();
            emit log("Resetting...");
            raidBot.quitGame(false);
            if(abort)
                break;
            raidBot.enterGame();
            if(abort)
                break;
            raidBot.skipIntroAnimation();
        } else {
            emit log("Results Found. Check main window.");
            if(!raidBot.foundActions()) {
                break;
            }
        }

    }

    raidBot.close();
}
