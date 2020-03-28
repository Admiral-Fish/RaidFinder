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

#include "XoroShiro.hpp"

static inline u64 rotl(u64 x, u8 k)
{
    return (x << k) | (x >> (64 - k));
}

XoroShiro::XoroShiro() : state { 0, 0x82A2B175229D6A5B }
{
}

XoroShiro::XoroShiro(u64 seed) : state { seed, 0x82A2B175229D6A5B }
{
}

void XoroShiro::setSeed(u64 seed)
{
    state[0] = seed;
    state[1] = 0x82A2B175229D6A5B;
}

u32 XoroShiro::nextInt(u32 max, u32 mask, int &count)
{
    u32 result;
    do
    {
        result = next() & mask;
        count++;
    } while (result >= max);
    return result;
}

u32 XoroShiro::nextInt(u32 max, u32 mask)
{
    u32 result;
    do
    {
        result = next() & mask;
    } while (result >= max);
    return result;
}

u32 XoroShiro::nextInt(u32 mask)
{
    return next() & mask;
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
