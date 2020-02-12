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

#include "SeedSearcher1.hpp"
#include <Core/RNG/XoroShiro.hpp>
#include <QtConcurrent>

SeedSearcher1::SeedSearcher1(const QVector<Pokemon> &pokemon, const QVector<int> &ivCount, bool firstResult) :
    SeedSearcher(pokemon, ivCount, firstResult)
{
    const auto &mon = pokemon.at(0);
    templateIVs = { mon.getIV(1), mon.getIV(2), mon.getIV(3), mon.getIV(4), mon.getIV(5), mon.getIV(0), mon.getIV(2), mon.getIV(3),
                    mon.getIV(4), mon.getIV(5), mon.getIV(0), mon.getIV(1), mon.getIV(3), mon.getIV(4), mon.getIV(5), mon.getIV(0),
                    mon.getIV(1), mon.getIV(2), mon.getIV(4), mon.getIV(5), mon.getIV(0), mon.getIV(1), mon.getIV(2), mon.getIV(3),
                    mon.getIV(5), mon.getIV(0), mon.getIV(1), mon.getIV(2), mon.getIV(3), mon.getIV(4) };

    for (u8 i = 0; i < 6; i++)
    {
        if (mon.getIV(i) == 31)
        {
            fixedIndex = i;
        }
    }

    ability = !mon.getAllowHiddenAbility() && (mon.getAbility() == 0 || mon.getAbility() == 1);
    max = 0xfffffff;
}

void SeedSearcher1::startSearch(int minRolls, int maxRolls, int threads)
{
    pool.setMaxThreadCount(threads);
    searching = true;

    progressOffset = maxRolls - minRolls + 1;
    u32 split = max / threads;

    for (int i = minRolls; i <= maxRolls && searching; i++)
    {
        matrix.prepare(ability, i);
        ivOffset = i;

        u32 min = 0;
        QVector<QFuture<void>> threadContainer;
        for (int j = 0; j < threads; j++)
        {
            if (j == threads - 1)
            {
                threadContainer.append(QtConcurrent::run(&pool, [=] { search(min, max); }));
            }
            else
            {
                threadContainer.append(QtConcurrent::run(&pool, [=] { search(min, min + split); }));
            }

            min += split;
        }

        for (int j = 0; j < threads; j++)
        {
            threadContainer[j].waitForFinished();
        }
    }
}

bool SeedSearcher1::searchSeed(u64 &seed)
{
    int length = ability ? 57 : 56;

    u64 target = ability ? (pokemon.at(0).getAbility() & 1) : 0;
    int bitOffset = ability ? 1 : 0;

    target |= (seed & 0xE000000ul) << (28 + bitOffset);

    target |= (seed & 0x1F00000ul) << (25 + bitOffset);
    target |= (seed & 0xF8000ul) << (20 + bitOffset);
    target |= (seed & 0x7C00ul) << (15 + bitOffset);
    target |= (seed & 0x3E0ul) << (10 + bitOffset);
    target |= (seed & 0x1Ful) << (5 + bitOffset);

    target |= ((8ul + fixedIndex - ((seed & 0xE000000ul) >> 25)) & 7) << (50 + bitOffset);

    target |= ((32ul + templateIVs.at(fixedIndex * 5) - ((seed & 0x1F00000ul) >> 20)) & 0x1F) << (40 + bitOffset);
    target |= ((32ul + templateIVs.at(fixedIndex * 5 + 1) - ((seed & 0xF8000ul) >> 15)) & 0x1F) << (30 + bitOffset);
    target |= ((32ul + templateIVs.at(fixedIndex * 5 + 2) - ((seed & 0x7C00ul) >> 10)) & 0x1F) << (20 + bitOffset);
    target |= ((32ul + templateIVs.at(fixedIndex * 5 + 3) - ((seed & 0x3E0ul) >> 5)) & 0x1F) << (10 + bitOffset);
    target |= ((32ul + templateIVs.at(fixedIndex * 5 + 4) - (seed & 0x1Ful)) & 0x1F) << bitOffset;

    target ^= matrix.getConstantTermVector();

    u64 processedTarget = 0;
    for (int i = 0, offset = 0; i < 64; i++)
    {
        while (matrix.getFreeBit(i + offset))
        {
            offset++;
        }
        processedTarget |= matrix.getModifiedAnswerFlag(i, target) << (63 - (i + offset));
    }

    u64 max = ((1ull << (64 - length)) - 1);
    for (u64 search = 0; search <= max; search++)
    {
        u64 searchSeed = (processedTarget ^ matrix.getCoefficientData(search)) | matrix.getSearchPattern(search);

        if (compareFirst(searchSeed) && compareRest(searchSeed))
        {
            seed = searchSeed;
            return true;
        }
    }

    return false;
}

bool SeedSearcher1::compareFirst(u64 seed) const
{
    XoroShiro rng(seed);

    u32 ec = rng.nextInt(0xffffffff, 0xffffffff);
    if (!characteristicFlags.at(0).at(ec % 6))
    {
        return false;
    }

    rng.nextInt(0xffffffff, 0xffffffff); // SIDTID
    rng.nextInt(0xffffffff, 0xffffffff); // PID

    int offset = -1;
    rng.nextInt(6, 7, offset);

    if (offset != ivOffset)
    {
        return false;
    }

    for (int i = 0; i < 5; i++)
    {
        rng.nextInt(31);
    }

    u8 ability;
    if (pokemon.at(0).getAllowHiddenAbility())
    {
        ability = static_cast<u8>(rng.nextInt(3, 3));
    }
    else
    {
        ability = static_cast<u8>(rng.nextInt(1));
    }
    if (pokemon.at(0).getAbility() != 255 && pokemon.at(0).getAbility() != ability)
    {
        return false;
    }

    if (!pokemon.at(0).getGenderLocked())
    {
        rng.nextInt(253, 255);
    }

    u8 nature = static_cast<u8>(rng.nextInt(25, 31));
    if (nature != pokemon.at(0).getNature())
    {
        return false;
    }

    return true;
}
