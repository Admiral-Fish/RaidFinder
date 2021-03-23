#include "SWSHBot.hpp"
#include <QMessageBox>

SWSHBot::SWSHBot(QThread *controllingThread, QString *ipRaw, QString *portRaw) : BotCore(controllingThread, ipRaw, portRaw)
{
    QByteArray trainerSave = readTrainerBlock();
    eventoffset = 0;
    resets = 0;
    if(trainerSave.at(0xA4) == 44 || trainerSave.at(0xA4) == 45)
    {
        isPlayingSword = (trainerSave.at(0xA4) == 44);
        getEventOffset(getSystemLanguage());
        TID = trainerSave.mid(0xA0, 2).toUShort();
        SID = trainerSave.mid(0xA2, 2).toUShort();
    }
}

uint SWSHBot::getEventOffset(int language)
{
    if(language == SystemLanguage::ZHCN || language == SystemLanguage::ZHHANS)
    {
        eventoffset = -0xE00;
    } else if(language == SystemLanguage::ZHTW || language == SystemLanguage::ZHHANT)
    {
        eventoffset = -0xE60;
    } else if(language == SystemLanguage::KO)
    {
        eventoffset = -0xA00;
    } else if(language == SystemLanguage::IT)
    {
        eventoffset = -0x80;
    } else if(language == SystemLanguage::JA)
    {
        eventoffset = +0x160;
    } else if(language == SystemLanguage::FR || language == SystemLanguage::FRCA)
    {
        eventoffset = +0x1F0;
    } else if(language == SystemLanguage::ES || language == SystemLanguage::ES419)
    {
        eventoffset = +0x1C0;
    } else if(language == SystemLanguage::DE)
    {
        eventoffset = +0x2D0;
    } else {
        eventoffset = 0;
    }
    return eventoffset;
}

QByteArray SWSHBot::readTrainerBlock() {
    return read("0x45068F18", "0x110") + read("0x45072DF4", "0x3");
}

QByteArray SWSHBot::readParty(int slot)
{
    if(slot > 6)
        slot = 6;
    QString address = "0x" + QString::number(0x450C68B0 + (slot - 1) * PK8PARTYSIZE, 16);
    return read(address, "0x" + QString::number(PK8PARTYSIZE, 16));
}

QByteArray SWSHBot::readBox(int box, int slot)
{
    if(box > 31)
        box = 31;
    if(slot > 29)
        slot = 29;
    QString address = "0x" + QString::number(0x45075880 + ((box - 1) * 30 * PK8PARTYSIZE) + ((slot - 1) * PK8PARTYSIZE), 16);
    return read(address, "0x" + QString::number(PK8PARTYSIZE, 16));
}

QByteArray SWSHBot::readTrade()
{
    return read("0xAF286078", "0x" + QString::number(PK8STOREDSIZE, 16));
}

QByteArray SWSHBot::readWild()
{
    return read("0x8FEA3648", "0x" + QString::number(PK8STOREDSIZE, 16));
}

QByteArray SWSHBot::readRaid()
{
    return read("0x886C1EC8", "0x" + QString::number(PK8STOREDSIZE, 16));
}

QByteArray SWSHBot::readLegend()
{
    return read("0x886BC348", "0x" + QString::number(PK8STOREDSIZE, 16));
}

QByteArray SWSHBot::readEventBlock_RaidEncounter(QString path)
{
    return read("0x" + QString::number(0x2F9EB300 + eventoffset, 16), "0x23D4", path + "normal_encount");
}

QByteArray SWSHBot::readEventBlock_CrystalEncounter(QString path)
{
    return read("0x" + QString::number(0x2F9ED788 + eventoffset, 16), "0x1241C", path + "dai_encount");
}

QByteArray SWSHBot::readEventBlock_DropRewards(QString path)
{
    return read("0x" + QString::number(0x2F9FFC58 + eventoffset, 16), "0x426C", path + "drop_rewards");
}

QByteArray SWSHBot::readEventBlock_BonusRewards(QString path)
{
    return read("0x" + QString::number(0x2FA03F78 + eventoffset, 16), "0x116C4", path + "bonus_rewards");
}

QByteArray SWSHBot::readDen(int denID)
{
    int denDataSize = 0x18;
    if(denID > DENCOUNT + 31)
        denID = DENCOUNT + 31;
    QString address = "0x" + QString::number(0x450C8A70 + denID * denDataSize, 16);
    return read(address, "0x" + QString::number(denDataSize, 16));
}

QByteArray SWSHBot::readScreenOff()
{
    return read("0x6B30FA00", "0x8");
}

QByteArray SWSHBot::readOverworldCheck()
{
    return read("0x" + QString::number(0x2F770638 + eventoffset, 16), "0x4");
}

QByteArray SWSHBot::readBattleStart()
{
    return read("0x6B578EDC", "0x8");
}

void SWSHBot::increaseResets()
{
    resets += 1;
}

void SWSHBot::quitGame(bool needHome)
{
    if(needHome)
    {
        click("HOME");
        pause(800);
    }
    click("X");
    pause(200);
    click("X");
    pause(400);
    click("A");
    pause(200);
    click("A");
    pause(3000);
}

void SWSHBot::enterGame()
{
    click("A");
    pause(200);
    click("A");
    pause(1300);
    click("A");
    pause(200);
    click("A");
}

void SWSHBot::skipIntroAnimation()
{
    bool skip = false;
    pause(14700);
    while(!skip)
    {
        pause(300);
        QByteArray currScreen = readScreenOff();
        currScreen.truncate(8);
        if(QString(currScreen.toHex()).toULongLong(nullptr, 16) >= 0xFFFF)
        {
            skip = true;
        }
        else
            click("A");
    }
    for(int i = 0; i < 10; i++)
    {
        click("A");
        pause(500);
    }
    bool skipped = false;
    while(!skipped)
    {
        QByteArray currScreen = readScreenOff();
        if(currScreen.at(0))
        {
            skipped = true;
        }
        pause(500);
    }
}

void SWSHBot::saveGame()
{
    click("X");
    pause(1200);
    click("R");
    pause(1500);
    click("A");
    pause(4000);
}

void SWSHBot::closeGame()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Close", "Close the game?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        reply = QMessageBox::question(nullptr, "Home", "Need HOME button pressing?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        bool needHome = (reply == QMessageBox::Yes);
        quitGame(needHome);
    }
    close();
}

bool SWSHBot::foundActions()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Continue", "Found after " + QString::number(resets) + " resets.\n" + "Continue searching?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if(reply == QMessageBox::No)
        closeGame();
    else
    {
        increaseResets();
    }
    return reply == QMessageBox::Yes;
}

void SWSHBot::notFoundActions()
{
    increaseResets();
}
