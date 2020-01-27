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

#include "RaidInfo35.hpp"
#include "ui_RaidInfo35.h"
#include <Core/Loader/PersonalLoader.hpp>
#include <Core/Util/Translator.hpp>

RaidInfo35::RaidInfo35(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RaidInfo35)
{
    ui->setupUi(this);

    setupModels();
}

RaidInfo35::~RaidInfo35()
{
    delete ui;
}

void RaidInfo35::setDen(const Den &den, Game game)
{
    this->den = den;
    this->game = game;

    ui->comboBoxRaidDay4_1->clear();
    ui->comboBoxRaidDay4_2->clear();
    ui->comboBoxRaidDay5->clear();
    ui->comboBoxRaidDay6->clear();

    auto raids = den.getRaids(game);
    for (u8 i = 0; i < raids.size(); i++)
    {
        auto raid = raids.at(i);
        if ((raid.getIVCount() == 2 || raid.getIVCount() == 3) && (raid.getStar(2) || raid.getStar(3) || raid.getStar(4)))
        {
            ui->comboBoxRaidDay4_1->addItem(QString("%1: %2").arg(Translator::getSpecie(raid.getSpecies()), raid.getStarDisplay()), i);
        }

        if ((raid.getIVCount() == 3 || raid.getIVCount() == 4) && (raid.getStar(2) || raid.getStar(3) || raid.getStar(4)))
        {
            ui->comboBoxRaidDay4_2->addItem(QString("%1: %2").arg(Translator::getSpecie(raid.getSpecies()), raid.getStarDisplay()), i);
        }

        if (raid.getStar(2) || raid.getStar(3) || raid.getStar(4))
        {
            ui->comboBoxRaidDay5->addItem(QString("%1: %2").arg(Translator::getSpecie(raid.getSpecies()), raid.getStarDisplay()), i);
            ui->comboBoxRaidDay6->addItem(QString("%1: %2").arg(Translator::getSpecie(raid.getSpecies()), raid.getStarDisplay()), i);
        }
    }

    raidDay4_1IndexChanged(0);
    raidDay4_2IndexChanged(0);
    raidDay5IndexChanged(0);
    raidDay6IndexChanged(0);
}

bool RaidInfo35::isValid() const
{
    return ui->labelCheck->text() != tr("Invalid");
}

QVector<u8> RaidInfo35::getIVs(int index) const
{
    if (index == 0)
    {
        return { static_cast<u8>(ui->spinBoxHPDay4_1->value()),  static_cast<u8>(ui->spinBoxAtkDay4_1->value()),
                 static_cast<u8>(ui->spinBoxDefDay4_1->value()), static_cast<u8>(ui->spinBoxSpADay4_1->value()),
                 static_cast<u8>(ui->spinBoxSpDDay4_1->value()), static_cast<u8>(ui->spinBoxSpeDay4_1->value()) };
    }

    if (index == 1)
    {
        return { static_cast<u8>(ui->spinBoxHPDay4_2->value()),  static_cast<u8>(ui->spinBoxAtkDay4_2->value()),
                 static_cast<u8>(ui->spinBoxDefDay4_2->value()), static_cast<u8>(ui->spinBoxSpADay4_2->value()),
                 static_cast<u8>(ui->spinBoxSpDDay4_2->value()), static_cast<u8>(ui->spinBoxSpeDay4_2->value()) };
    }

    if (index == 2)
    {
        return { static_cast<u8>(ui->spinBoxHPDay5->value()),  static_cast<u8>(ui->spinBoxAtkDay5->value()),
                 static_cast<u8>(ui->spinBoxDefDay5->value()), static_cast<u8>(ui->spinBoxSpADay5->value()),
                 static_cast<u8>(ui->spinBoxSpDDay5->value()), static_cast<u8>(ui->spinBoxSpeDay5->value()) };
    }

    return { static_cast<u8>(ui->spinBoxHPDay6->value()),  static_cast<u8>(ui->spinBoxAtkDay6->value()),
             static_cast<u8>(ui->spinBoxDefDay6->value()), static_cast<u8>(ui->spinBoxSpADay6->value()),
             static_cast<u8>(ui->spinBoxSpDDay6->value()), static_cast<u8>(ui->spinBoxSpeDay6->value()) };
}

