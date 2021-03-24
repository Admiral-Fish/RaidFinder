#include "Bots.hpp"
#include "ui_Bots.h"
#include "QTcpSocket"
#include "../../Core/Bots/Workers/RaidFinder.hpp"
#include "../../Core/Loader/ProfileLoader.hpp"
#include "../../Core/Results/Profile.hpp"
#include "../../Core/Bots/RaidBot.hpp"

Bots::Bots(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Bots)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();

}

Bots::~Bots()
{
    delete ui;
}

void Bots::setupModels()
{
    connect(ui->btnStartStop, &QPushButton::clicked, this, &Bots::startScript);
    connect(ui->btnUpdateProfile, &QPushButton::clicked, this, &Bots::addProfile);

    connect(&raidFinder, &RaidFinder::generate, this, &Bots::generate);
    connect(this, &Bots::generated, &raidFinder, &RaidFinder::generated);
    connect(&raidFinder, &RaidFinder::finished, this, &Bots::botFinished);
    connect(&raidFinder, &RaidFinder::log, this, &Bots::log);

    connect(&starFinder, &StarFinder::finished, this, &Bots::botFinished);
    connect(&starFinder, &StarFinder::log, this, &Bots::log);
}

void Bots::setDenInfo(int denID, int denType, int species, int starsMin, int starsMax, bool gmax, int shinyLock)
{
    this->denID = denID;
    this->denType = denType;
    this->species = species;
    this->starsMin = starsMin;
    this->starsMax = starsMax;
    this->gmax = gmax;
    this->shinyLock = shinyLock;
}

void Bots::log(QString log){
    ui->plainTextEdit->appendPlainText(log);
}

void Bots::startScript()
{
    if(threadRunning)
    {
        log("Stopping bot...");
        if(raidFinder.isRunning())
            raidFinder.stopScript();
        if(starFinder.isRunning())
            starFinder.stopScript();
    } else
    {
        ui->plainTextEdit->clear();
        switch(ui->cmbScriptSelection->currentIndex())
        {
            case 0:
                emit getDenInfo();
                emit lockBoxes(true, true, true, false, true);
                threadRunning = true;
                ui->btnStartStop->setText("Stop");
                raidFinder.startScript(ui->txtIP->text(), ui->txtPort->text(), denID, denType);
                break;
            case 1:
                emit getDenInfo();
                emit lockBoxes(true, true, true, true, false);
                threadRunning = true;
                ui->btnStartStop->setText("Stop");
                starFinder.startScript(ui->txtIP->text(), ui->txtPort->text(), denID, denType, starsMin, starsMax, species, gmax, shinyLock);
                break;
            default:
                break;
        }
    }
}

void Bots::botFinished()
{
    threadRunning = false;
    ui->btnStartStop->setText("Start");
    emit unlockBoxes();
}

void Bots::addProfile()
{
    std::vector<Profile> profiles = ProfileLoader::getProfiles();
    QString *ip = new QString(ui->txtIP->text());
    QString *port = new QString(ui->txtPort->text());
    SWSHBot r = SWSHBot(nullptr, ip, port);
    Profile p2 = Profile("Bot", r.TID, r.SID, r.isPlayingSword ? Game::Sword : Game::Shield);
    for(auto & p : profiles)
    {
        if(p.getName() == "Bot")
        {
            ProfileLoader::updateProfile(p2, p);
            emit updateProfiles();
            r.closeNoThread();
            return;
        }
    }

    ProfileLoader::addProfile(p2);
    emit updateProfiles();
    r.closeNoThread();
}


