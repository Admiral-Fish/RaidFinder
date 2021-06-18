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

#ifndef STATE_HPP
#define STATE_HPP

#include <Core/Util/Global.hpp>

class State
{

public:
    State() = default;

    explicit State(u64 seed, u32 advances) : seed(seed), advances(advances), ivs { 255, 255, 255, 255, 255, 255 }
    {
    }

    u64 getSeed() const
    {
        return seed;
    }

    u32 getAdvances() const
    {
        return advances;
    }

    u32 getEC() const
    {
        return ec;
    }

    void setEC(u32 ec)
    {
        this->ec = ec;
    }

    u32 getPID() const
    {
        return pid;
    }

    void setPID(u32 pid)
    {
        this->pid = pid;
    }

    u8 getNature() const
    {
        return nature;
    }

    void setNature(u8 nature)
    {
        this->nature = nature;
    }

    u8 getAbility() const
    {
        return ability;
    }

    void setAbility(u8 ability)
    {
        this->ability = ability;
    }

    u8 getGender() const
    {
        return gender;
    }

    void setGender(u8 gender)
    {
        this->gender = gender;
    }

    u8 getShiny() const
    {
        return shiny;
    }

    void setShiny(u8 shiny)
    {
        this->shiny = shiny;
    }
    
    u8 getIV(int index) const
    {
        return ivs[index];
    }

    void setIV(int index, u8 iv)
    {
        ivs[index] = iv;
    }

    u8 getCharacteristic() const
    {
        constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };

        u8 charStat = ec % 6;
        for (u8 i = 0; i < 6; i++)
        {
            u8 stat = (charStat + i) % 6;
            if (ivs[order[stat]] == 31)
            {
                return stat;
            }
        }
        return charStat;
    }

private:
    u64 seed;
    u32 advances;
    u32 ec;
    u32 pid;
    u8 nature;
    u8 ability;
    u8 gender;
    u8 shiny;
    u8 ivs[6];
};

#endif // STATE_HPP
