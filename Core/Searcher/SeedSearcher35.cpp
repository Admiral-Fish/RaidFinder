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

#include "SeedSearcher35.hpp"
#include <Core/RNG/XoroShiro.hpp>
#include <QtConcurrent>

constexpr u8 toxtricityAmpedNatures[13] = { 3, 4, 2, 8, 9, 19, 22, 11, 13, 14, 0, 6, 24 };
constexpr u8 toxtricityLowKeyNatures[12] = { 1, 5, 7, 10, 12, 15, 16, 17, 18, 20, 21, 23 };

SeedSearcher35::SeedSearcher35(const QVector<Pokemon> &pokemon, const QVector<int> &ivCount, bool firstResult) :
    SeedSearcher(pokemon, ivCount, firstResult),
    ivFlag(ivCount.at(0) == 3),
    fixedIVs(ivCount.at(0) == 2 ? 6 : 5)
{
    max = ivFlag ? 0x1FFFFFF : 0x3FFFFFFF;
}

void SeedSearcher35::setIVs(const QVector<u8> &templateIVs)
{
    this->templateIVs = templateIVs;
}

void SeedSearcher35::startSearch(int minRolls, int maxRolls, int threads)
{
    pool.setMaxThreadCount(threads);
    searching = true;

    progressOffset = maxRolls - minRolls + 1;
    u32 split = max / threads;

    for (int i = minRolls; i <= maxRolls && searching; i++)
    {
        matrix.prepareSix(i, fixedIVs);
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
            threadContainer[i].waitForFinished();
        }
    }
}

