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

#include "FrameCompare.hpp"

FrameCompare::FrameCompare(u8 gender, u8 ability, u8 shiny, bool skip, const QVector<u8> &min, const QVector<u8> &max,
    const QVector<bool> &natures)
    : min(min)
    , max(max)
    , gender(gender)
    , ability(ability)
    , natures(natures)
    , shiny(shiny)
    , skip(skip)
{
}

bool FrameCompare::compareFrame(const Frame &frame) const
{
    if (skip)
    {
        return true;
    }

    if (!natures.at(frame.getNature()))
    {
        return false;
    }

    if (gender != 255 && gender != frame.getGender())
    {
        return false;
    }

    if (ability != 255 && ability != frame.getAbility())
    {
        return false;
    }

    if (((shiny == 1 || shiny == 2) && shiny != frame.getShiny()) || (shiny == 3 && !frame.getShiny()))
    {
        return false;
    }

    for (u8 i = 0; i < 6; i++)
    {
        u8 iv = frame.getIV(i);
        if (iv > max.at(i) || iv < min.at(i))
        {
            return false;
        }
    }

    return true;
}
