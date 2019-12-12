/*
 * This file is part of RaidFinder
 * Copyright (C) 2019 by Admiral_Fish
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
#include <Core/XoroShiro.hpp>

static inline u16 getSv(u32 val)
{
    return ((val >> 16) ^ (val & 0xFFFF)) >> 4;
}

RaidGenerator::RaidGenerator(
    u32 startFrame, u32 maxResults, u8 abilityType, u16 tsv, u8 genderType, u8 genderRatio, u8 ivCount)
{
    this->startFrame = startFrame;
    this->maxResults = maxResults;
    this->abilityType = abilityType;
    this->tsv = tsv;
    this->genderType = genderType;
    this->genderRatio = genderRatio;
    this->ivCount = ivCount;
}

QVector<Frame> RaidGenerator::generate(u64 seed)
{
    QVector<Frame> frames;

    for (u32 i = 1; i < startFrame; i++)
    {
        seed += 0x82A2B175229D6A5B;
    }

    for (u32 frame = 0; frame < maxResults; frame++, seed += 0x82A2B175229D6A5B)
    {
        XoroShiro rng(seed);
        Frame result(tsv);
        result.setFrame(startFrame + frame);

        u32 ec = static_cast<u32>(rng.nextInt());
        result.setEC(ec);

        u32 otid = static_cast<u32>(rng.nextInt());
        u32 pid = static_cast<u32>(rng.nextInt());

        u16 otsv = getSv(otid);
        u16 psv = getSv(pid);

        // Game uses a fake TID/SID to determine shiny or not
        // PID is later modified using the actual TID/SID of trainer if necessary
        if (otsv == psv) // Force shiny
        {
            result.setShiny(true);
            if (psv != tsv)
            {
                pid ^= tsv << 16;
            }
        }
        else // Force non shiny
        {
            result.setShiny(false);
            if (psv == tsv)
            {
                pid ^= 0x10000000;
            }
        }
        result.setPID(pid);

        // Set IVs that will be 31s
        for (u8 i = 0; i < ivCount;)
        {
            u8 index = static_cast<u8>(rng.nextInt(6));
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
                result.setIV(i, static_cast<u8>(rng.nextInt(32)));
            }
        }

        if (abilityType == 4) // Allow hidden ability
        {
            result.setAbility(static_cast<u8>(rng.nextInt(3)));
        }
        else if (abilityType == 3) // No hidden ability
        {
            result.setAbility(static_cast<u8>(rng.nextInt(2)));
        }

        // Altform, doesn't seem to have a rand call for raids

        if (genderType == 0) // Random
        {
            result.setGender(static_cast<u8>(rng.nextInt(252) + 1) < genderRatio);
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

        // TODO: toxtricity (why)
        result.setNature(static_cast<u8>(rng.nextInt(25)));

        // Do a filter eventually
        frames.push_back(result);
    }

    return frames;
}
