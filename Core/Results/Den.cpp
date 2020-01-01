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

#include "Den.hpp"

Den::Den(u64 hash, const QVector<Raid> &swordRaids, const QVector<Raid> &shieldRaids) :
    swordRaids(swordRaids), shieldRaids(shieldRaids), hash(hash)
{
}

Raid Den::getRaid(u8 index, Game version) const
{
    if (version == Game::Sword)
    {
        return swordRaids[index];
    }

    return shieldRaids[index];
}

QVector<QPair<u16, QString>> Den::getSpecies(Game version) const
{
    auto raids = version == Game::Sword ? swordRaids : shieldRaids;

    QVector<QPair<u16, QString>> species;
    for (const Raid &raid : raids)
    {
        u16 specie = raid.getSpecies();

        u8 low = 4;
        u8 high = 0;
        for (u8 i = 0; i < 5; i++)
        {
            if (raid.getStar(i))
            {
                if (i < low)
                {
                    low = i;
                }
                if (i > high)
                {
                    high = i;
                }
            }
        }

        QString star;
        if (low == high)
        {
            star = QString("%1★").arg(low + 1);
        }
        else
        {
            star = QString("%1-%2★").arg(low + 1).arg(high + 1);
        }

        species.append(qMakePair(specie, star));
    }
    return species;
}

u64 Den::getHash() const
{
    return hash;
}
