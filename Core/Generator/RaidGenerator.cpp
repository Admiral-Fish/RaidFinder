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

static inline u16 getSv(u32 val)
{
    return ((val >> 16) ^ (val & 0xFFFF)) >> 4;
}

static inline u8 getShinyType(u32 sidtid, u32 pid)
{
    u16 val = (sidtid ^ pid) >> 16;
    if ((val ^ (sidtid & 0xffff)) == (pid & 0xffff))
    {
        return 2; // Square shiny
    }

    return 1; // Star shiny
}

RaidGenerator::RaidGenerator(u32 startFrame, u32 maxResults, u16 tid, u16 sid, const Raid &raid) :
    startFrame(startFrame),
    maxResults(maxResults),
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

QVector<Frame> RaidGenerator::generate(const FrameFilter &filter, u64 seed) const
{
    QVector<Frame> frames;
    u16 tsv = (tid ^ sid) >> 4;

    for (u32 i = 1; i < startFrame; i++)
    {
        seed += 0x82A2B175229D6A5B;
    }

    for (u32 frame = 0; frame < maxResults; frame++, seed += 0x82A2B175229D6A5B)
    {
        XoroShiro rng(seed);
        Frame result(seed, startFrame + frame);

        u32 ec = rng.nextInt(0xffffffff, 0xffffffff);
        result.setEC(ec);

        u32 sidtid = rng.nextInt(0xffffffff, 0xffffffff);
        u32 pid = rng.nextInt(0xffffffff, 0xffffffff);

        if (shinyType == 0) // Random shiny chance
        {
            // Game uses a fake TID/SID to determine shiny or not
            // PID is later modified using the actual TID/SID of trainer if necessary
            u16 ftsv = getSv(sidtid);
            u16 psv = getSv(pid);

            if (ftsv == psv) // Force shiny
            {
                u8 type = getShinyType(sidtid, pid);
                result.setShiny(type);
                if (psv != tsv)
                {
                    u16 high = (pid & 0xFFFF) ^ tid ^ sid ^ (2 - type);
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
            u16 psv = getSv(pid);
            if (psv == tsv)
            {
                pid ^= 0x10000000;
            }
        }
        else // Force shiny
        {
            result.setShiny(2);
            if (((pid >> 16) ^ (pid & 0xffff) ^ tid ^ sid) != 0) // Check if PID is not normally square shiny
            {
                u16 high = (pid & 0xffff) ^ tid ^ sid;
                pid = (high << 16) | (pid & 0xffff);
            }
        }
        result.setPID(pid);

        if (!filter.compareShiny(result))
        {
            continue;
        }

        // Set IVs that will be 31s
        for (int i = 0; i < ivCount;)
        {
            u8 index = static_cast<u8>(rng.nextInt(6, 7));
            if (result.getIV(index) == 255)
            {
                result.setIV(index, 31);
                i++;
            }
        }

        // Fill rest of IVs with rand calls
        for (int i = 0; i < 6; i++)
        {
            if (result.getIV(i) == 255)
            {
                result.setIV(i, static_cast<u8>(rng.nextInt(31)));
            }
        }

        if (abilityType == 4) // Allow hidden ability
        {
            result.setAbility(static_cast<u8>(rng.nextInt(3, 3)));
        }
        else if (abilityType == 3) // No hidden ability
        {
            result.setAbility(static_cast<u8>(rng.nextInt(1)));
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
                result.setGender(static_cast<u8>(rng.nextInt(253, 255) + 1) < genderRatio);
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
            result.setNature(static_cast<u8>(rng.nextInt(25, 31)));
        }
        else
        {
            if (altform == 0)
            {
                result.setNature(toxtricityAmpedNatures[rng.nextInt(13, 15)]);
            }
            else
            {
                result.setNature(toxtricityLowKeyNatures[rng.nextInt(12, 15)]);
            }
        }

        // Height (2 calls)
        // Weight (2 calls)

        if (filter.compareFrame(result))
        {
            frames.append(result);
        }
    }

    return frames;
}
