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

#include "IVChecker.hpp"
#include <Core/Util/Nature.hpp>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <set>

namespace
{
    std::vector<std::vector<u8>> calculateIVs(const std::vector<u8> &baseStats, const std::vector<u16> &stats, u8 level, u8 nature)
    {
        std::vector<std::vector<u8>> ivs(6);

        for (u8 i = 0; i < 6; i++)
        {
            for (u8 iv = 0; iv < 32; iv++)
            {
                double stat;
                if (i == 0)
                {
                    stat = std::floor(((2 * baseStats.at(i) + iv) * level) / 100.0) + level + 10;
                }
                else
                {
                    stat = (std::floor(((2 * baseStats.at(i) + iv) * level) / 100.0) + 5) * Nature::getNatureModifier(nature, i);
                }

                if (static_cast<u16>(stat) == stats.at(i))
                {
                    ivs[i].emplace_back(iv);
                }
            }
        }

        return ivs;
    }
}

std::vector<std::vector<u8>> IVChecker::calculateIVRange(const std::vector<u8> &baseStats, const std::vector<std::vector<u16>> &stats,
                                                         const std::vector<u8> &level, u8 nature)
{
    std::vector<std::vector<u8>> ivs(6);
    for (size_t i = 0; i < stats.size(); i++)
    {
        auto current = calculateIVs(baseStats, stats[i], level[i], nature);

        if (i == 0)
        {
            ivs = current;
        }
        else
        {
            std::vector<std::vector<u8>> temp(6);
            for (size_t j = 0; j < 6; j++)
            {
                std::set_intersection(ivs[j].begin(), ivs[j].end(), current[j].begin(), current[j].end(), std::back_inserter(temp[j]));
            }
            ivs = temp;
        }
    }

    return ivs;
}
