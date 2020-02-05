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

#include "RaidInfo12.hpp"
#include "ui_RaidInfo12.h"
#include <Core/Loader/PersonalLoader.hpp>
#include <Core/Util/Translator.hpp>

RaidInfo12::RaidInfo12(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RaidInfo12)
{
    ui->setupUi(this);

    setupModels();
}

RaidInfo12::~RaidInfo12()
{
    delete ui;
}

void RaidInfo12::setDen(const Den &den, Game game)
{
    this->den = den;
    this->game = game;

    ui->comboBoxRaidDay1->clear();
    ui->comboBoxRaidDay2->clear();

    auto raids = den.getRaids(game);
    for (u8 i = 0; i < raids.size(); i++)
    {
        auto raid = raids.at(i);
        if (raid.getIVCount() == 1)
        {
            ui->comboBoxRaidDay1->addItem(QString("%1: %2").arg(Translator::getSpecie(raid.getSpecies()), raid.getStarDisplay()), i);
        }

        if (raid.getStar(0) || raid.getStar(1))
        {
            ui->comboBoxRaidDay2->addItem(QString("%1: %2").arg(Translator::getSpecie(raid.getSpecies()), raid.getStarDisplay()), i);
        }
    }

    raidDay1IndexChanged(0);
    raidDay2IndexChanged(0);
}

QVector<u8> RaidInfo12::getIVs(int index) const
{
    if (index == 0)
    {
        return { static_cast<u8>(ui->spinBoxHPDay1->value()),  static_cast<u8>(ui->spinBoxAtkDay1->value()),
                 static_cast<u8>(ui->spinBoxDefDay1->value()), static_cast<u8>(ui->spinBoxSpADay1->value()),
                 static_cast<u8>(ui->spinBoxSpDDay1->value()), static_cast<u8>(ui->spinBoxSpeDay1->value()) };
    }

    return { static_cast<u8>(ui->spinBoxHPDay2->value()),  static_cast<u8>(ui->spinBoxAtkDay2->value()),
             static_cast<u8>(ui->spinBoxDefDay2->value()), static_cast<u8>(ui->spinBoxSpADay2->value()),
             static_cast<u8>(ui->spinBoxSpDDay2->value()), static_cast<u8>(ui->spinBoxSpeDay2->value()) };
}

QVector<int> RaidInfo12::getIVCounts() const
{
    return { ui->spinBoxIVCountDay1->value(), ui->spinBoxIVCountDay2->value() };
}

Pokemon RaidInfo12::getPokemonDay1() const
{
    auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay1->currentData().toInt()), game);
    auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

    auto ivs = getIVs(0);
    u16 species = raid.getSpecies();
    u8 altform = raid.getAltForm();
    u8 ability = static_cast<u8>(ui->comboBoxAbilityDay1->currentIndex());
    if (ability != 2 && (info.getAbility1() == info.getAbility2()))
    {
        ability = 255;
    }
    u8 nature = static_cast<u8>(ui->comboBoxNatureDay1->currentIndex());
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristicDay1->currentIndex());
    bool genderLocked = raid.getGender() != 0 || raid.getGenderRatio() == 0 || raid.getGenderRatio() == 254 || raid.getGenderRatio() == 255;
    bool allowHiddenAbility = raid.getAbility() == 4;

    return Pokemon(ivs, species, altform, ability, nature, characteristic, genderLocked, allowHiddenAbility);
}

Pokemon RaidInfo12::getPokemonDay2() const
{
    auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay2->currentData().toInt()), game);
    auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

    auto ivs = getIVs(1);
    u16 species = raid.getSpecies();
    u8 altform = raid.getAltForm();
    u8 ability = static_cast<u8>(ui->comboBoxAbilityDay2->currentIndex());
    if (ability != 2 && (info.getAbility1() == info.getAbility2()))
    {
        ability = 255;
    }
    u8 nature = static_cast<u8>(ui->comboBoxNatureDay2->currentIndex());
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristicDay2->currentIndex());
    bool genderLocked = raid.getGender() != 0 || raid.getGenderRatio() == 0 || raid.getGenderRatio() == 254 || raid.getGenderRatio() == 255;
    bool allowHiddenAbility = raid.getAbility() == 4;

    return Pokemon(ivs, species, altform, ability, nature, characteristic, genderLocked, allowHiddenAbility);
}

bool RaidInfo12::isValid() const
{
    return ui->labelCheck->text() == tr("Valid");
}

void RaidInfo12::setupModels()
{
    ui->comboBoxNatureDay1->addItems(Translator::getNatures());
    ui->comboBoxNatureDay2->addItems(Translator::getNatures());

    ui->comboBoxCharacteristicDay1->addItems(Translator::getCharacteristics());
    ui->comboBoxCharacteristicDay2->addItems(Translator::getCharacteristics());

    connect(ui->comboBoxRaidDay1, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RaidInfo12::raidDay1IndexChanged);
    connect(ui->comboBoxRaidDay2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RaidInfo12::raidDay2IndexChanged);
    connect(ui->spinBoxHPDay1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkValid(); });
    connect(ui->spinBoxAtkDay1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkValid(); });
    connect(ui->spinBoxDefDay1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkValid(); });
    connect(ui->spinBoxSpADay1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkValid(); });
    connect(ui->spinBoxSpDDay1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkValid(); });
    connect(ui->spinBoxSpeDay1, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { checkValid(); });
}

void RaidInfo12::raidDay1IndexChanged(int index)
{
    if (index >= 0)
    {
        auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay1->currentData().toInt()), game);
        auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

        ui->spinBoxIVCountDay1->setValue(raid.getIVCount());

        ui->comboBoxAbilityDay1->clear();
        ui->comboBoxAbilityDay1->addItem(Translator::getAbility(info.getAbility1()), info.getAbility1());
        if (info.getAbility1() != info.getAbility2())
        {
            ui->comboBoxAbilityDay1->addItem(Translator::getAbility(info.getAbility2()), info.getAbility2());
        }
        if (raid.getAbility() == 4) // Allow hidden ability
        {
            ui->comboBoxAbilityDay1->addItem(Translator::getAbility(info.getAbilityH()), info.getAbilityH());
        }
    }
}

void RaidInfo12::raidDay2IndexChanged(int index)
{
    if (index >= 0)
    {
        auto raid = den.getRaid(static_cast<u8>(ui->comboBoxRaidDay2->currentData().toInt()), game);
        auto info = PersonalLoader::getInfo(raid.getSpecies(), raid.getAltForm());

        ui->spinBoxIVCountDay2->setValue(raid.getIVCount());

        ui->comboBoxAbilityDay2->clear();
        ui->comboBoxAbilityDay2->addItem(Translator::getAbility(info.getAbility1()), info.getAbility1());
        if (info.getAbility1() != info.getAbility2())
        {
            ui->comboBoxAbilityDay2->addItem(Translator::getAbility(info.getAbility2()), info.getAbility2());
        }
        if (raid.getAbility() == 4) // Allow hidden ability
        {
            ui->comboBoxAbilityDay2->addItem(Translator::getAbility(info.getAbilityH()), info.getAbilityH());
        }
    }
}

void RaidInfo12::checkValid()
{
    auto ivs = getIVs(0);

    if (ivs.count(31) == 1)
    {
        ui->labelCheck->setText(tr("Valid"));
    }
    else
    {
        ui->labelCheck->setText(tr("Invalid"));
    }
}
