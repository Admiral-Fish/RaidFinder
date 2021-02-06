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

#include "PersonalInfo.hpp"

PersonalInfo::PersonalInfo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 genderRatio, u16 ability1, u16 ability2, u16 abilityH,
                           u8 formCount, u16 formStatIndex, bool included) :
    hp(hp),
    atk(atk),
    def(def),
    spa(spa),
    spd(spd),
    spe(spe),
    genderRatio(genderRatio),
    ability1(ability1),
    ability2(ability2),
    abilityH(abilityH),
    formCount(formCount),
    formStatIndex(formStatIndex),
    included(included)
{
}

std::vector<u8> PersonalInfo::getBaseStats() const
{
    return { hp, atk, def, spa, spd, spe };
}

u8 PersonalInfo::getBaseStat(int index) const
{
    switch (index)
    {
    case 0:
        return hp;
    case 1:
        return atk;
    case 2:
        return def;
    case 3:
        return spa;
    case 4:
        return spd;
    case 5:
        return spe;
    }
    return 0;
}

u8 PersonalInfo::getGenderRatio() const
{
    return genderRatio;
}

u16 PersonalInfo::getAbility1() const
{
    return ability1;
}

u16 PersonalInfo::getAbility2() const
{
    return ability2;
}

u16 PersonalInfo::getAbilityH() const
{
    return abilityH;
}

u8 PersonalInfo::getFormCount() const
{
    return formCount;
}

u16 PersonalInfo::getFormStatIndex() const
{
    return formStatIndex;
}

bool PersonalInfo::getIncluded() const
{
    return included;
}
