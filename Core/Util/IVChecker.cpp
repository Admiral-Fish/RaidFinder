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

#include "IVChecker.hpp"
#include <Core/Util/Nature.hpp>
#include <QSet>
#include <cmath>

namespace
{
    QVector<QSet<u8>> calculateIVs(const PersonalInfo &info, const QVector<u16> &stats, u8 level, u8 nature)
    {
        QVector<QSet<u8>> ivs(6);
        QVector<u8> baseStats = info.getBaseStats();

        for (u8 i = 0; i < 6; i++)
        {
            for (u8 iv = 0; iv < 32; iv++)
            {
                double stat;
                if (i == 0)
                {
                    stat = std::floor(((2 * baseStats.at(i) + iv) * level) / 100.0) + level + 10;
                }
                else
                {
                    stat = (std::floor(((2 * baseStats.at(i) + iv) * level) / 100.0) + 5) * Nature::getNatureModifier(nature, i);
                }

                if (static_cast<u16>(stat) == stats.at(i))
                {
                    ivs[i].insert(iv);
                }
            }
        }

        return ivs;
    }
}

QVector<QVector<u8>> IVChecker::calculateIVRange(const PersonalInfo &info, const QVector<QVector<u16>> &stats, const QVector<u8> &level,
                                                 u8 nature)
{
    QVector<QSet<u8>> first = calculateIVs(info, stats.at(0), level.at(0), nature);

    for (int i = 1; i < stats.size(); i++)
    {
        auto next = calculateIVs(info, stats.at(i), level.at(i), nature);

        for (u8 j = 0; j < 6; j++)
        {
            first[j].intersect(next.at(j));
        }
    }

    QVector<QVector<u8>> ivs(6);
    for (u8 i = 0; i < 6; i++)
    {
        for (const u8 num : first.at(i))
        {
            ivs[i].append(num);
        }
        std::sort(ivs[i].begin(), ivs[i].end());
    }
    return ivs;
}
