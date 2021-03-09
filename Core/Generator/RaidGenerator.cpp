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

#include "RaidGenerator.hpp"
#include <Core/RNG/XoroShiro.hpp>

constexpr u8 toxtricityAmpedNatures[13] = { 3, 4, 2, 8, 9, 19, 22, 11, 13, 14, 0, 6, 24 };
constexpr u8 toxtricityLowKeyNatures[12] = { 1, 5, 7, 10, 12, 15, 16, 17, 18, 20, 21, 23 };

RaidGenerator::RaidGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const Raid &raid) :
    initialAdvances(initialAdvances),
    maxAdvances(maxAdvances),
    tid(tid),
    sid(sid),
    species(raid.getSpecies()),
    altform(raid.getAltForm()),
    abilityType(raid.getAbility()),
    shinyType(raid.getShiny()),
    ivCount(raid.getIVCount()),
    genderType(raid.getGender()),
    genderRatio(raid.getGenderRatio())
{
}

std::vector<State> RaidGenerator::generate(const StateFilter &filter, u64 seed) const
{
    std::vector<State> states;
    u16 tsv = (tid ^ sid) >> 4;

    seed += 0x82A2B175229D6A5B * initialAdvances;

    for (u32 advance = 0; advance <= maxAdvances; advance++, seed += 0x82A2B175229D6A5B)
    {
        XoroShiro rng(seed);
        State result(seed, initialAdvances + advance);

        u32 ec = rng.nextInt<0xffffffff>();
        result.setEC(ec);

        u32 sidtid = rng.nextInt<0xffffffff>();
        u32 pid = rng.nextInt<0xffffffff>();

        if (shinyType == 0) // Random shiny chance
        {
            // Game uses a fake TID/SID to determine shiny or not
            // PID is later modified using the actual TID/SID of trainer if necessary
            u16 shinyXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ (pid >> 16) ^ (pid & 0xffff);
            u16 psv = ((pid >> 16) ^ (pid & 0xffff)) >> 4;
            u8 PIDShinyType = (pid >> 16) ^ (pid & 0xffff) ^ tid ^ sid;

            if (shinyXor < 16) // Force shiny
            {
                u8 shinyType = shinyXor == 0 ? 2 : 1;
                result.setShiny(shinyType);
                if (shinyXor != PIDShinyType)
                {
                    u16 high = (pid & 0xFFFF) ^ tid ^ sid ^ (2 - shinyType);
                    pid = (high << 16) | (pid & 0xFFFF);
                }
            }
            else // Force non shiny
            {
                result.setShiny(0);
                if (psv == tsv)
                {
                    pid ^= 0x10000000;
                }
            }
        }
        else if (shinyType == 1) // Force non-shiny
        {
            result.setShiny(0);
            u16 psv = ((pid >> 16) ^ (pid & 0xffff)) >> 4;
            if (psv == tsv)
            {
                pid ^= 0x10000000;
            }
        }
        else // Force shiny
        {
            result.setShiny(2);
            u8 PIDShinyType = (pid >> 16) ^ (pid & 0xffff) ^ tid ^ sid;
            if (PIDShinyType >= 16 || PIDShinyType) // Check if PID is not normally shiny
            {
                // Force shiny (makes it square)
                u16 high = (pid & 0xffff) ^ tid ^ sid;
                pid = (high << 16) | (pid & 0xffff);
            }
        }
        result.setPID(pid);

        // Early shiny filter reduces further computation
        if (!filter.compareShiny(result))
        {
            continue;
        }

        // Set IVs that will be 31s
        for (u8 i = 0; i < ivCount;)
        {
            u8 index = rng.nextInt<6>();
            if (result.getIV(index) == 255)
            {
                result.setIV(index, 31);
                i++;
            }
        }

        // Fill rest of IVs with rand calls
        for (u8 i = 0; i < 6; i++)
        {
            if (result.getIV(i) == 255)
            {
                result.setIV(i, rng.nextInt<32>());
            }
        }

        if (abilityType == 4) // Allow hidden ability
        {
            result.setAbility(rng.nextInt<3>());
        }
        else if (abilityType == 3) // No hidden ability
        {
            result.setAbility(rng.nextInt<2>());
        }
        else // Locked ability
        {
            result.setAbility(abilityType);
        }

        // Altform, doesn't seem to have a rand call for raids

        if (genderType == 0) // Random
        {
            if (genderRatio == 255) // Locked genderless
            {
                result.setGender(2);
            }
            else if (genderRatio == 254) // Locked female
            {
                result.setGender(1);
            }
            else if (genderRatio == 0) // Locked male
            {
                result.setGender(0);
            }
            else // Random
            {
                result.setGender((rng.nextInt<253>() + 1) < genderRatio);
            }
        }
        else if (genderType == 1) // Male
        {
            result.setGender(0);
        }
        else if (genderType == 2) // Female
        {
            result.setGender(1);
        }
        else if (genderType == 3) // Genderless
        {
            result.setGender(2);
        }

        if (species != 849)
        {
            result.setNature(rng.nextInt<25>());
        }
        else
        {
            if (altform == 0)
            {
                result.setNature(toxtricityAmpedNatures[rng.nextInt<13>()]);
            }
            else
            {
                result.setNature(toxtricityLowKeyNatures[rng.nextInt<12>()]);
            }
        }

        // Height (2 calls)
        // Weight (2 calls)

        if (filter.compareState(result))
        {
            states.emplace_back(result);
        }
    }

    return states;
}
