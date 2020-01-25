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

#include "SeedSearcher.hpp"
#include <Core/RNG/XoroShiro.hpp>

constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };

SeedSearcher::SeedSearcher(const QVector<Pokemon> &pokemon, const QVector<int> &ivCount, bool firstResult) :
    pokemon(pokemon),
    ivCount(ivCount),
    firstResult(firstResult),
    searching(false)
{
}

void SeedSearcher::cancelSearch()
{
    searching = false;
}

QVector<u64> SeedSearcher::getResults() const
{
    return results;
}

void SeedSearcher::search(u64 min, u64 max)
{
    for (u64 search = min; search <= max && searching; search++)
    {
        u64 seed = search;
        if (searchSeed(seed))
        {
            std::lock_guard<std::mutex> lock(mutex);

            results.append(seed);
            if (firstResult)
            {
                searching = false;
            }
        }
        emit singleSearchFinished();
    }
}

u8 SeedSearcher::checkCharacteristic(u8 characteristic, u8 index) const
{
    for (u8 j = 0; j < 6; j++)
    {
        u8 stat = (characteristic + j) % 6;

        if (pokemon.at(index).getIV(order[stat]) == 31)
        {
            return stat;
        }
    }

    return characteristic;
}
