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

#ifndef FRAME_HPP
#define FRAME_HPP

#include <Core/Global.hpp>

class Frame
{
public:
    Frame() = default;
    Frame(u32 frame, u16 tsv);
    u32 getFrame() const;
    void setFrame(u32 frame);
    u32 getEC() const;
    void setEC(u32 ec);
    u32 getPID() const;
    void setPID(u32 pid);
    u16 getTSV() const;
    void setTSV(u16 tsv);
    u8 getNature() const;
    void setNature(u8 nature);
    u8 getAbility() const;
    void setAbility(u8 ability);
    u8 getGender() const;
    void setGender(u8 gender);
    bool getShiny() const;
    void setShiny(bool shiny);
    u8 getIV(u8 index) const;
    void setIV(u8 index, u8 iv);

private:
    u32 frame;
    u32 ec;
    u32 pid;
    u16 tsv;
    u8 nature;
    u8 ability;
    u8 gender;
    bool shiny;
    u8 ivs[6];
};

#endif // FRAME_HPP
