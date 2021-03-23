#include "Bots.hpp"
#include "ui_Bots.h"
#include "QTcpSocket"
#include "../../Core/Bots/Workers/RaidFinder.hpp"

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
}

void Bots::setDenInfo(int denID, int location, int denType)
{
    this->denID = denID;
    this->location = location;
    this->denType = denType;
}

void Bots::startScript()
{
    switch(ui->cmbScriptSelection->currentIndex())
    {
        case 0:
            connect(&thread, &RaidFinder::generate, this, &Bots::generate);
            connect(this, &Bots::generated, &thread, &RaidFinder::generated);
            emit getDenInfo();
            thread.startScript(ui->txtIP->text(), ui->txtPort->text(), denID, location, denType);
            break;
        default:
            break;
    }
}