bool SeedSearcher35::searchSeed(u64 &seed)
{
    int length = fixedIVs * 10;

    u64 target = 0;

    if (fixedIVs == 6)
    {
        target |= (seed & 0x3E000000ul) << 30;
        target |= (seed & 0x1F00000ul) << 25;
        target |= (seed & 0xF8000ul) << 20;
        target |= (seed & 0x7C00ul) << 15;
        target |= (seed & 0x3E0ul) << 10;
        target |= (seed & 0x1Ful) << 5;

        target |= ((32ul + templateIVs.at(0) - ((seed & 0x3E000000ul) >> 25)) & 0x1F) << 50;
        target |= ((32ul + templateIVs.at(1) - ((seed & 0x1F00000ul) >> 20)) & 0x1F) << 40;
        target |= ((32ul + templateIVs.at(2) - ((seed & 0xF8000ul) >> 15)) & 0x1F) << 30;
        target |= ((32ul + templateIVs.at(3) - ((seed & 0x7C00ul) >> 10)) & 0x1F) << 20;
        target |= ((32ul + templateIVs.at(4) - ((seed & 0x3E0ul) >> 5)) & 0x1F) << 10;
        target |= ((32ul + templateIVs.at(5) - (seed & 0x1Ful)) & 0x1F);
    }
    else if (fixedIVs == 5)
    {
        target |= (seed & 0x1F00000ul) << 25;
        target |= (seed & 0xF8000ul) << 20;
        target |= (seed & 0x7C00ul) << 15;
        target |= (seed & 0x3E0ul) << 10;
        target |= (seed & 0x1Ful) << 5;

        target |= ((32ul + templateIVs.at(0) - ((seed & 0x1F00000ul) >> 20)) & 0x1F) << 40;
        target |= ((32ul + templateIVs.at(1) - ((seed & 0xF8000ul) >> 15)) & 0x1F) << 30;
        target |= ((32ul + templateIVs.at(2) - ((seed & 0x7C00ul) >> 10)) & 0x1F) << 20;
        target |= ((32ul + templateIVs.at(3) - ((seed & 0x3E0ul) >> 5)) & 0x1F) << 10;
        target |= ((32ul + templateIVs.at(4) - (seed & 0x1Ful)) & 0x1F);
    }

    target ^= matrix.getConstantTermVector();

    u64 processedTarget = 0;
    for (int i = 0; i < 64; i++)
    {
        if (matrix.getAnswerFlag(i) != 0)
        {
            processedTarget |= matrix.getModifiedAnswerFlag(i, target) << (63 - i);
        }
    }

    XoroShiro rng;
    u64 max = ((1 << (64 - length)) - 1);
    for (u64 search = 0; search <= max; search++)
    {
        u64 searchSeed = (processedTarget ^ matrix.getCoefficientData(search)) ^ matrix.getSearchPattern(search);
        rng.setSeed(searchSeed);

        {
            u32 ec = rng.nextInt(0xffffffff, 0xffffffff);
            u8 characteristic = ec % 6;

            if (!characteristicFlags.at(0).at(characteristic) || !characteristicFlags.at(1).at(characteristic))
            {
                continue;
            }
        }

        rng.nextInt(0xffffffff, 0xffffffff); // SIDTID
        rng.nextInt(0xffffffff, 0xffffffff); // PID

        {
            u8 ivs[6] = { 255, 255, 255, 255, 255, 255 };
            int count = 0;
            int offset = -(8 - fixedIVs);

            do
            {
                u8 stat;
                do
                {
                    stat = static_cast<u8>(rng.nextInt(7));
                    offset++;
                } while (stat >= 6);

                if (ivs[stat] == 255)
                {
                    ivs[stat] = 31;
                    count++;
                }
            } while (count < (8 - fixedIVs));

            if (offset != ivOffset)
            {
                continue;
            }

            bool flag = true;
            for (u8 i = 0; i < 6; i++)
            {
                if (ivs[i] != 31)
                {
                    ivs[i] = static_cast<u8>(rng.nextInt(31));
                }

                if (ivs[i] != pokemon.at(0).getIV(i))
                {
                    flag = false;
                    break;
                }
            }

            if (!flag)
            {
                continue;
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
                continue;
            }

            if (!pokemon.at(0).getGenderLocked())
            {
                rng.nextInt(253, 255);
            }

            u8 nature;
            if (pokemon.at(0).getSpecies() != 849)
            {
                nature = static_cast<u8>(rng.nextInt(25, 31));
            }
            else
            {
                if (pokemon.at(0).getAltform() == 0)
                {
                    nature = toxtricityAmpedNatures[rng.nextInt(13, 15)];
                }
                else
                {
                    nature = toxtricityLowKeyNatures[rng.nextInt(12, 15)];
                }
            }

            if (nature != pokemon.at(0).getNature())
            {
                continue;
            }
        }

        bool flag = true;
        for (u8 i = 1; i < pokemon.size(); i++, searchSeed += 0x82a2b175229d6a5bull)
        {
            rng.setSeed(searchSeed);

            u32 ec = rng.nextInt(0xffffffff, 0xffffffff);

            if (i != 1)
            {
                if (!characteristicFlags.at(i).at(ec % 6))
                {
                    flag = false;
                    break;
                }
            }

            rng.nextInt(0xffffffff, 0xffffffff); // SIDTID
            rng.nextInt(0xffffffff, 0xffffffff); // PID

            u8 ivs[6] = { 255, 255, 255, 255, 255, 255 };
            int count = 0;
            do
            {
                u8 stat = static_cast<u8>(rng.nextInt(6, 7));

                if (ivs[stat] == 255)
                {
                    ivs[stat] = 31;
                    count++;
                }
            } while (count < ivCount[i]);

            for (u8 j = 0; j < 6; j++)
            {
                if (ivs[j] != 31)
                {
                    ivs[j] = static_cast<u8>(rng.nextInt(31));
                }

                if (ivs[j] != pokemon.at(i).getIV(j))
                {
                    flag = false;
                    break;
                }
            }
            if (!flag)
            {
                break;
            }

            u8 ability;
            if (pokemon.at(i).getAllowHiddenAbility())
            {
                ability = static_cast<u8>(rng.nextInt(3, 3));
            }
            else
            {
                ability = static_cast<u8>(rng.nextInt(1));
            }
            if ((pokemon.at(i).getAbility() != ability) || (pokemon.at(i).getAbility() == 255 && ability >= 2))
            {
                flag = false;
                break;
            }

            if (!pokemon.at(i).getGenderLocked())
            {
                rng.nextInt(253, 255);
            }

            u8 nature;
            if (pokemon.at(i).getSpecies() != 849)
            {
                nature = static_cast<u8>(rng.nextInt(25, 31));
            }
            else
            {
                if (pokemon.at(i).getAltform() == 0)
                {
                    nature = toxtricityAmpedNatures[rng.nextInt(13, 15)];
                }
                else
                {
                    nature = toxtricityLowKeyNatures[rng.nextInt(12, 15)];
                }
            }

            if (nature != pokemon.at(i).getNature())
            {
                flag = false;
                break;
            }
        }

        if (flag)
        {
            seed = searchSeed - (0x82a2b175229d6a5b * static_cast<u8>((pokemon.size() + 2)));
            return true;
        }
    }

    return false;
}
