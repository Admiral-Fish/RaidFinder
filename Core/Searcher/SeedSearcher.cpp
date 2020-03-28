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

constexpr u8 toxtricityAmpedNatures[13] = { 3, 4, 2, 8, 9, 19, 22, 11, 13, 14, 0, 6, 24 };
constexpr u8 toxtricityLowKeyNatures[12] = { 1, 5, 7, 10, 12, 15, 16, 17, 18, 20, 21, 23 };

SeedSearcher::SeedSearcher(const QVector<Pokemon> &pokemon, const QVector<int> &ivCount, bool firstResult) :
    pokemon(pokemon),
    ivCount(ivCount),
    firstResult(firstResult),
    searching(false),
    progress(0)
{
    for (int i = 0; i < pokemon.size(); i++)
    {
        const auto &mon = pokemon.at(i);

        characteristicFlags.append(QVector<bool>(6, false));
        characteristicFlags[i][mon.getCharacteristic()] = true;

        for (u8 j = 1; j < 6; j++)
        {
            u8 target = (mon.getCharacteristic() + 6 - j) % 6;
            if (!mon.isCharacterized(target))
            {
                characteristicFlags[i][target] = true;
            }
            else
            {
                break;
            }
        }
    }
}

void SeedSearcher::cancelSearch()
{
    searching = false;
}

QVector<u64> SeedSearcher::getResults() const
{
    return results;
}

int SeedSearcher::getProgress() const
{
    return static_cast<int>(progress / progressOffset);
}

int SeedSearcher::getMaxProgress() const
{
    return static_cast<int>(max);
}

void SeedSearcher::search(u32 min, u32 max)
{
    for (u64 search = min; search <= max && searching; search++)
    {
        u64 seed = search;
        if (searchSeed(seed))
        {
            std::lock_guard<std::mutex> lock(resultMutex);
            results.append(seed);

            if (firstResult)
            {
                searching = false;
            }
        }

        std::lock_guard<std::mutex> lock(progressMutex);
        progress++;
    }
}

bool SeedSearcher::compareFirst(u64 seed) const
{
    XoroShiro rng(seed);

    u32 ec = rng.nextInt(0xffffffff, 0xffffffff);
    u8 characteristic = ec % 6;

    if (!characteristicFlags.at(0).at(characteristic) || !characteristicFlags.at(1).at(characteristic))
    {
        return false;
    }

    rng.nextInt(0xffffffff, 0xffffffff); // SIDTID
    rng.nextInt(0xffffffff, 0xffffffff); // PID

    u8 ivs[6] = { 255, 255, 255, 255, 255, 255 };
    int offset = -ivCount.at(0);

    for (int count = 0; count < ivCount.at(0);)
    {
        u8 stat = static_cast<u8>(rng.nextInt(6, 7, offset));
        if (ivs[stat] == 255)
        {
            ivs[stat] = 31;
            count++;
        }
    }

    if (offset != ivOffset)
    {
        return false;
    }

    for (u8 i = 0; i < 6; i++)
    {
        if (ivs[i] != 31)
        {
            ivs[i] = static_cast<u8>(rng.nextInt(31));
        }

        if (ivs[i] != pokemon.at(0).getIV(i))
        {
            return false;
        }
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

    return nature == pokemon.at(0).getNature();
}

bool SeedSearcher::compareRest(u64 seed) const
{
    for (u8 i = 1; i < pokemon.size(); i++, seed += 0x82a2b175229d6a5bull)
    {
        XoroShiro rng(seed);

        u32 ec = rng.nextInt(0xffffffff, 0xffffffff);
        if (!characteristicFlags.at(i).at(ec % 6))
        {
            return false;
        }

        rng.nextInt(0xffffffff, 0xffffffff); // SIDTID
        rng.nextInt(0xffffffff, 0xffffffff); // PID

        u8 ivs[6] = { 255, 255, 255, 255, 255, 255 };
        for (int count = 0; count < ivCount.at(i);)
        {
            u8 stat = static_cast<u8>(rng.nextInt(6, 7));
            if (ivs[stat] == 255)
            {
                ivs[stat] = 31;
                count++;
            }
        }

        for (u8 j = 0; j < 6; j++)
        {
            if (ivs[j] != 31)
            {
                ivs[j] = static_cast<u8>(rng.nextInt(31));
            }

            if (ivs[j] != pokemon.at(i).getIV(j))
            {
                return false;
            }
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
        if (pokemon.at(i).getAbility() != 255 && pokemon.at(i).getAbility() != ability)
        {
            return false;
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
            return false;
        }
    }

    return true;
}
