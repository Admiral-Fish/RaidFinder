#ifndef SWSHBOT_HPP
#define SWSHBOT_HPP

#include "BotCore.hpp"

class SWSHBot : public BotCore
{

public:
    SWSHBot(QThread *controllingThread, QString *ipRaw = nullptr, QString *portRaw = nullptr);

    int resets = 0;

    uint getEventOffset(int language = SystemLanguage::ENUS);
    QByteArray readTrainerBlock();
    QByteArray readParty(int slot = 1);
    QByteArray readBox(int box = 1, int slot = 1);
    QByteArray readTrade();
    QByteArray readWild();
    QByteArray readRaid();
    QByteArray readLegend();
    QByteArray readEventBlock_RaidEncounter(QString path = "");
    QByteArray readEventBlock_CrystalEncounter(QString path = "");
    QByteArray readEventBlock_DropRewards(QString path = "");
    QByteArray readEventBlock_BonusRewards(QString path = "");

    QByteArray readDen(int denID);
    QByteArray readScreenOff();
    QByteArray readOverworldCheck();
    QByteArray readBattleStart();
    void increaseResets();
    void quitGame(bool needHome = true);
    void enterGame();
    void skipIntroAnimation();
    void saveGame();
    void closeGame();
    bool foundActions();
    void notFoundActions();
    ushort TID;
    ushort SID;
    bool isPlayingSword;

private:
    const uint PK8STOREDSIZE = 0x148;
    const uint PK8PARTYSIZE = 0x158;
    const int DENCOUNT = 276;

    int eventoffset = 0;


};

#endif // SWSHBOT_HPP
