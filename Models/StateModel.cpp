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

#include "StateModel.hpp"
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <cmath>

StateModel::StateModel(QObject *parent) : TableModel<State>(parent), showStats(false), level(1)
{
}

void StateModel::setLevel(int level)
{
    this->level = level;
    emit dataChanged(index(0, 1), index(rowCount(), 6), { Qt::DisplayRole });
}

void StateModel::setShowStats(bool showStats)
{
    this->showStats = showStats;
    emit dataChanged(index(0, 1), index(rowCount(), 6), { Qt::DisplayRole });
}

void StateModel::setInfo(const PersonalInfo &info)
{
    this->info = info;
}

int StateModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 15;
}

QVariant StateModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto &state = model.at(index.row());
        int column = index.column();
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            if (!showStats)
            {
                return state.getIV(static_cast<u8>(column - 1));
            }
            else
            {
                double stat = std::floor(((2 * info.getBaseStat(column - 1) + state.getIV(column - 1)) * level) / 100.0);
                if (column == 1)
                {
                    stat += level + 10;
                }
                else
                {
                    stat = std::floor((stat + 5) * Nature::getNatureModifier(state.getNature(), column - 1));
                }

                return stat;
            }
        case 7:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 8:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 9:
        {
            u8 ability = state.getAbility();
            if (ability == 0)
            {
                return "1: " + QString::fromStdString(Translator::getAbility(info.getAbility1()));
            }

            if (ability == 1)
            {
                return "2: " + QString::fromStdString(Translator::getAbility(info.getAbility2()));
            }

            return "H: " + QString::fromStdString(Translator::getAbility(info.getAbilityH()));
        }
        case 10:
        {
            u8 gender = state.getGender();
            return gender == 0 ? "♂" : gender == 1 ? "♀" : "-";
        }
        case 11:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
        case 12:
            return QString::number(state.getSeed(), 16).toUpper();
        case 13:
            return QString::number(state.getEC(), 16).toUpper();
        case 14:
            return QString::number(state.getPID(), 16).toUpper();
        }
    }
    return QVariant();
}

QVariant StateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
