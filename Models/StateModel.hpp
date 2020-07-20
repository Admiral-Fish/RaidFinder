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

#ifndef STATEMODEL_HPP
#define STATEMODEL_HPP

#include <Core/Results/State.hpp>
#include <Core/Results/PersonalInfo.hpp>
#include <Models/TableModel.hpp>

class StateModel final : public TableModel<State>
{
    Q_OBJECT
public:
    StateModel(QObject *parent = nullptr);
    void setShowStats(bool showStats);
    void setLevel(int level);
    void setInfo(const PersonalInfo &info);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role) const override final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override final;

private:
    PersonalInfo info;
    bool showStats;
    int level;
    QStringList header = { tr("Advances"), tr("HP"),    tr("Atk"),    tr("Def"),     tr("SpA"),    tr("SpD"),
                           tr("Spe"),      tr("Shiny"), tr("Nature"), tr("Ability"), tr("Gender"), tr("Characteristic"),
                           tr("Seed"),     tr("EC"),    tr("PID") };
};

#endif // STATEMODEL_HPP
