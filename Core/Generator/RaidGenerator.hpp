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

#ifndef RAIDGENERATOR_HPP
#define RAIDGENERATOR_HPP

#include <Core/Results/Raid.hpp>
#include <Core/Results/State.hpp>
#include <Core/StateFilter.hpp>
#include <vector>

class RaidGenerator
{
public:
    RaidGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const Raid &raid);
    std::vector<State> generate(const StateFilter &filter, u64 seed) const;

private:
    u32 initialAdvances;
    u32 maxAdvances;
    u16 tid;
    u16 sid;
    u16 species;
    u8 altform;
    u8 abilityType;
    u8 shinyType;
    u8 ivCount;
    u8 genderType;
    u8 genderRatio;
};

#endif // RAIDGENERATOR_HPP
