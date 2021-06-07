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

#include "ProfileModel.hpp"

ProfileModel::ProfileModel(QObject *parent) : TableModel<Profile>(parent)
{
}

int ProfileModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 4;
}

QVariant ProfileModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &profile = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return QString::fromStdString(profile.getName());
        case 1:
            return QString::fromStdString(profile.getVersionString());
        case 2:
            return profile.getTID();
        case 3:
            return profile.getSID();
        }
    }
    return QVariant();
}

QVariant ProfileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
