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

#ifndef DEN_HPP
#define DEN_HPP

#include <Core/Results/Raid.hpp>
#include <Core/Util/Game.hpp>
#include <Core/Util/Global.hpp>
#include <vector>

class Den
{
public:
    Den() = default;
    Den(const std::vector<Raid> &swordRaids, const std::vector<Raid> &shieldRaids);
    Raid getRaid(u8 index, Game version) const;
    std::vector<Raid> getRaids(Game version) const;

private:
    std::vector<Raid> swordRaids;
    std::vector<Raid> shieldRaids;
};

#endif // DEN_HPP
