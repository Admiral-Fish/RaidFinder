#include "RaidBot.hpp"

RaidBot::RaidBot(QThread *controllingThread, QString *ipRaw, QString *portRaw) : SWSHBot(controllingThread, ipRaw, portRaw)
{
    resets = 0;
}

void RaidBot::setTargetDen(int denId)
{
    denID = denId - 1;
}

QByteArray RaidBot::getDenData()
{
    return readDen(denID);
}

void RaidBot::getWatts(bool wattFarmer, int speed)
{
    click("A");
    pause(1500 - speed);
    click("A");
    pause(1200 - speed);
    if(wattFarmer)
        readWatts();
    click("A");
    pause(1200);
    if(!wattFarmer)
        saveGame();
    else
    {
        click("B");
        pause(200);
        click("B");
        pause(900);
    }
}

void RaidBot::setWatts(int count)
{
    watts = count;
}

void RaidBot::readWatts()
{
    QByteArray wattsBE = read("0x45068FE8", "0x3");
    std::reverse(wattsBE.begin(), wattsBE.end());
    watts = wattsBE.toHex().toInt(nullptr, 16);
}

void RaidBot::throwPiece()
{
    click("A");
    pause(500);
    click("A");
    pause(1300);
    click("A");
    pause(1400);
    click("A");
    pause(1000);
    click("HOME");
    pause(500);
}
