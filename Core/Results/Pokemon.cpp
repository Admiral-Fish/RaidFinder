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

#include "Pokemon.hpp"

Pokemon::Pokemon(const QVector<u8> &ivs, u16 species, u8 ability, u8 nature, u8 characteristic, bool genderLocked,
                 bool allowHiddenAbility) :
    ivs(ivs),
    species(species),
    ability(ability),
    nature(nature),
    characteristic(characteristic),
    genderLocked(genderLocked),
    allowHiddenAbility(allowHiddenAbility)
{
}

u8 Pokemon::getIV(u8 index) const
{
    return ivs.at(index);
}

u16 Pokemon::getSpecies() const
{
    return species;
}

u8 Pokemon::getAbility() const
{
    return ability;
}

u8 Pokemon::getNature() const
{
    return nature;
}

u8 Pokemon::getCharacteristic() const
{
    return characteristic;
}

bool Pokemon::getGenderLocked() const
{
    return genderLocked;
}

bool Pokemon::getAllowHiddenAbility() const
{
    return allowHiddenAbility;
}

bool Pokemon::checkCharacteristic(u8 index)
{
    switch (index)
    {
    case 0:
        return (ivs[0] == 31);
    case 1:
        return (ivs[1] == 31);
    case 2:
        return (ivs[2] == 31);
    case 3:
        return (ivs[5] == 31);
    case 4:
        return (ivs[3] == 31);
    case 5:
        return (ivs[4] == 31);
    default:
        return true;
    }
}
