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

#include "Frame.hpp"

Frame::Frame(u64 seed, u32 frame) :
    seed(seed), frame(frame), ivs { 255, 255, 255, 255, 255, 255 }
{
}

u64 Frame::getSeed() const
{
    return seed;
}

u32 Frame::getFrame() const
{
    return frame;
}

u32 Frame::getEC() const
{
    return ec;
}

void Frame::setEC(u32 ec)
{
    this->ec = ec;
}

u32 Frame::getPID() const
{
    return pid;
}

void Frame::setPID(u32 pid)
{
    this->pid = pid;
}

u8 Frame::getNature() const
{
    return nature;
}

void Frame::setNature(u8 nature)
{
    this->nature = nature;
}

u8 Frame::getAbility() const
{
    return ability;
}

void Frame::setAbility(u8 ability)
{
    this->ability = ability;
}

u8 Frame::getGender() const
{
    return gender;
}

void Frame::setGender(u8 gender)
{
    this->gender = gender;
}

u8 Frame::getShiny() const
{
    return shiny;
}

void Frame::setShiny(u8 shiny)
{
    this->shiny = shiny;
}

u8 Frame::getIV(u8 index) const
{
    return ivs[index];
}

void Frame::setIV(u8 index, u8 iv)
{
    ivs[index] = iv;
}
