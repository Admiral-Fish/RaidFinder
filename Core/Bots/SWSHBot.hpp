#ifndef SWSHBOT_HPP
#define SWSHBOT_HPP

#include "BotCore.hpp"

class SWSHBot : public BotCore
{

    Q_ENUMS(SystemLanguage)
public:
    SWSHBot(QThread *controllingThread, QString ipRaw = NULL, QString portRaw = NULL);

    enum class SystemLanguage {
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

    uint getEventOffset(SystemLanguage language = SystemLanguage::ENUS);
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
    void foundActions();
    void notFoundActions(int i = 0, QString bot = "raid");

private:
    const uint PK8STOREDSIZE = 0x148;
    const uint PK8PARTYSIZE = 0x158;
    const int DENCOUNT = 276;

    int eventoffset = 0;

    int resets = 0;

};

#endif // SWSHBOT_HPP
