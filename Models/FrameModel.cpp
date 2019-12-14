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

#include "FrameModel.hpp"
#include <Core/Util/Translator.hpp>

FrameModel::FrameModel(QObject *parent)
    : TableModel<Frame>(parent)
{
}

int FrameModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 13;
}

QVariant FrameModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto &frame = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return frame.getFrame();
        case 1:
            return frame.getIV(0);
        case 2:
            return frame.getIV(1);
        case 3:
            return frame.getIV(2);
        case 4:
            return frame.getIV(3);
        case 5:
            return frame.getIV(4);
        case 6:
            return frame.getIV(5);
        case 7:
            return frame.getShiny() ? tr("Yes") : tr("No");
        case 8:
            return Translator::getNature(frame.getNature());
        case 9:
        {
            u8 ability = frame.getAbility();
            return ability == 0 ? "1" : ability == 1 ? "2" : "H";
        }
        case 10:
        {
            u8 gender = frame.getGender();
            return gender == 0 ? "♂" : gender == 1 ? "♀" : "-";
        }
        case 11:
            return QString::number(frame.getEC(), 16).toUpper();
        case 12:
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
