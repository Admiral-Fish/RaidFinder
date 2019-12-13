/*
 * This file is part of RaidFinder
 * Copyright (C) 2019 by Admiral_Fish
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

#include "Raid.hpp"

Raid::Raid(u8 ability, u8 altform, u8 ivCount, u8 gender, u8 genderRatio, bool gigantamax, u16 species)
{
    this->ability = ability;
    this->altform = altform;
    this->ivCount = ivCount;
    this->gender = gender;
    this->genderRatio = genderRatio;
    this->gigantamax = gigantamax;
    this->species = species;
}

u8 Raid::getAbility() const
{
    return ability;
}

u8 Raid::getAltForm() const
{
    return altform;
}

u8 Raid::getIVCount() const
{
    return ivCount;
}

u8 Raid::getGender() const
{
    return gender;
}

u8 Raid::getGenderRatio() const
{
    return genderRatio;
}

bool Raid::getGigantamax() const
{
    return gigantamax;
}

u16 Raid::getSpecies() const
{
    return species;
}
