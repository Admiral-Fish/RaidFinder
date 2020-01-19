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

#include "FrameFilter.hpp"

FrameFilter::FrameFilter(u8 gender, u8 ability, u8 shiny, bool skip, const QVector<u8> &min, const QVector<u8> &max,
                           const QVector<bool> &natures) :
    min(min), max(max), gender(gender), ability(ability), natures(natures), shiny(shiny), skip(skip)
{
}

bool FrameFilter::compareShiny(const Frame &frame) const
{
    return skip || shiny == 255 || (shiny & frame.getShiny());
}

bool FrameFilter::compareIVs(const Frame &frame) const
{
    if (skip)
    {
        return true;
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

bool FrameFilter::compareAbility(const Frame &frame) const
{
    return skip || ability == 255 || ability == frame.getAbility();
}

bool FrameFilter::compareGender(const Frame &frame) const
{
    return skip || gender == 255 || gender == frame.getGender();
}

bool FrameFilter::compareNature(const Frame &frame) const
{
    return skip || natures.at(frame.getNature());
}
