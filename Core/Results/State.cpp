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

#include "State.hpp"

constexpr u8 statOrder[6] = { 0, 1, 2, 5, 3, 4 };

State::State(u64 seed, u32 advances) : seed(seed), advances(advances), ivs { 255, 255, 255, 255, 255, 255 }
{
}

u64 State::getSeed() const
{
    return seed;
}

u32 State::getAdvances() const
{
    return advances;
}

u32 State::getEC() const
{
    return ec;
}

void State::setEC(u32 ec)
{
    this->ec = ec;
}

u32 State::getPID() const
{
    return pid;
}

void State::setPID(u32 pid)
{
    this->pid = pid;
}

u8 State::getNature() const
{
    return nature;
}

void State::setNature(u8 nature)
{
    this->nature = nature;
}

u8 State::getAbility() const
{
    return ability;
}

void State::setAbility(u8 ability)
{
    this->ability = ability;
}

u8 State::getGender() const
{
    return gender;
}

void State::setGender(u8 gender)
{
    this->gender = gender;
}

u8 State::getShiny() const
{
    return shiny;
}

void State::setShiny(u8 shiny)
{
    this->shiny = shiny;
}

u8 State::getIV(int index) const
{
    return ivs[index];
}

void State::setIV(int index, u8 iv)
{
    ivs[index] = iv;
}

u8 State::getCharacteristic() const
{
    u8 charStat = ec % 6;
    for (u8 i = 0; i < 6; i++)
    {
        u8 stat = (charStat + i) % 6;
        if (ivs[statOrder[stat]] == 31)
        {
            return stat;
        }
    }
    return charStat;
}
