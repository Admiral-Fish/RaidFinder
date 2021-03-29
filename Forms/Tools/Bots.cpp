#include "Bots.hpp"
#include "ui_Bots.h"
#include "QTcpSocket"
#include "QSettings"
#include "../../Core/Loader/ProfileLoader.hpp"
#include "../../Core/Results/Profile.hpp"
#include "../../Core/Bots/RaidBot.hpp"
#include "../../Core/Results/NestHoleDistributionEncounter8Archive.h"
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>
#include <iostream>
#include <fstream>

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
    QSettings setting;
    setting.setValue("settings/ip", ui->txtIP->text());
    setting.setValue("settings/port", ui->txtPort->text());
    delete ui;
}

void Bots::setupModels()
{
    connect(ui->btnStartStop, &QPushButton::clicked, this, &Bots::startScript);
    connect(ui->btnUpdateProfile, &QPushButton::clicked, this, &Bots::addProfile);

    connect(&worker, &BotWorker::generate, this, &Bots::generate);
    connect(this, &Bots::generated, &worker, &BotWorker::generated);
    connect(&worker, &BotWorker::finished, this, &Bots::botFinished);
    connect(&worker, &BotWorker::log, this, &Bots::log);

    QSettings setting;
    if (setting.contains("settings/ip"))
    {
        ui->txtIP->setText(setting.value("settings/ip").toString());
    }
    if (setting.contains("settings/port"))
    {
        ui->txtPort->setText(setting.value("settings/port").toString());
    }
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
        if(worker.isRunning())
            worker.stopScript();
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
                worker.startScript(ui->txtIP->text(), ui->txtPort->text(), 0, denID, denType);
                break;
            case 1:
                emit getDenInfo();
                emit lockBoxes(true, true, true, true, false);
                threadRunning = true;
                ui->btnStartStop->setText("Stop");
                worker.startScript(ui->txtIP->text(), ui->txtPort->text(), 1, denID, denType, starsMin, starsMax, species, gmax, shinyLock);
                break;
            case 2:
                emit getDenInfo();
                emit lockBoxes(true, true, false, false, false);
                threadRunning = true;
                ui->btnStartStop->setText("Stop");
                worker.startScript(ui->txtIP->text(), ui->txtPort->text(), 2, denID, denType);
                break;
            case 3:
                dumpLatestWildAreaEvent();
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
    if(r.isConnected())
    {
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
    else
    {
        log("Failed to connect.");
    }

    delete ip;
    delete port;
}

void Bots::dumpLatestWildAreaEvent()
{
    QString *ip = new QString(ui->txtIP->text());
    QString *port = new QString(ui->txtPort->text());
    SWSHBot r = SWSHBot(nullptr, ip, port);
    if(r.isConnected())
    {
        r.readEventBlock_RaidEncounter(QApplication::applicationDirPath() + "/");

        std::ifstream infile;
        infile.open(QApplication::applicationDirPath().toStdString() + "/normal_encount", std::ios::binary | std::ios::in);
        infile.seekg(0,std::ios::end);
        int length = infile.tellg();
        infile.seekg(0,std::ios::beg);
        std::vector<char> buffer(length);
        infile.read(buffer.data(), length);
        infile.close();

        buffer.erase(buffer.begin(), buffer.begin() + 32);
        structure::NestHoleDistributionEncounter8ArchiveT nestObj;
        structure::GetNestHoleDistributionEncounter8Archive(buffer.data())->UnPackTo(&nestObj);

        QFile fbs(":/fbs/NestHoleDistributionEncounter8Archive.fbs");
        QByteArray schemaBuffer;
        if(fbs.open(QIODevice::ReadOnly))
        {
            schemaBuffer = fbs.readAll();
            fbs.close();
        }

        std::string jsongen;
        flatbuffers::Parser parser;
        qDebug() << schemaBuffer.toStdString().c_str();
        parser.Parse(schemaBuffer.toStdString().c_str());
        parser.builder_.Finish(structure::NestHoleDistributionEncounter8Archive::Pack(parser.builder_, &nestObj));
        parser.opts.strict_json = true;
        parser.opts.output_default_scalars_in_json = true;
        if(GenerateText(parser, parser.builder_.GetBufferPointer(), &jsongen))
        {
            qDebug() << jsongen.c_str();

            size_t pos = 0;
            while ((pos = jsongen.find("ShinyFlag", pos)) != std::string::npos) {
                jsongen.replace(pos, 9, "ShinyForced");
                pos += 11;
            }

            QFile f(QApplication::applicationDirPath() + "/nests_event.json");
            if (f.open(QIODevice::WriteOnly))
            {
                f.write(jsongen.c_str());
                f.close();
                log("Start RaidFinder to see latest event data.");
            }
        }

        r.closeNoThread();
    }
    else
    {
        log("Failed to connect.");
    }

    delete ip;
    delete port;
}