QVector<u8> RaidInfo35::getConditionIVs() const
{
    auto ivs1 = getIVs(0);
    auto ivs2 = getIVs(1);
    int ivCount2 = ivs2.count(31);

    QVector<bool> flags;
    for (int i = 0; i < 6; i++)
    {
        flags.append(ivs1.at(i) == 31);
    }

    int ivCount1 = ui->spinBoxIVCountDay4_1->value();
    int index = 0;
    QVector<u8> ivs(6);
    for (int i = 0; i < 6; i++)
    {
        if (ivs1.at(i) != 31)
        {
            ivs[index++] = ivs1.at(i);
            int position = ivs1.at(i) % 8;
            if (position < 6 && !flags.at(position))
            {
                flags[position] = true;
                ivCount1++;
                if (ivCount1 == ivCount2)
                {
                    break;
                }
            }
        }
    }

    for (int i = 0; i < 6; i++)
    {
        if (!flags.at(i))
        {
            ivs[index++] = ivs2.at(i);
            if (index == 6)
            {
                break;
            }
        }
    }

    return ivs;
}

QVector<int> RaidInfo35::getIVCounts() const
{
    return { ui->spinBoxIVCountDay4_1->value(), ui->spinBoxIVCountDay4_2->value(), ui->spinBoxIVCountDay5->value(),
             ui->spinBoxIVCountDay6->value() };
}

Pokemon RaidInfo35::getPokemonDay4_1() const
{
    auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay4_1->currentData().toInt()), game);
    auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

    auto ivs = getIVs(0);
    u16 species = raid.getSpecies();
    u8 ability = static_cast<u8>(ui->comboBoxAbilityDay4_1->currentIndex());
    if (ability != 2 && (info.getAbility1() == info.getAbility2()))
    {
        ability = 255;
    }
    u8 nature = static_cast<u8>(ui->comboBoxNatureDay4_1->currentIndex());
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristicDay4_1->currentIndex());
    bool genderLocked = raid.getGender() != 0 || raid.getGenderRatio() == 0 || raid.getGenderRatio() == 254 || raid.getGenderRatio() == 255;
    bool allowHiddenAbility = raid.getAbility() == 4;

    return Pokemon(ivs, species, ability, nature, characteristic, genderLocked, allowHiddenAbility);
}

Pokemon RaidInfo35::getPokemonDay4_2() const
{
    auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay4_2->currentData().toInt()), game);
    auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

    auto ivs = getIVs(1);
    u16 species = raid.getSpecies();
    u8 ability = static_cast<u8>(ui->comboBoxAbilityDay4_2->currentIndex());
    if (ability != 2 && (info.getAbility1() == info.getAbility2()))
    {
        ability = 255;
    }
    u8 nature = static_cast<u8>(ui->comboBoxNatureDay4_2->currentIndex());
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristicDay4_2->currentIndex());
    bool genderLocked = raid.getGender() != 0 || raid.getGenderRatio() == 0 || raid.getGenderRatio() == 254 || raid.getGenderRatio() == 255;
    bool allowHiddenAbility = raid.getAbility() == 4;

    return Pokemon(ivs, species, ability, nature, characteristic, genderLocked, allowHiddenAbility);
}

Pokemon RaidInfo35::getPokemonDay5() const
{
    auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay5->currentData().toInt()), game);
    auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

    auto ivs = getIVs(2);
    u16 species = raid.getSpecies();
    u8 ability = static_cast<u8>(ui->comboBoxAbilityDay5->currentIndex());
    if (ability != 2 && (info.getAbility1() == info.getAbility2()))
    {
        ability = 255;
    }
    u8 nature = static_cast<u8>(ui->comboBoxNatureDay5->currentIndex());
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristicDay5->currentIndex());
    bool genderLocked = raid.getGender() != 0 || raid.getGenderRatio() == 0 || raid.getGenderRatio() == 254 || raid.getGenderRatio() == 255;
    bool allowHiddenAbility = raid.getAbility() == 4;

    return Pokemon(ivs, species, ability, nature, characteristic, genderLocked, allowHiddenAbility);
}

