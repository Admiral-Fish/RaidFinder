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

#ifndef RAID_HPP
#define RAID_HPP

#include <Core/Util/Global.hpp>
#include <array>
#include <string>

class Raid
{
public:
    Raid() = default;
    Raid(u8 ability, u8 altform, u8 ivCount, u8 gender, bool gigantamax, u16 species, const std::array<bool, 5> &star, u8 shinyType = 0);
    u8 getAbility() const;
    u8 getAltForm() const;
    u8 getIVCount() const;
    u8 getGender() const;
    u8 getGenderRatio() const;
    bool getGigantamax() const;
    u16 getSpecies() const;
    u8 getShiny() const;
    std::string getStarDisplay() const;

private:
    u8 ability;
    u8 altform;
    u8 ivCount;
    u8 gender;
    u8 genderRatio;
    bool gigantamax;
    u16 species;
    std::array<bool, 5> star;
    u8 shinyType;
};

#endif // RAID_HPP
