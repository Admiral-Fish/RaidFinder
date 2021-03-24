/*
 * This file is part of RaidFinder
 * Copyright (C) 2019-2020 by Admiral_Fish
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <Core/Generator/RaidGenerator.hpp>
#include <Core/Loader/DenLoader.hpp>
#include <Core/Loader/PersonalLoader.hpp>
#include <Core/Loader/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Profile/ProfileManager.hpp>
#include <Forms/Tools/DenMap.hpp>
#include <Forms/Tools/EncounterLookup.hpp>
#include <Forms/Tools/IVCalculator.hpp>
#include <Forms/Tools/Settings.hpp>
#include <Forms/Tools/Bots.hpp>
#include <Models/StateModel.hpp>
#include <QApplication>
#include <QDesktopServices>
#include <QEventLoop>
#include <QFile>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QSettings>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("RaidFinder %1").arg(VERSION));

    PersonalLoader::init();
    DenLoader::init(QApplication::applicationDirPath().toStdString());

    updateProfiles();
    setupModels();
    QTimer::singleShot(1000, this, &MainWindow::checkUpdates);
}

MainWindow::~MainWindow()
{
    QSettings setting;
    setting.setValue("mainWindow/profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("mainWindow/geometry", this->saveGeometry());
    setting.setValue("settings/seed", ui->textBoxSeed->text());

    delete ui;
}

void MainWindow::setupModels()
{
    model = new StateModel(ui->tableView);
    ui->tableView->setModel(model);

    menu = new QMenu(ui->tableView);

    ui->comboBoxNature->setup(Translator::getNatures());

    ui->comboBoxAbilityType->setItemData(0, 0);
    ui->comboBoxAbilityType->setItemData(1, 1);
    ui->comboBoxAbilityType->setItemData(2, 2);
    ui->comboBoxAbilityType->setItemData(3, 3);
    ui->comboBoxAbilityType->setItemData(4, 4);

    ui->textBoxSeed->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advances32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advances32Bit);

    ui->comboBoxAbility->setItemData(0, 255); // Any ability
    ui->comboBoxAbility->setItemData(1, 0); // Ability 0
    ui->comboBoxAbility->setItemData(2, 1); // Ability 1
    ui->comboBoxAbility->setItemData(3, 2); // Ability 2 (H)

    ui->comboBoxGender->setItemData(0, 255); // Any gender
    ui->comboBoxGender->setItemData(1, 0); // Male
    ui->comboBoxGender->setItemData(2, 1); // Female

    ui->comboBoxGenderRatio->setItemData(0, 255);
    ui->comboBoxGenderRatio->setItemData(1, 0);
    ui->comboBoxGenderRatio->setItemData(2, 254);
    ui->comboBoxGenderRatio->setItemData(3, 31);
    ui->comboBoxGenderRatio->setItemData(4, 63);
    ui->comboBoxGenderRatio->setItemData(5, 127);
    ui->comboBoxGenderRatio->setItemData(6, 191);

    ui->comboBoxShiny->setItemData(0, 255); // Any shiny type
    ui->comboBoxShiny->setItemData(1, 1); // Star
    ui->comboBoxShiny->setItemData(2, 2); // Square
    ui->comboBoxShiny->setItemData(3, 3); // Star or square

    ui->comboBoxShinyType->setItemData(0, 0); // Random shiny chance
    ui->comboBoxShinyType->setItemData(1, 1); // Forced non-shiny
    ui->comboBoxShinyType->setItemData(2, 2); // Forced shiny

    locationIndexChanged(0);
    denIndexChanged(0);
    speciesIndexChanged(0);

    QAction *outputTXT = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSV = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXT, &QAction::triggered, this, [=] { ui->tableView->outputModel(false); });
    connect(outputCSV, &QAction::triggered, this, [=] { ui->tableView->outputModel(true); });

    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &MainWindow::openProfileManager);
    connect(ui->actionDenMap, &QAction::triggered, this, &MainWindow::openDenMap);
    connect(ui->actionEncounterLookup, &QAction::triggered, this, &MainWindow::openEncounterLookup);
    connect(ui->actionIVCalculator, &QAction::triggered, this, &MainWindow::openIVCalculator);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::openSettings);
    connect(ui->actionBots, &QAction::triggered, this, &MainWindow::openBots);
    connect(ui->actionDownloadEventData, &QAction::triggered, this, &MainWindow::downloadEventData);
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &MainWindow::generate);
    connect(ui->comboBoxLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::locationIndexChanged);
    connect(ui->comboBoxDen, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::denIndexChanged);
    connect(ui->comboBoxRarity, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::rarityIndexChange);
    connect(ui->comboBoxSpecies, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::speciesIndexChanged);
    connect(ui->checkBoxShowStats, &QCheckBox::toggled, this, &MainWindow::showStatsToggled);
    connect(ui->spinBoxLevel, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::levelValueChanged);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &MainWindow::tableViewContextMenu);

    QSettings setting;
    if (setting.contains("settings/seed"))
    {
        ui->textBoxSeed->setText(setting.value("settings/seed").toString());
    }
    if (setting.contains("mainWindow/geometry"))
    {
        this->restoreGeometry(setting.value("mainWindow/geometry").toByteArray());
    }
}

QByteArray MainWindow::downloadFile(const QString &url)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QScopedPointer<QNetworkReply> reply(manager.get(request));

    QEventLoop loop;
    connect(reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    return reply->readAll();
}

void MainWindow::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::profilesIndexChanged);

    profiles = ProfileLoader::getProfiles();
    profiles.insert(profiles.begin(), Profile());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("mainWindow/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void MainWindow::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));

        denIndexChanged(ui->comboBoxDen->currentIndex());
    }
}

void MainWindow::openProfileManager()
{
    auto *manager = new ProfileManager();
    connect(manager, &ProfileManager::updateProfiles, this, &MainWindow::updateProfiles);
    manager->show();
}

void MainWindow::openDenMap()
{
    auto *map = new DenMap();
    map->show();
}

void MainWindow::openEncounterLookup()
{
    auto *lookup = new EncounterLookup(currentProfile.getVersion());
    lookup->show();
}

void MainWindow::openIVCalculator()
{
    auto *calculator = new IVCalculator();
    calculator->show();
}

void MainWindow::openSettings()
{
    auto *settings = new Settings();
    settings->show();
}

void MainWindow::openBots()
{
    auto *bots = new Bots(this);
    connect(bots, &Bots::generate, this, &MainWindow::generate2);
    connect(this, &MainWindow::generated, bots, &Bots::generated);
    connect(this, &MainWindow::denInfo, bots, &Bots::setDenInfo);
    connect(bots, &Bots::getDenInfo, this, &MainWindow::sendDenInfo);
    connect(bots, &Bots::updateProfiles, this, &MainWindow::updateProfiles);
    connect(bots, &Bots::lockBoxes, this, &MainWindow::lockBoxes);
    connect(bots, &Bots::unlockBoxes, this, &MainWindow::unlockBoxes);
    bots->show();
}

void MainWindow::downloadEventData()
{
    auto fileResponse
        = downloadFile("https://raw.githubusercontent.com/Admiral-Fish/RaidFinder/master/Resources/Encounters/Event/files.txt");
    if (fileResponse.isEmpty())
    {
        QMessageBox error(QMessageBox::Critical, tr("Download failed"),
                          tr("Make sure you are connected to the internet and have OpenSSL setup"), QMessageBox::Ok);
        error.exec();
        return;
    }

    QStringList infos = QString(fileResponse).split('\n');
    QStringList files;
    QStringList entries;
    for (const QString &info : infos)
    {
        QStringList data = info.split(',');
        QString file = data.at(0);
        u16 specie = data.at(1).toUShort();

        files.prepend(file);

        file = file.left(file.indexOf('_'));
        file.insert(2, '-');
        file.insert(5, '-');

        entries.prepend(QString("%1: %2").arg(file, QString::fromStdString(Translator::getSpecie(specie))));
    }

    bool flag;
    QString item = QInputDialog::getItem(this, tr("Download Event Data"), tr("Event"), entries, 0, false, &flag, Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    if (!flag)
    {
        return;
    }

    int index = entries.indexOf(item);
    auto eventResponse
        = downloadFile("https://raw.githubusercontent.com/Admiral-Fish/RaidFinder/master/Resources/Encounters/Event/" + files.at(index));
    if (eventResponse.isEmpty())
    {
        QMessageBox error(QMessageBox::Critical, tr("Download failed"),
                          tr("Make sure you are connected to the internet and have OpenSSL setup"), QMessageBox::Ok);
        error.exec();
        return;
    }

    QFile f(QApplication::applicationDirPath() + "/nests_event.json");
    if (f.open(QIODevice::WriteOnly))
    {
        f.write(qUncompress(eventResponse));
        f.close();

        QMessageBox message(QMessageBox::Question, tr("Download finished"), tr("Restart to see event data. Restart now?"),
                            QMessageBox::Yes | QMessageBox::No);
        if (message.exec() == QMessageBox::Yes)
        {
            QProcess::startDetached(QApplication::applicationFilePath());
            QApplication::quit();
        }
    }
}

void MainWindow::checkUpdates()
{
    QSettings setting;
    QDate today = QDate::currentDate();
    QDate lastOpened = setting.value("settings/lastOpened", today).toDate();

    if (lastOpened.daysTo(today) > 0)
    {
        auto response = downloadFile("https://api.github.com/repos/Admiral-Fish/RaidFinder/releases/latest");
        auto json = QJsonDocument::fromJson(response).object();

        QString webVersion = json["tag_name"].toString().right(5);
        if (!webVersion.isEmpty() && VERSION != webVersion)
        {
            QMessageBox info(QMessageBox::Question, tr("Update Check"),
                             tr("An update is available. Would you like to download the newest version?"),
                             QMessageBox::Yes | QMessageBox::No);
            if (info.exec() == QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("https://github.com/Admiral-Fish/RaidFinder/releases/latest"));
            }
        }
    }

    setting.setValue("settings/lastOpened", today);
}

void MainWindow::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->comboBoxDen->clear();

        if (QFile::exists(QApplication::applicationDirPath() + "/nests_event.json"))
        {
            ui->comboBoxDen->addItem(tr("Event"), 65535);
        }

        u16 start, end, offset;
        if (index == 0)
        {
            start = 0;
            end = 100;
            offset = 0;
        }
        else if (index == 1)
        {
            start = 100;
            end = 190;
            offset = 100;
        }
        else
        {
            start = 190;
            end = 276;
            offset = 190;
        }

        for (u16 denID = start; denID < end; denID++)
        {
            if (denID == 16)
            {
                continue;
            }

            QString location = QString::fromStdString(Translator::getLocation(DenLoader::getLocation(denID)));
            ui->comboBoxDen->addItem(QString("%1: %2").arg(denID + 1 - offset).arg(location), denID);
        }
    }
}

void MainWindow::denIndexChanged(int index)
{
    if (index >= 0)
    {
        u16 denID = ui->comboBoxDen->currentData().toInt();
        int rarity = ui->comboBoxRarity->currentIndex();
        den = DenLoader::getDen(denID, rarity);

        ui->comboBoxSpecies->clear();
        auto raids = den.getRaids(currentProfile.getVersion());
        for (const auto &raid : raids)
        {
            ui->comboBoxSpecies->addItem(QString("%1: %2").arg(QString::fromStdString(Translator::getSpecie(raid.getSpecies())),
                                                               QString::fromStdString(raid.getStarDisplay())), QString::number(raid.getSpecies()) + "," + QString::number(raid.getMinStars()) + "," + QString::number(raid.getMaxStars()));
        }
    }
}

void MainWindow::rarityIndexChange(int index)
{
    if (index >= 0)
    {
        denIndexChanged(ui->comboBoxDen->currentIndex());
    }
}

void MainWindow::speciesIndexChanged(int index)
{
    if (index >= 0)
    {
        Raid raid = den.getRaid(static_cast<u8>(index), currentProfile.getVersion());
        PersonalInfo info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

        ui->spinBoxIVCount->setValue(raid.getIVCount());
        ui->comboBoxAbilityType->setCurrentIndex(ui->comboBoxAbilityType->findData(raid.getAbility()));
        ui->comboBoxGenderType->setCurrentIndex(raid.getGender());
        ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(raid.getGenderRatio()));
        ui->comboBoxShinyType->setCurrentIndex(ui->comboBoxShinyType->findData(raid.getShiny()));
        ui->labelGigantamaxValue->setText(raid.getGigantamax() ? tr("Yes") : tr("No"));

        int abilityIndex = ui->comboBoxAbility->currentIndex();

        ui->comboBoxAbility->setItemText(1, "1: " + QString::fromStdString(Translator::getAbility(info.getAbility1())));
        ui->comboBoxAbility->setItemText(2, "2: " + QString::fromStdString(Translator::getAbility(info.getAbility2())));

        ui->comboBoxAbility->removeItem(3);
        if (raid.getAbility() == 2 || raid.getAbility() == 4)
        {
            ui->comboBoxAbility->addItem("H: " + QString::fromStdString(Translator::getAbility(info.getAbilityH())), 2);
        }

        if (abilityIndex < ui->comboBoxAbility->count())
        {
            ui->comboBoxAbility->setCurrentIndex(abilityIndex);
        }
        else
        {
            ui->comboBoxAbility->setCurrentIndex(0);
        }
    }
}

void MainWindow::showStatsToggled(bool flag)
{
    ui->spinBoxLevel->setEnabled(flag);
    model->setShowStats(flag);
}

void MainWindow::levelValueChanged(int value)
{
    model->setLevel(value);
}

void MainWindow::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::sendDenInfo(){
    emit denInfo(ui->comboBoxDen->currentData().toInt(), ui->comboBoxRarity->currentIndex(), ui->comboBoxSpecies->currentData().toString().split(",").at(0).toInt(), ui->comboBoxSpecies->currentData().toString().split(",").at(1).toInt(), ui->comboBoxSpecies->currentData().toString().split(",").at(2).toInt(), ui->labelGigantamaxValue->text() == "Yes" ? true : false, ui->comboBoxShinyType->currentData().toInt());
}

void MainWindow::lockBoxes(bool location, bool den, bool rarity, bool species, bool seed)
{
    if(location)
        ui->comboBoxLocation->setEnabled(false);
    if(den)
        ui->comboBoxDen->setEnabled(false);
    if(rarity)
        ui->comboBoxRarity->setEnabled(false);
    if(species)
        ui->comboBoxSpecies->setEnabled(false);
    if(seed)
        ui->textBoxSeed->setEnabled(false);
}

void MainWindow::unlockBoxes()
{
    ui->comboBoxLocation->setEnabled(true);
    ui->comboBoxDen->setEnabled(true);
    ui->comboBoxRarity->setEnabled(true);
    ui->comboBoxSpecies->setEnabled(true);
    ui->textBoxSeed->setEnabled(true);
}

void MainWindow::generate2(QString seed)
{
    ui->textBoxSeed->setText(seed);
    generate();
}

void MainWindow::generate()
{
    Raid raid = den.getRaid(static_cast<u8>(ui->comboBoxSpecies->currentIndex()), currentProfile.getVersion());
    model->clearModel();
    model->setInfo(PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm()));

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();

    RaidGenerator generator(initialAdvances, maxAdvances, tid, sid, raid);

    u8 gender = static_cast<u8>(ui->comboBoxGender->currentData().toInt());
    u8 ability = static_cast<u8>(ui->comboBoxAbility->currentData().toInt());
    u8 shiny = static_cast<u8>(ui->comboBoxShiny->currentData().toInt());
    bool skip = ui->checkBoxDisableFilters->isChecked();
    std::array<u8, 6> min = ui->ivFilter->getLower();
    std::array<u8, 6> max = ui->ivFilter->getUpper();
    std::vector<bool> natures = ui->comboBoxNature->getChecked();
    StateFilter filter(gender, ability, shiny, skip, min, max, natures);

    u64 seed = ui->textBoxSeed->getULong();

    std::vector<State> states = generator.generate(filter, seed);
    model->addItems(states);
    emit generated(!states.empty());
}
