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

#include "StateFilter.hpp"

StateFilter::StateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                         const std::vector<bool> &natures) :
    min(min), max(max), gender(gender), ability(ability), natures(natures), shiny(shiny), skip(skip)
{
}

bool StateFilter::compareState(const State &state) const
{
    if (skip)
    {
        return true;
    }

    if (gender != 255 && gender != state.getGender())
    {
        return false;
    }

    if (ability != 255 && ability != state.getAbility())
    {
        return false;
    }

    if (!natures[state.getNature()])
    {
        return false;
    }

    for (u8 i = 0; i < 6; i++)
    {
        u8 iv = state.getIV(i);
        if (iv > max[i] || iv < min[i])
        {
            return false;
        }
    }

    return true;
}

bool StateFilter::compareShiny(const State &state) const
{
    return skip || shiny == 255 || (shiny & state.getShiny());
}