Pokemon RaidInfo35::getPokemonDay6() const
{
    auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay6->currentData().toInt()), game);
    auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

    auto ivs = getIVs(3);
    u16 species = raid.getSpecies();
    u8 ability = static_cast<u8>(ui->comboBoxAbilityDay6->currentIndex());
    if (ability != 2 && (info.getAbility1() == info.getAbility2()))
    {
        ability = 255;
    }
    u8 nature = static_cast<u8>(ui->comboBoxNatureDay6->currentIndex());
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristicDay6->currentIndex());
    bool genderLocked = raid.getGender() != 0 || raid.getGenderRatio() == 0 || raid.getGenderRatio() == 254 || raid.getGenderRatio() == 255;
    bool allowHiddenAbility = raid.getAbility() == 4;

    return Pokemon(ivs, species, ability, nature, characteristic, genderLocked, allowHiddenAbility);
}

void RaidInfo35::displayDay6(int flag)
{
    ui->groupBoxDay6->setVisible(flag == Qt::Checked);
}

void RaidInfo35::setupModels()
{
    ui->comboBoxNatureDay4_1->addItems(Translator::getNatures());
    ui->comboBoxNatureDay4_2->addItems(Translator::getNatures());
    ui->comboBoxNatureDay5->addItems(Translator::getNatures());
    ui->comboBoxNatureDay6->addItems(Translator::getNatures());

    ui->comboBoxCharacteristicDay4_1->addItems(Translator::getCharacteristics());
    ui->comboBoxCharacteristicDay4_2->addItems(Translator::getCharacteristics());
    ui->comboBoxCharacteristicDay5->addItems(Translator::getCharacteristics());
    ui->comboBoxCharacteristicDay6->addItems(Translator::getCharacteristics());

    connect(ui->comboBoxRaidDay4_1, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RaidInfo35::raidDay4_1IndexChanged);
    connect(ui->comboBoxRaidDay4_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RaidInfo35::raidDay4_2IndexChanged);
    connect(ui->comboBoxRaidDay5, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RaidInfo35::raidDay5IndexChanged);
    connect(ui->comboBoxRaidDay6, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RaidInfo35::raidDay6IndexChanged);
    connect(ui->spinBoxHPDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxAtkDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxDefDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxSpADay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxSpDDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
    connect(ui->spinBoxSpeDay4_1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkDay4(); });
}

void RaidInfo35::checkDay4()
{
    QVector<u8> ivs = getIVs(0);

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

void RaidInfo35::raidDay4_1IndexChanged(int index)
{
    if (index >= 0)
    {
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

void RaidInfo35::raidDay4_2IndexChanged(int index)
{
    if (index >= 0)
    {
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

void RaidInfo35::raidDay5IndexChanged(int index)
{
    if (index >= 0)
    {
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

void RaidInfo35::raidDay6IndexChanged(int index)
{
    if (index >= 0)
    {
        auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay6->currentData().toInt()), game);
        auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

        ui->spinBoxIVCountDay6->setValue(raid.getIVCount());

        ui->comboBoxAbilityDay6->clear();
        ui->comboBoxAbilityDay6->addItem(Translator::getAbility(info.getAbility1()), info.getAbility1());
        if (info.getAbility1() != info.getAbility2())
        {
            ui->comboBoxAbilityDay6->addItem(Translator::getAbility(info.getAbility2()), info.getAbility2());
        }
        if (raid.getAbility() == 4) // Allow hidden ability
        {
            ui->comboBoxAbilityDay6->addItem(Translator::getAbility(info.getAbilityH()), info.getAbilityH());
        }
    }
}
