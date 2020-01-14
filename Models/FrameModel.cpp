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

#include "FrameModel.hpp"
#include <Core/Util/Translator.hpp>

FrameModel::FrameModel(QObject *parent) : TableModel<Frame>(parent)
{
}

void FrameModel::setInfo(const PersonalInfo &info)
{
    this->info = info;
}

int FrameModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 15;
}

QVariant FrameModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto &frame = model.at(index.row());
        int column = index.column();
        switch (column)
        {
        case 0:
            return frame.getFrame();
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return frame.getIV(static_cast<u8>(column - 1));
        case 7:
        {
            u8 shiny = frame.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 8:
            return Translator::getNature(frame.getNature());
        case 9:
        {
            u8 ability = frame.getAbility();
            if (ability == 0)
            {
                return "1: " + Translator::getAbility(info.getAbility1());
            }
            else if (ability == 1)
            {
                return "2: " + Translator::getAbility(info.getAbility2());
            }

            return "H: " + Translator::getAbility(info.getAbilityH());
        }
        case 10:
        {
            u8 gender = frame.getGender();
            return gender == 0 ? "♂" : gender == 1 ? "♀" : "-";
        }
        case 11:
        {
            return Translator::getCharacteristic(frame.getCharacteristic());
        }
        case 12:
            return QString::number(frame.getSeed(), 16).toUpper();
        case 13:
            return QString::number(frame.getEC(), 16).toUpper();
        case 14:
            return QString::number(frame.getPID(), 16).toUpper();
        }
    }
    return QVariant();
}

QVariant FrameModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
