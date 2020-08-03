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
    explicit State(u64 seed, u32 advances);
    u64 getSeed() const;
    u32 getAdvances() const;
    u32 getEC() const;
    void setEC(u32 ec);
    u32 getPID() const;
    void setPID(u32 pid);
    u8 getNature() const;
    void setNature(u8 nature);
    u8 getAbility() const;
    void setAbility(u8 ability);
    u8 getGender() const;
    void setGender(u8 gender);
    u8 getShiny() const;
    void setShiny(u8 shiny);
    u8 getIV(int index) const;
    void setIV(int index, u8 iv);
    u8 getCharacteristic() const;

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
