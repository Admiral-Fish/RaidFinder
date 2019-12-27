/*
 * This file is part of RaidFinder
 * Copyright (C) 2019 by Admiral_Fish
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
#include <Core/Loader/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/DenMap.hpp>
#include <Forms/IVCalculator.hpp>
#include <Forms/ProfileManager.hpp>
#include <Models/FrameModel.hpp>
#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("RaidFinder %1").arg(VERSION));

    DenLoader::init();
    updateProfiles();
    setupModels();
}

MainWindow::~MainWindow()
{
    QSettings setting;
    setting.setValue("mainWindow/profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("mainWindow/geometry", this->saveGeometry());
    setting.setValue("settings/locale", currentLanguage);
    setting.setValue("settings/style", currentStyle);

    delete ui;
}

void MainWindow::setupModels()
{
    model = new FrameModel(ui->tableView);
    ui->tableView->setModel(model);

    menu = new QMenu(ui->tableView);

    ui->comboBoxNature->setup(Translator::getNatures());

    ui->comboBoxAbilityType->setItemData(0, 3);
    ui->comboBoxAbilityType->setItemData(1, 4);

    ui->textBoxSeed->setValues(InputType::Seed64Bit);
    ui->textBoxInitialFrame->setValues(InputType::Frame32Bit);
    ui->textBoxMaxResults->setValues(InputType::Frame32Bit);

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

    ui->comboBoxShiny->setItemData(0, 255);
    ui->comboBoxShiny->setItemData(1, 1);
    ui->comboBoxShiny->setItemData(2, 2);
    ui->comboBoxShiny->setItemData(3, 3);

    for (u8 i = 0; i < 99; i++)
    {
        QString location = Translator::getLocation(DenLoader::getLocation(i));
        ui->comboBoxDen->addItem(QString("%1: %2").arg(i + 1).arg(location));
    }
    ui->comboBoxDen->addItem(tr("Event"));
    denIndexChanged(0);

    QSettings setting;

    languageGroup = new QActionGroup(ui->menuLanguage);
    languageGroup->setExclusive(true);
    connect(languageGroup, &QActionGroup::triggered, this, &MainWindow::slotLanguageChanged);
    currentLanguage = setting.value("settings/locale", "en").toString();
    QStringList locales = { "de", "en", "es", "fr", "it", "ja", "ko", "zh" };
    for (auto i = 0; i < locales.size(); i++)
    {
        const QString &lang = locales.at(i);

        auto *action = ui->menuLanguage->actions()[i];
        action->setData(lang);

        if (currentLanguage == lang)
        {
            action->setChecked(true);
        }

        languageGroup->addAction(action);
    }

    styleGroup = new QActionGroup(ui->menuStyle);
    styleGroup->setExclusive(true);
    connect(styleGroup, &QActionGroup::triggered, this, &MainWindow::slotStyleChanged);
    currentStyle = setting.value("settings/style", "dark").toString();
    QStringList styles = { "dark", "light" };
    for (auto i = 0; i < styles.size(); i++)
    {
        const QString &style = styles.at(i);

        auto *action = ui->menuStyle->actions()[i];
        action->setData(style);

        if (currentStyle == style)
        {
            action->setChecked(true);
        }

        styleGroup->addAction(action);
    }

    QAction *outputTXT = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSV = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXT, &QAction::triggered, this, [=]() { ui->tableView->outputModelTXT(); });
    connect(outputCSV, &QAction::triggered, this, [=]() { ui->tableView->outputModelCSV(); });

    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &MainWindow::openProfileManager);
    connect(ui->actionDen_Map, &QAction::triggered, this, &MainWindow::openDenMap);
    connect(ui->actionIV_Calculator, &QAction::triggered, this, &MainWindow::openIVCalculator);
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &MainWindow::generate);
    connect(ui->comboBoxDen, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::denIndexChanged);
    connect(
        ui->comboBoxRarity, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::rarityIndexChange);
    connect(ui->comboBoxSpecies, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::speciesIndexChanged);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &MainWindow::tableViewContextMenu);

    if (setting.contains("mainWindow/geometry"))
    {
        this->restoreGeometry(setting.value("mainWindow/geometry").toByteArray());
    }
}

void MainWindow::slotLanguageChanged(QAction *action)
{
    if (action)
    {
        QString language = action->data().toString();
        if (currentLanguage != language)
        {
            currentLanguage = language;

            QMessageBox message(QMessageBox::Question, tr("Language update"),
                                tr("Restart for changes to take effect. Restart now?"), QMessageBox::Yes | QMessageBox::No);
            if (message.exec() == QMessageBox::Yes)
            {
                QProcess::startDetached(QApplication::applicationFilePath());
                QApplication::quit();
            }
        }
    }
}

void MainWindow::slotStyleChanged(QAction *action)
{
    if (action)
    {
        QString style = action->data().toString();
        if (currentStyle != style)
        {
            currentStyle = style;

            QMessageBox message(QMessageBox::Question, tr("Style change"),
                                tr("Restart for changes to take effect. Restart now?"), QMessageBox::Yes | QMessageBox::No);
            if (message.exec() == QMessageBox::Yes)
            {
                QProcess::startDetached(QApplication::applicationFilePath());
                QApplication::quit();
            }
        }
    }
}

void MainWindow::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::profilesIndexChanged);

    profiles = ProfileLoader::getProfiles();
    profiles.insert(profiles.begin(), Profile());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
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
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());

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

void MainWindow::openIVCalculator()
{
    auto *calculator = new IVCalculator();
    calculator->show();
}

void MainWindow::denIndexChanged(int index)
{
    if (index >= 0)
    {
        int rarity = ui->comboBoxRarity->currentIndex();
        ui->comboBoxSpecies->clear();
        den = DenLoader::getDen(static_cast<u8>(index), static_cast<u8>(rarity));

        const auto species = den.getSpecies(currentProfile.getVersion());
        for (const auto &specie : species)
        {
            ui->comboBoxSpecies->addItem(QString("%1: %2").arg(Translator::getSpecie(specie.first), specie.second));
        }
    }
}

void MainWindow::rarityIndexChange(int index)
{
    (void)index;
    denIndexChanged(ui->comboBoxDen->currentIndex());
}

void MainWindow::speciesIndexChanged(int index)
{
    if (index >= 0)
    {
        Raid raid = den.getRaid(static_cast<u8>(index), currentProfile.getVersion());

        ui->spinBoxIVCount->setValue(raid.getIVCount());
        ui->comboBoxAbilityType->setCurrentIndex(ui->comboBoxAbilityType->findData(raid.getAbility()));
        ui->comboBoxGenderType->setCurrentIndex(raid.getGender());
        ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(raid.getGenderRatio()));
        ui->labelGigantamaxValue->setText(raid.getGigantamax() ? tr("Yes") : tr("No"));
    }
}

void MainWindow::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() == 0)
    {
        return;
    }

    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::generate()
{
    model->clearModel();

    u32 initialFrame = ui->textBoxInitialFrame->getUInt();
    u32 maxResults = ui->textBoxMaxResults->getUInt();
    u8 abilityType = static_cast<u8>(ui->comboBoxAbilityType->currentData().toInt());
    u8 genderType = static_cast<u8>(ui->comboBoxGenderType->currentIndex());
    u8 genderRatio = static_cast<u8>(ui->comboBoxGenderRatio->currentData().toInt());
    u8 ivCount = static_cast<u8>(ui->spinBoxIVCount->value());
    Raid raid = den.getRaid(static_cast<u8>(ui->comboBoxSpecies->currentIndex()), currentProfile.getVersion());
    RaidGenerator generator(initialFrame, maxResults, abilityType, currentProfile.getTID(), currentProfile.getSID(),
                            genderType, genderRatio, ivCount, raid.getSpecies());

    u8 gender = static_cast<u8>(ui->comboBoxGender->currentData().toInt());
    u8 ability = static_cast<u8>(ui->comboBoxAbility->currentData().toInt());
    u8 shiny = static_cast<u8>(ui->comboBoxShiny->currentData().toInt());
    bool skip = ui->checkBoxDisableFilters->isChecked();
    QVector<u8> min = ui->ivFilter->getLower();
    QVector<u8> max = ui->ivFilter->getUpper();
    QVector<bool> natures = ui->comboBoxNature->getChecked();
    FrameCompare compare(gender, ability, shiny, skip, min, max, natures);

    u64 seed = ui->textBoxSeed->getULong();
    QVector<Frame> frames = generator.generate(compare, seed);

    model->addItems(frames);
}
