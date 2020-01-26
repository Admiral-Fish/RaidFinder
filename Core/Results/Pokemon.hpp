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

#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <Core/Util/Global.hpp>
#include <QVector>

class Pokemon
{
public:
    Pokemon() = default;
    Pokemon(const QVector<u8> &ivs, u16 species, u8 ability, u8 nature, u8 characteristic, bool genderLocked, bool allowHiddenAbility);
    u8 getIV(u8 index) const;
    u16 getSpecies() const;
    u8 getAbility() const;
    u8 getNature() const;
    u8 getCharacteristic() const;
    bool getGenderLocked() const;
    bool getAllowHiddenAbility() const;
    bool isCharacterized(u8 num) const;

private:
    QVector<u8> ivs;
    u16 species;
    u8 ability;
    u8 nature;
    u8 characteristic;
    bool genderLocked;
    bool allowHiddenAbility;
};

#endif // POKEMON_HPP
