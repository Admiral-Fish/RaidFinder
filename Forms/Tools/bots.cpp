#include "Bots.hpp"
#include "ui_Bots.h"
#include "QTcpSocket"

Bots::Bots(QWidget *parent, Ui::MainWindow *mainui) :
    QDialog(parent),
    ui(new Ui::Bots)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    uimain = mainui;

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

void Bots::startScript()
{

}


