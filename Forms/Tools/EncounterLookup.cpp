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
#include <Core/Util/Translator.hpp>
#include <QCompleter>

EncounterLookup::EncounterLookup(Game version, QWidget *parent) : QWidget(parent), ui(new Ui::EncounterLookup)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
    ui->comboBoxGame->setCurrentIndex(ui->comboBoxGame->findData(version));
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

    ui->comboBoxPokemon->setEditable(true);
    ui->comboBoxPokemon->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBoxPokemon->completer()->setCompletionMode(QCompleter::PopupCompletion);

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

        for (u8 i = 0; i < 100; i++)
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

    for (u8 i = 0; i < 100; i++)
    {
        auto normalDen = DenLoader::getDen(i, 0);
        auto normalRaids = normalDen.getRaids(game);
        for (auto raid : normalRaids)
        {
            if (raid.getSpecies() == species) {
                addRow(i, false, raid);
            }
        }

        auto rareDen = DenLoader::getDen(i, 1);
        auto rareRaids = rareDen.getRaids(game);
        for (auto raid : rareRaids)
        {
            if (raid.getSpecies() == species) {
                addRow(i, true, raid);
            }
        }
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void EncounterLookup::addRow(int denId, bool isRare, Raid raid)
{
    QString location = QString("%1: %2").arg(denId + 1).arg(Translator::getLocation(DenLoader::getLocation(denId)));
    QString rarity = isRare ? tr("Rare") : tr("Normal");
    QString iv = QString::number(raid.getIVCount());
    QString ha = raid.getAbility() == 4 ? tr("Yes") : tr("No");
    QString gigantamax = raid.getGigantamax() ? tr("Yes") : tr("No");

    auto iLocation = new QStandardItem(location);
    auto iRarity = new QStandardItem(rarity);
    auto iIV = new QStandardItem(iv);
    auto iHA = new QStandardItem(ha);
    auto iGMax = new QStandardItem(gigantamax);

    iRarity->setTextAlignment(Qt::AlignCenter);
    iIV->setTextAlignment(Qt::AlignCenter);
    iHA->setTextAlignment(Qt::AlignCenter);
    iGMax->setTextAlignment(Qt::AlignCenter);

    model->appendRow({ iLocation, iRarity, iIV, iHA, iGMax });
}
