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

#include "SeedSearcher3.hpp"
#include <Core/RNG/XoroShiro.hpp>
#include <QtConcurrent>

SeedSearcher3::SeedSearcher3(const QVector<Pokemon> &pokemon, const QVector<int> &ivCount, const QVector<u8> &templateIVs,
                             bool firstResult) :
    SeedSearcher(pokemon, ivCount, firstResult)
{
    this->templateIVs = templateIVs;
    max = 0x1FFFFFF;
}

void SeedSearcher3::startSearch(int minRolls, int maxRolls, int threads)
{
    pool.setMaxThreadCount(threads);
    searching = true;

    progressOffset = maxRolls - minRolls + 1;
    u32 split = max / threads;

    for (int i = minRolls; i <= maxRolls && searching; i++)
    {
        matrix.prepareSix(i, 5);
        ivOffset = i;

        QVector<QFuture<void>> threadContainer;
        u32 min = 0;
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

bool SeedSearcher3::searchSeed(u64 &seed)
{
    u64 target = (seed & 0x1F00000ul) << 25;
    target |= (seed & 0xF8000ul) << 20;
    target |= (seed & 0x7C00ul) << 15;
    target |= (seed & 0x3E0ul) << 10;
    target |= (seed & 0x1Ful) << 5;

    target |= ((32ul + templateIVs.at(0) - ((seed & 0x1F00000ul) >> 20)) & 0x1F) << 40;
    target |= ((32ul + templateIVs.at(1) - ((seed & 0xF8000ul) >> 15)) & 0x1F) << 30;
    target |= ((32ul + templateIVs.at(2) - ((seed & 0x7C00ul) >> 10)) & 0x1F) << 20;
    target |= ((32ul + templateIVs.at(3) - ((seed & 0x3E0ul) >> 5)) & 0x1F) << 10;
    target |= ((32ul + templateIVs.at(4) - (seed & 0x1Ful)) & 0x1F);

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

    for (u64 search = 0; search <= 16383; search++)
    {
        u64 searchSeed = (processedTarget ^ matrix.getCoefficientData(search)) ^ matrix.getSearchPattern(search);

        if (compareFirst(searchSeed) && compareRest(searchSeed))
        {
            seed = searchSeed - (0x82a2b175229d6a5b * 2);
            return true;
        }
    }

    return false;
}
