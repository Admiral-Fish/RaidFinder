#ifndef RAIDBOT_HPP
#define RAIDBOT_HPP

#include "SWSHBot.hpp"

class RaidBot : public SWSHBot
{
public:
    RaidBot(QThread *controllingThread, QString *ipRaw = nullptr, QString *portRaw = nullptr);
    void setTargetDen(int denId);
    QByteArray getDenData();
    void getWatts(bool wattFarmer = false, int speed = 0);
    void setWatts(int count);
    void readWatts();
    void throwPiece();
private:
    int denID;
    int watts;

};

#endif // RAIDBOT_HPP
