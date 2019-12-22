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

#include "XoroShiro.hpp"

static inline u64 rotl(u64 x, u8 k)
{
    return (x << k) | (x >> (64 - k));
}

static inline u32 nextPower(u32 num)
{
    switch (num)
    {
    case 2:
        return 1;
    case 3:
        return 3;
    case 6:
        return 7;
    case 32:
        return 31;
    case 253:
        return 255;
    case 0xffffffff:
        return 0xffffffff;
    }
    return 0;
}

XoroShiro::XoroShiro(u64 seed)
{
    state[0] = seed;
    state[1] = 0x82A2B175229D6A5B;
}

u64 XoroShiro::nextInt(u32 num)
{
    u32 mask = nextPower(num);

    u64 result;
    do
    {
        result = next() & mask;
    } while (result >= num);
    return result;
}

u64 XoroShiro::next()
{
    const u64 s0 = state[0];
    u64 s1 = state[1];
    const u64 result = s0 + s1;

    s1 ^= s0;
    state[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);
    state[1] = rotl(s1, 37);

    return result;
}
