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

#ifndef XOROSHIRO_HPP
#define XOROSHIRO_HPP

#include <Core/Util/Global.hpp>

class XoroShiro
{
public:
    XoroShiro() : state0(0), state1(0x82A2B175229D6A5B)
    {
    }

    explicit XoroShiro(u64 seed) : state0(seed), state1(0x82A2B175229D6A5B)
    {
    }

    u32 nextInt(u32 max, u32 mask)
    {
        u32 result;
        do
        {
            result = next() & mask;
        } while (result >= max);
        return result;
    }

    u32 nextInt(u32 mask)
    {
        return next() & mask;
    }

private:
    u64 state0;
    u64 state1;

    u64 next()
    {
        const u64 s0 = state0;
        u64 s1 = state1;
        const u64 result = s0 + s1;

        auto rotl = [](u64 x, int k) { return (x << k) | (x >> (64 - k)); };

        s1 ^= s0;
        state0 = rotl(s0, 24) ^ s1 ^ (s1 << 16);
        state1 = rotl(s1, 37);

        return result;
    }
};

#endif // XOROSHIRO_HPP
