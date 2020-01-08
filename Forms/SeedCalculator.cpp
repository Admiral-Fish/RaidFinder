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

#include "SeedCalculator.hpp"
#include "ui_SeedCalculator.h"
#include <Core/Game.hpp>
#include <Core/Loader/DenLoader.hpp>
#include <Core/Loader/PersonalLoader.hpp>
#include <Core/Results/Pokemon.hpp>
#include <Core/Searcher/SeedSearcher.hpp>
#include <Core/Util/Translator.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>

SeedCalculator::SeedCalculator(QWidget *parent) : QWidget(parent), ui(new Ui::SeedCalculator)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

SeedCalculator::~SeedCalculator()
{
    delete ui;
}

void SeedCalculator::setupModels()
{
    for (u8 i = 0; i < 100; i++)
    {
        if (i == 16)
        {
            continue;
        }

        QString location = Translator::getLocation(DenLoader::getLocation(i));
        ui->comboBoxDen->addItem(QString("%1: %2").arg(i + 1).arg(location), i);
    }
    ui->comboBoxDen->addItem(tr("Event"), 100);

    denIndexChanged(0);
    raidDay4_1IndexChanged(0);
    raidDay4_2IndexChanged(0);
    raidDay5IndexChanged(0);

    ui->comboBoxGame->setItemData(0, Game::Sword);
    ui->comboBoxGame->setItemData(1, Game::Shield);

    ui->comboBoxNatureDay4_1->addItems(Translator::getNatures());
    ui->comboBoxNatureDay4_2->addItems(Translator::getNatures());
    ui->comboBoxNatureDay5->addItems(Translator::getNatures());

    ui->comboBoxCharacteristicDay4_1->addItems(Translator::getCharacteristics());
    ui->comboBoxCharacteristicDay4_2->addItems(Translator::getCharacteristics());
    ui->comboBoxCharacteristicDay5->addItems(Translator::getCharacteristics());

    connect(ui->comboBoxDen, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::denIndexChanged);
    connect(ui->comboBoxRarity, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::rarityIndexChanged);
    connect(ui->comboBoxGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::gameIndexChanged);
    connect(ui->comboBoxRaidDay4_1, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::raidDay4_1IndexChanged);
    connect(ui->comboBoxRaidDay4_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::raidDay4_2IndexChanged);
    connect(ui->comboBoxRaidDay5, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::raidDay5IndexChanged);
    connect(ui->spinBoxHPDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxAtkDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxDefDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxSpADay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxSpDDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxSpeDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &SeedCalculator::search);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &SeedCalculator::clear);
}

QVector<u8> SeedCalculator::getIVs(const QVector<u8> &ivs1, const QVector<u8> &ivs2)
{
    QVector<u8> ivs = ivs1 + ivs2;
    ivs.removeAll(31);
    return ivs;
}

