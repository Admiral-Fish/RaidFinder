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

#include "PersonalLoader.hpp"
#include <Core/Resources.hpp>
#include <vector>

namespace
{
    std::vector<PersonalInfo> info;
}

namespace PersonalLoader
{
    void init()
    {
        for (size_t i = 0; i < personal_swsh.size(); i += 176)
        {
            u8 hp = personal_swsh[i];
            u8 atk = personal_swsh[i + 1];
            u8 def = personal_swsh[i + 2];
            u8 spe = personal_swsh[i + 3];
            u8 spa = personal_swsh[i + 4];
            u8 spd = personal_swsh[i + 5];
            u8 genderRatio = personal_swsh[i + 18];
            u16 ability1 = (personal_swsh[i + 25] << 8) | personal_swsh[i + 24];
            u16 ability2 = (personal_swsh[i + 27] << 8) | personal_swsh[i + 26];
            u16 abilityH = (personal_swsh[i + 29] << 8) | personal_swsh[i + 28];
            u16 formStatIndex = (personal_swsh[i + 31] << 8) | personal_swsh[i + 30];
            u8 formCount = personal_swsh[i + 32];
            bool included = (personal_swsh[i + 33] >> 6) & 1;

            info.emplace_back(hp, atk, def, spa, spd, spe, genderRatio, ability1, ability2, abilityH, formCount, formStatIndex, included);
        }
    }

    PersonalInfo getInfo(u16 species, u8 form)
    {
        PersonalInfo base = info[species];

        u16 formIndex = base.getFormStatIndex();

        if (form == 0 || formIndex == 0)
        {
            return base;
        }

        return info[formIndex + form - 1];
    }
}
