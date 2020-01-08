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
    model->setHorizontalHeaderLabels(QStringList({ tr("Locations") }));
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
        encounterLookup.clear();
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

                if (!encounterLookup.contains(species))
                {
                    encounterLookup[species] = QSet<QPair<u8, u8>> { qMakePair(i + 1, DenLoader::getLocation(i)) };
                }
                else
                {
                    encounterLookup[species].insert(qMakePair(i + 1, DenLoader::getLocation(i)));
                }
            }

            auto rareRaids = rareDen.getRaids(game);
            for (auto raid : rareRaids)
            {
                u16 species = raid.getSpecies();

                if (!encounterLookup.contains(species))
                {
                    encounterLookup[species] = QSet<QPair<u8, u8>> { qMakePair(i + 1, DenLoader::getLocation(i)) };
                }
                else
                {
                    encounterLookup[species].insert(qMakePair(i + 1, DenLoader::getLocation(i)));
                }
            }
        }

        for (auto specie : encounterLookup.keys())
        {
            ui->comboBoxPokemon->addItem(Translator::getSpecie(specie), specie);
        }
    }
}

void EncounterLookup::find()
{
    model->removeRows(0, model->rowCount());
    u16 species = static_cast<u16>(ui->comboBoxPokemon->currentData().toInt());

    auto encounters = encounterLookup[species];
    auto locations = encounters.values();
    std::sort(locations.begin(), locations.end(),
              [](const QPair<u8, u8> pair1, const QPair<u8, u8> pair2) { return pair1.first < pair2.first; });

    for (auto location : locations)
    {
        QString entry = QString("%1: %2").arg(location.first).arg(Translator::getLocation(location.second));
        model->appendRow(new QStandardItem(entry));
    }
}