void SeedCalculator::checkDay4()
{
    QVector<u8> ivs = { static_cast<u8>(ui->spinBoxHPDay4_1->value()),  static_cast<u8>(ui->spinBoxAtkDay4_1->value()),
                        static_cast<u8>(ui->spinBoxDefDay4_1->value()), static_cast<u8>(ui->spinBoxSpADay4_1->value()),
                        static_cast<u8>(ui->spinBoxSpDDay4_1->value()), static_cast<u8>(ui->spinBoxSpeDay4_1->value()) };

    int ivCount = ivs.count(31);
    int limit = ui->spinBoxIVCountDay4_1->value();

    if (ivCount != limit)
    {
        ui->labelCheck->setText(tr("Invalid"));
        return;
    }

    QVector<bool> ivFlag;
    for (auto i = 0; i < 6; i++)
    {
        ivFlag.append(ivs.at(i) == 31);
    }

    int needNumber = ui->spinBoxIVCountDay4_1->value() < 3 ? 6 : 5;
    QVector<bool> possible(6, false);
    for (int count = ivCount + 1; count < 5; count++)
    {
        int c = ivCount;
        int used = ivCount;
        QVector<bool> ivFlagCopy(ivFlag);

        for (auto i = 0; i < 6; i++)
        {
            if (ivs.at(i) != 31)
            {
                used++;
                u8 position = ivs.at(i) % 8;

                if (position < 6 && !ivFlagCopy.at(position))
                {
                    ivFlagCopy[position] = true;
                    if (++c == count)
                    {
                        if ((6 - count) - (6 - used) + (6 - ivCount) >= needNumber)
                        {
                            possible[count] = true;
                        }
                        break;
                    }
                }
            }
        }
    }

    if (possible.at(3) && possible.at(4))
    {
        ui->labelCheck->setText(tr("Day 4 (2nd): 3IV/4IV"));
    }
    else if (possible.at(3))
    {
        ui->labelCheck->setText(tr("Day 4 (2nd): 3IV"));
    }
    else if (possible.at(4))
    {
        ui->labelCheck->setText(tr("Day 4 (2nd): 4IV"));
    }
    else
    {
        ui->labelCheck->setText(tr("Invalid"));
    }
}

void SeedCalculator::denIndexChanged(int index)
{
    if (index >= 0)
    {
        u8 rarity = static_cast<u8>(ui->comboBoxRarity->currentIndex());
        den = DenLoader::getDen(static_cast<u8>(ui->comboBoxDen->currentData().toInt()), rarity);

        ui->comboBoxRaidDay4_1->clear();
        ui->comboBoxRaidDay4_2->clear();
        ui->comboBoxRaidDay5->clear();

        auto game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());
        const auto species = den.getSpecies(game);
        for (u8 i = 0; i < 12; i++)
        {
            auto raid = den.getRaid(i, game);
            if ((raid.getIVCount() == 2 || raid.getIVCount() == 3) && (raid.getStar(2) || raid.getStar(3) || raid.getStar(4)))
            {
                ui->comboBoxRaidDay4_1->addItem(QString("%1: %2").arg(Translator::getSpecie(species[i].first), species[i].second), i);
            }

            if ((raid.getIVCount() == 3 || raid.getIVCount() == 4) && (raid.getStar(2) || raid.getStar(3) || raid.getStar(4)))
            {
                ui->comboBoxRaidDay4_2->addItem(QString("%1: %2").arg(Translator::getSpecie(species[i].first), species[i].second), i);
            }

            if (raid.getIVCount() >= 2 && (raid.getStar(2) || raid.getStar(3) || raid.getStar(4)))
            {
                ui->comboBoxRaidDay5->addItem(QString("%1: %2").arg(Translator::getSpecie(species[i].first), species[i].second), i);
            }
        }
    }
}

void SeedCalculator::rarityIndexChanged(int index)
{
    if (index >= 0)
    {
        denIndexChanged(ui->comboBoxDen->currentIndex());
    }
}

void SeedCalculator::gameIndexChanged(int index)
{
    if (index >= 0)
    {
        denIndexChanged(ui->comboBoxDen->currentIndex());
    }
}

void SeedCalculator::raidDay4_1IndexChanged(int index)
{
    if (index >= 0)
    {
        auto game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());
        auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay4_1->currentData().toInt()), game);
        auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

        ui->spinBoxIVCountDay4_1->setValue(raid.getIVCount());
        checkDay4();

        ui->comboBoxAbilityDay4_1->clear();
        ui->comboBoxAbilityDay4_1->addItem(Translator::getAbility(info.getAbility1()), info.getAbility1());
        if (info.getAbility1() != info.getAbility2())
        {

            ui->comboBoxAbilityDay4_1->addItem(Translator::getAbility(info.getAbility2()), info.getAbility2());
        }
        if (raid.getAbility() == 4) // Allow hidden ability
        {
            ui->comboBoxAbilityDay4_1->addItem(Translator::getAbility(info.getAbilityH()), info.getAbilityH());
        }
    }
}

