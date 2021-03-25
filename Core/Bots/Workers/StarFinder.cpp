#include "StarFinder.hpp"
#include "../RaidBot.hpp"
#include "../../../Core/Loader/DenLoader.hpp"
#include "../../Results/Den.hpp"
#include "../../Util/Translator.hpp"

StarFinder::StarFinder(QObject *parent) : QThread(parent)
{

}

StarFinder::~StarFinder()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void StarFinder::stopScript()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
    emit log("Bot stopped.");
}

void StarFinder::startScript(QString ipRaw, QString portRaw, int denID, int rarity, int starsMin, int starsMax, int species, bool gmax, int shinyLock)
{
    QMutexLocker locker(&mutex);

    this->ipRaw = ipRaw;
    this->portRaw = portRaw;
    if(denID == 65535)
    {
        this->denID = 65535;
        this->rarity = 2;
    } else {
        this->rarity = rarity;
        if(denID > 189)
            this->denID = denID + 32;
        else if(denID > 99)
            this->denID = denID + 11;
        else
            this->denID = denID;
    }

    this->starsMin = starsMin;
    this->starsMax = starsMax;
    this->species = species;
    this->gmax = gmax;
    this->shinyLock = shinyLock;
    this->rarity = rarity;

    if(!isRunning())
        start(LowPriority);
    else
    {
        restart = true;
        condition.wakeOne();
    }
}

void StarFinder::run()
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

        Den den = DenLoader::getDen((denData.at(0x13) & 2) == 2 ? 65535 : denID, rarity);
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
