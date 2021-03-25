#include "BotWorker.hpp"
#include "../RaidBot.hpp"
#include "../../../Core/Loader/DenLoader.hpp"
#include "../../Results/Den.hpp"
#include "../../Util/Translator.hpp"

BotWorker::BotWorker(QObject *parent) : QThread(parent)
{

}

BotWorker::~BotWorker()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void BotWorker::stopScript()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
    emit log("Bot stopped.");
}

void BotWorker::startScript(QString ipRaw, QString portRaw, int script, int denID, int rarity, int starsMin, int starsMax, int species, bool gmax, int shinyLock)
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
        this->denType = rarity;
        if(denID > 189)
            this->denID = denID + 32;
        else if(denID > 99)
            this->denID = denID + 11;
        else
            this->denID = denID;
    }

    this->script = script;

    switch(script)
    {
    case 0:
        startRaidFinder();
        break;
    case 1:
        startStarFinder(rarity, starsMin, starsMax, species, gmax, shinyLock);
    default:
        break;
    }

    if(!isRunning())
        start(LowPriority);
    else
    {
        restart = true;
        condition.wakeOne();
    }

}

void BotWorker::startRaidFinder()
{

}

void BotWorker::startStarFinder(int rarity, int starsMin, int starsMax, int species, bool gmax, int shinyLock)
{
    this->starsMin = starsMin;
    this->starsMax = starsMax;
    this->species = species;
    this->gmax = gmax;
    this->shinyLock = shinyLock;
    this->denType = rarity;
}

void BotWorker::generated(bool results)
{
    genned = true;
    this->results = results;
}

void BotWorker::raidFinder()
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

void BotWorker::starFinder()
{
    RaidBot raidBot = RaidBot(this, &ipRaw, &portRaw);
    forever {
        if(abort)
            break;
        raidBot.click("R");
        raidBot.pause(1700);
        if(abort)
            break;
        QByteArray denData;

        QString info = "";

        if(denID == 65535)
            info += "denID: 1";
        else if(denID - 32 > 189)
            info += "[CT] denID: " + QString::number(denID - 31);
        else if(denID - 11 > 99)
            info += "[IoA] denID: " + QString::number(denID - 10);
        else
            info += "denID: " + QString::number(denID + 1);

        if(abort)
            break;

        denData = raidBot.readDen(denID);

        int rank = denData.at(0x10);
        int randroll = denData.at(0x11);

        Den den = DenLoader::getDen((denData.at(0x13) & 2) == 2 ? 65535 : denID, denType);
        std::vector<Raid> raids = den.getRaids(raidBot.isPlayingSword ? Game::Sword : Game::Shield);
        Raid raid;
        int probability = 1;
        for(auto & r : raids)
        {
            probability += r.getStars().at(rank);
            if(probability > randroll)
            {
                raid = r;
                break;
            }
        }

        info += QString('\t') + QString::number(rank + 1) + "★" + QString('\t') + "Species: " + QString::fromStdString(Translator::getSpecie(raid.getSpecies())) + " ";
        if(raid.getGigantamax())
            info += "G-Max";

        if((denData.at(0x13) & 2) == 2)
        {
            if(raid.getShiny() == 1)
                info += "Shiny Locked ";
            else if(raid.getShiny() == 2)
                info += "◇ ";
            info += QString('\t') + "Event";
        }

        emit log(info);

        bool shinyLockCheck = true;
        if((denData.at(0x13) & 2) == 2)
        {
            shinyLockCheck = shinyLock == raid.getShiny();
        }

        if((rank + 1) >= starsMin && (rank + 1) <= starsMax && raid.getSpecies() == species && gmax == raid.getGigantamax() && shinyLockCheck)
        {
            emit log("Found.");
            if(!raidBot.foundActions())
                break;
        }

        raidBot.notFoundActions();
        emit log("Resetting...");
        if(abort)
            break;
        raidBot.quitGame();
        if(abort)
            break;
        raidBot.enterGame();
        if(abort)
            break;
        raidBot.skipIntroAnimation();

    }

    raidBot.close();
}

void BotWorker::run()
{
     switch(script)
     {
     case 0:
         raidFinder();
         break;
     case 1:
         starFinder();
         break;
     default:
         break;
     }
}
