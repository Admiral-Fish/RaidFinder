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

#include "EncounterLookup.hpp"
#include "ui_EncounterLookup.h"
#include <Core/Loader/DenLoader.hpp>
#include <Core/Util/Game.hpp>
#include <Core/Util/Translator.hpp>

EncounterLookup::EncounterLookup(QWidget *parent) : QWidget(parent), ui(new Ui::EncounterLookup)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

EncounterLookup::~EncounterLookup()
{
    delete ui;
}

void EncounterLookup::setupModels()
{
    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList({ tr("Locations"), tr("Rarity"), tr("IV Count"), tr("HA"), tr("Gigantamax") }));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->comboBoxGame->setItemData(0, Game::Sword);
    ui->comboBoxGame->setItemData(1, Game::Shield);
    gameIndexChanged(0);

    connect(ui->comboBoxGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EncounterLookup::gameIndexChanged);
    connect(ui->pushButtonFind, &QPushButton::clicked, this, &EncounterLookup::find);
}

void EncounterLookup::gameIndexChanged(int index)
{
    if (index >= 0)
    {
        speciesLookup.clear();
        ui->comboBoxPokemon->clear();

        auto game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());

        for (u8 i = 0; i < 101; i++)
        {
            if (i == 16)
            {
                continue;
            }

            auto normalDen = DenLoader::getDen(i, 0);
            auto rareDen = DenLoader::getDen(i, 1);

            auto normalRaids = normalDen.getRaids(game);
            for (auto raid : normalRaids)
            {
                u16 species = raid.getSpecies();
                if (!speciesLookup.contains(species))
                {
                    speciesLookup.append(species);
                }
            }

            auto rareRaids = rareDen.getRaids(game);
            for (auto raid : rareRaids)
            {
                u16 species = raid.getSpecies();
                if (!speciesLookup.contains(species))
                {
                    speciesLookup.append(species);
                }
            }
        }

        std::sort(speciesLookup.begin(), speciesLookup.end());
        for (auto specie : speciesLookup)
        {
            ui->comboBoxPokemon->addItem(Translator::getSpecie(specie), specie);
        }
    }
}

void EncounterLookup::find()
{
    model->removeRows(0, model->rowCount());

    u16 species = static_cast<u16>(ui->comboBoxPokemon->currentData().toInt());
    auto game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());

    for (u8 i = 0; i < 101; i++)
    {
        auto normalDen = DenLoader::getDen(i, 0);
        auto normalRaids = normalDen.getRaids(game);
        for (auto raid : normalRaids)
        {
            if (raid.getSpecies() == species)
            {
                QString location = QString("%1: %2").arg(i + 1).arg(Translator::getLocation(DenLoader::getLocation(i)));
                QString rarity = tr("Normal");
                QString iv = QString::number(raid.getIVCount());
                QString ha = raid.getAbility() == 4 ? tr("Yes") : tr("No");
                QString gigantamax = raid.getGigantamax() ? tr("Yes") : tr("No");

                QList<QStandardItem *> list = { new QStandardItem(location), new QStandardItem(rarity), new QStandardItem(iv),
                                                new QStandardItem(ha), new QStandardItem(gigantamax) };
                model->appendRow(list);
            }
        }

        auto rareDen = DenLoader::getDen(i, 1);
        auto rareRaids = rareDen.getRaids(game);
        for (auto raid : rareRaids)
        {
            if (raid.getSpecies() == species)
            {
                QString location = QString("%1: %2").arg(i + 1).arg(Translator::getLocation(DenLoader::getLocation(i)));
                QString rarity = tr("Rare");
                QString iv = QString::number(raid.getIVCount());
                QString ha = raid.getAbility() == 4 ? tr("Yes") : tr("No");
                QString gigantamax = raid.getGigantamax() ? tr("Yes") : tr("No");

                QList<QStandardItem *> list = { new QStandardItem(location), new QStandardItem(rarity), new QStandardItem(iv),
                                                new QStandardItem(ha), new QStandardItem(gigantamax) };
                model->appendRow(list);
            }
        }
    }
}