void SeedCalculator::raidDay4_2IndexChanged(int index)
{
    if (index >= 0)
    {
        auto game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());
        auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay4_2->currentData().toInt()), game);
        auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

        ui->spinBoxIVCountDay4_2->setValue(raid.getIVCount());

        ui->comboBoxAbilityDay4_2->clear();
        ui->comboBoxAbilityDay4_2->addItem(Translator::getAbility(info.getAbility1()), info.getAbility1());
        if (info.getAbility1() != info.getAbility2())
        {
            ui->comboBoxAbilityDay4_2->addItem(Translator::getAbility(info.getAbility2()), info.getAbility2());
        }
        if (raid.getAbility() == 4) // Allow hidden ability
        {
            ui->comboBoxAbilityDay4_2->addItem(Translator::getAbility(info.getAbilityH()), info.getAbilityH());
        }
    }
}

void SeedCalculator::raidDay5IndexChanged(int index)
{
    if (index >= 0)
    {
        auto game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());
        auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay5->currentData().toInt()), game);
        auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

        ui->spinBoxIVCountDay5->setValue(raid.getIVCount());

        ui->comboBoxAbilityDay5->clear();
        ui->comboBoxAbilityDay5->addItem(Translator::getAbility(info.getAbility1()), info.getAbility1());
        if (info.getAbility1() != info.getAbility2())
        {

            ui->comboBoxAbilityDay5->addItem(Translator::getAbility(info.getAbility2()), info.getAbility2());
        }
        if (raid.getAbility() == 4) // Allow hidden ability
        {
            ui->comboBoxAbilityDay5->addItem(Translator::getAbility(info.getAbilityH()), info.getAbilityH());
        }
    }
}

