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

#ifndef PERSONALINFO_HPP
#define PERSONALINFO_HPP

#include <Core/Global.hpp>
#include <QVector>

class PersonalInfo
{
public:
    PersonalInfo() = default;
    PersonalInfo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 genderRatio, u16 ability1, u16 ability2, u16 abilityH, u8 formCount,
                 u16 formStatIndex, bool included);
    QVector<u8> getBaseStats() const;
    u8 getGenderRatio() const;
    u16 getAbility1() const;
    u16 getAbility2() const;
    u16 getAbilityH() const;
    u8 getFormCount() const;
    u16 getFormStatIndex() const;
    bool getIncluded() const;

private:
    u8 hp, atk, def, spa, spd, spe;
    u8 genderRatio;
    u16 ability1, ability2, abilityH;
    u8 formCount;
    u16 formStatIndex;
    bool included;
};

#endif // PERSONALINFO_HPP
