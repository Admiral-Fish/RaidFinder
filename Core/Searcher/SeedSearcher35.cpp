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
#include <cstring>

constexpr u8 toxtricityAmpedNatures[13] = { 3, 4, 2, 8, 9, 19, 22, 11, 13, 14, 0, 6, 24 };

SeedSearcher35::SeedSearcher35(const QVector<Pokemon> &pokemon, const QVector<int> &ivCount, bool firstResult) :
    ivFlag(ivCount.at(0) == 3),
    fixedIVs(ivCount.at(0) == 2 ? 6 : 5),
    ivCount(ivCount),
    firstResult(firstResult),
    pokemon(pokemon)
{
}

void SeedSearcher35::setIVs(const QVector<u8> &templateIVs)
{
    this->templateIVs = templateIVs;
}

void SeedSearcher35::startSearch(int maxRolls, int threads)
{
    QThreadPool::globalInstance()->setMaxThreadCount(threads);
    for (int i = 0; i <= maxRolls; i++)
    {
        matrix.prepareSix(i, fixedIVs);

        u8 max = ivFlag ? 1 : 32;
        for (u8 j = 0; j < max; j++)
        {
            QVector<u64> seeds;
            seeds.reserve(0x2000000);
            for (u32 k = 0x2000000u * j; k < 0x2000000u * (j + 1); k++)
            {
                seeds.append(k);
            }

            QtConcurrent::blockingMap(seeds, [this](u64 &seed) { search(seed); });

            for (u64 seed : seeds)
            {
                if (seed)
                {
                    results.append(seed - (0x82a2b175229d6a5b * static_cast<u8>(pokemon.size() + 2)));
                }
            }

            if (firstResult && !results.isEmpty())
            {
                return;
            }
        }
    }
}

QVector<u64> SeedSearcher35::getResults() const
{
    return results;
}

void SeedSearcher35::search(u64 &seed)
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
    int offsetTarget = 0;
    for (int i = 0; i < length; i++)
    {
        while (matrix.getFreeBit(i + offsetTarget) > 0)
        {
            offsetTarget++;
        }
        processedTarget |= (matrix.getModifiedAnswerFlag(i, target) << (63 - (i + offsetTarget)));
    }

    XoroShiro rng;
    u64 max = ((1 << (64 - length)) - 1);
    for (u64 search = 0; search <= max; search++)
    {
        u64 searchSeed = (processedTarget ^ matrix.getCoefficientData(search)) ^ matrix.getSearchPattern(search);

        rng.setSeed(searchSeed);

        u32 ec = rng.nextInt(0xffffffff, 0xffffffff);

        {
            u8 characteristic = ec % 6;
            for (u8 i = 0; i < 6; i++)
            {
                characteristic = (characteristic + i) % 6;
                if (pokemon[0].checkCharacteristic(characteristic))
                {
                    break;
                }
            }

            if (characteristic != pokemon[0].getCharacteristic())
            {
                continue;
            }
        }

        {
            u8 characteristic = ec % 6;
            for (u8 i = 0; i < 6; i++)
            {
                characteristic = (characteristic + i) % 6;
                if (pokemon[1].checkCharacteristic(characteristic))
                {
                    break;
                }
            }

            if (characteristic != pokemon[1].getCharacteristic())
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

                if (ivs[i] != pokemon[0].getIV(i))
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
            if (pokemon[0].getAllowHiddenAbility())
            {
                ability = static_cast<u8>(rng.nextInt(3, 3));
            }
            else
            {
                ability = static_cast<u8>(rng.nextInt(1));
            }
            if (pokemon[0].getAbility() != 255 && pokemon[0].getAbility() != ability)
            {
                continue;
            }

            if (!pokemon[0].getGenderLocked())
            {
                rng.nextInt(253, 255);
            }

            u8 nature;
            if (pokemon[0].getSpecies() != 849)
            {
                nature = static_cast<u8>(rng.nextInt(25, 31));
            }
            else
            {
                nature = toxtricityAmpedNatures[rng.nextInt(13, 15)];
            }

            if (nature != pokemon[0].getNature())
            {
                continue;
            }
        }

        bool flag = true;
        for (u8 i = 1; i < pokemon.size(); i++, searchSeed += 0x82a2b175229d6a5bull)
        {
            rng.setSeed(searchSeed);

            ec = rng.nextInt(0xffffffff, 0xffffffff);
            rng.nextInt(0xffffffff, 0xffffffff); // SIDTID
            rng.nextInt(0xffffffff, 0xffffffff); // PID

            if (i > 1)
            {
                u8 characteristic = ec % 6;
                for (u8 j = 0; j < 6; j++)
                {
                    characteristic = (characteristic + j) % 6;
                    if (pokemon[i].checkCharacteristic(characteristic))
                    {
                        break;
                    }
                }

                if (characteristic != pokemon[i].getCharacteristic())
                {
                    flag = false;
                    break;
                }
            }

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

                if (ivs[j] != pokemon[i].getIV(j))
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
            if (pokemon[i].getAllowHiddenAbility())
            {
                ability = static_cast<u8>(rng.nextInt(3, 3));
            }
            else
            {
                ability = static_cast<u8>(rng.nextInt(1));
            }
            if ((pokemon[i].getAbility() != ability) || (pokemon[i].getAbility() == 255 && ability >= 2))
            {
                flag = false;
                break;
            }

            if (!pokemon[i].getGenderLocked())
            {
                rng.nextInt(253, 255);
            }

            u8 nature;
            if (pokemon[i].getSpecies() != 849)
            {
                nature = static_cast<u8>(rng.nextInt(25, 31));
            }
            else
            {
                nature = toxtricityAmpedNatures[rng.nextInt(13, 15)];
            }

            if (nature != pokemon[i].getNature())
            {
                flag = false;
                break;
            }
        }

        if (flag)
        {
            seed = searchSeed;
            return;
        }
    }

    seed = 0;
}