void SeedCalculator::search()
{
    QVector<u8> ivs1 = { static_cast<u8>(ui->spinBoxHPDay4_1->value()),  static_cast<u8>(ui->spinBoxAtkDay4_1->value()),
                         static_cast<u8>(ui->spinBoxDefDay4_1->value()), static_cast<u8>(ui->spinBoxSpADay4_1->value()),
                         static_cast<u8>(ui->spinBoxSpDDay4_1->value()), static_cast<u8>(ui->spinBoxSpeDay4_1->value()) };

    QVector<u8> ivs2 = { static_cast<u8>(ui->spinBoxHPDay4_2->value()),  static_cast<u8>(ui->spinBoxAtkDay4_2->value()),
                         static_cast<u8>(ui->spinBoxDefDay4_2->value()), static_cast<u8>(ui->spinBoxSpADay4_2->value()),
                         static_cast<u8>(ui->spinBoxSpDDay4_2->value()), static_cast<u8>(ui->spinBoxSpeDay4_2->value()) };

    QVector<u8> ivs3 = { static_cast<u8>(ui->spinBoxHPDay5->value()),  static_cast<u8>(ui->spinBoxAtkDay5->value()),
                         static_cast<u8>(ui->spinBoxDefDay5->value()), static_cast<u8>(ui->spinBoxSpADay5->value()),
                         static_cast<u8>(ui->spinBoxSpDDay5->value()), static_cast<u8>(ui->spinBoxSpeDay5->value()) };

    if (ivs1.count(31) != ui->spinBoxIVCountDay4_1->value())
    {
        QMessageBox message(QMessageBox::Warning, tr("Invalid IVs"), tr("IVs do not match IV count"));
        message.show();
        message.exec();
        return;
    }

    int fixedIVs = ui->spinBoxIVCountDay4_1->value();
    int ivCount[2] = { ui->spinBoxIVCountDay4_2->value(), ui->spinBoxIVCountDay5->value() };
    auto game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());

    auto raid1 = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay4_1->currentData().toInt()), game);
    auto info1 = PersonalLoader::getInfo(raid1.getSpecies(), raid1.getAltForm());
    auto raid2 = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay4_2->currentData().toInt()), game);
    auto info2 = PersonalLoader::getInfo(raid2.getSpecies(), raid2.getAltForm());
    auto raid3 = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay5->currentData().toInt()), game);
    auto info3 = PersonalLoader::getInfo(raid3.getSpecies(), raid3.getAltForm());

    u16 species1 = raid1.getSpecies();
    u8 ability1 = static_cast<u8>(ui->comboBoxAbilityDay4_1->currentIndex());
    if (ability1 != 2 && (info1.getAbility1() == info1.getAbility2()))
    {
        ability1 = 255;
    }
    u8 nature1 = static_cast<u8>(ui->comboBoxNatureDay4_1->currentIndex());
    u8 characteristic1 = static_cast<u8>(ui->comboBoxCharacteristicDay4_1->currentIndex());
    bool genderLocked1
        = raid1.getGender() != 0 || raid1.getGenderRatio() == 0 || raid1.getGenderRatio() == 254 || raid1.getGenderRatio() == 255;
    bool allowHiddenAbility1 = raid1.getAbility() == 4;
    Pokemon mon1(ivs1, species1, ability1, nature1, characteristic1, genderLocked1, allowHiddenAbility1);

    u16 species2 = raid2.getSpecies();
    u8 ability2 = static_cast<u8>(ui->comboBoxAbilityDay4_2->currentIndex());
    if (ability2 != 2 && (info2.getAbility1() == info2.getAbility2()))
    {
        ability2 = 255;
    }
    u8 nature2 = static_cast<u8>(ui->comboBoxNatureDay4_2->currentIndex());
    u8 characteristic2 = static_cast<u8>(ui->comboBoxCharacteristicDay4_2->currentIndex());
    bool genderLocked2
        = raid2.getGender() != 0 || raid2.getGenderRatio() == 0 || raid2.getGenderRatio() == 254 || raid2.getGenderRatio() == 255;
    bool allowHiddenAbility2 = raid2.getAbility() == 4;
    Pokemon mon2(ivs2, species2, ability2, nature2, characteristic2, genderLocked2, allowHiddenAbility2);

    u16 species3 = raid3.getSpecies();
    u8 ability3 = static_cast<u8>(ui->comboBoxAbilityDay5->currentIndex());
    if (ability3 != 2 && (info3.getAbility1() == info3.getAbility2()))
    {
        ability3 = 255;
    }
    u8 nature3 = static_cast<u8>(ui->comboBoxNatureDay5->currentIndex());
    u8 characteristic3 = static_cast<u8>(ui->comboBoxCharacteristicDay5->currentIndex());
    bool genderLocked3
        = raid3.getGender() != 0 || raid3.getGenderRatio() == 0 || raid3.getGenderRatio() == 254 || raid3.getGenderRatio() == 255;
    bool allowHiddenAbility3 = raid3.getAbility() == 4;
    Pokemon mon3(ivs3, species3, ability3, nature3, characteristic3, genderLocked3, allowHiddenAbility3);

    QVector<Pokemon> pokemon = { mon1, mon2, mon3 };

    ui->pushButtonSearch->setEnabled(false);

    auto *searcher = new SeedSearcher(pokemon, fixedIVs, ivCount, ui->checkBoxStop->isChecked());
    searcher->setIVs(getIVs(ivs1, ivs2));

    QSettings setting;
    int threads = setting.value("settings/thread", QThread::idealThreadCount()).toInt();
    auto *thread = QThread::create([=] { searcher->startSearch(3, threads); });

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);

        QVector<u64> seeds = searcher->getResults();
        delete searcher;

        for (u64 seed : seeds)
        {
            ui->textEdit->append(QString("0x%1").arg(QString::number(seed, 16).toUpper().rightJustified(16, '0')));
        }
    });

    thread->start();
}

void SeedCalculator::clear()
{
    ui->textEdit->clear();
}
