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

#include "IVChecker.hpp"
#include <QSet>
#include <cmath>

constexpr double modifiers[25][6] = {
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Hardy
    { 1.0, 1.1, 0.9, 1.0, 1.0, 1.0 }, // Lonely
    { 1.0, 1.1, 1.0, 1.0, 1.0, 0.9 }, // Brave
    { 1.0, 1.1, 1.0, 0.9, 1.0, 1.0 }, // Adamant
    { 1.0, 1.1, 1.0, 1.0, 0.9, 1.0 }, // Naughty
    { 1.0, 0.9, 1.1, 1.0, 1.0, 1.0 }, // Bold
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Docile
    { 1.0, 1.0, 1.1, 1.0, 1.0, 0.9 }, // Relaxed
    { 1.0, 1.0, 1.1, 0.9, 1.0, 1.0 }, // Impish
    { 1.0, 1.0, 1.1, 1.0, 0.9, 1.0 }, // Lax
    { 1.0, 0.9, 1.0, 1.0, 1.0, 1.1 }, // Timid
    { 1.0, 1.0, 0.9, 1.0, 1.0, 1.1 }, // Hasty
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Serious
    { 1.0, 1.0, 1.0, 0.9, 1.0, 1.1 }, // Jolly
    { 1.0, 1.0, 1.0, 1.0, 0.9, 1.1 }, // Naive
    { 1.0, 0.9, 1.0, 1.1, 1.0, 1.0 }, // Modest
    { 1.0, 1.0, 0.9, 1.1, 1.0, 1.0 }, // Mild
    { 1.0, 1.0, 1.0, 1.1, 1.0, 0.9 }, // Quiet
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Bashful
    { 1.0, 1.0, 1.0, 1.1, 0.9, 1.0 }, // Rash
    { 1.0, 0.9, 1.0, 1.0, 1.1, 1.0 }, // Calm
    { 1.0, 1.0, 0.9, 1.0, 1.1, 1.0 }, // Gentle
    { 1.0, 1.0, 1.0, 1.0, 1.1, 0.9 }, // Sassy
    { 1.0, 1.0, 1.0, 0.9, 1.1, 1.0 }, // Careful
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 } // Quirky
};

namespace
{
    QVector<QSet<u8>> calculateIVs(Personal info, const QVector<u16> &stats, u8 level, u8 nature)
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
                    stat = (std::floor(((2 * baseStats.at(i) + iv) * level) / 100.0) + 5) * modifiers[nature][i];
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

QVector<QVector<u8>> IVChecker::calculateIVRange(Personal info, const QVector<QVector<u16>> &stats, const QVector<u8> &level, u8 nature)
{
    QVector<QSet<u8>> first = calculateIVs(info, stats.at(0), level.at(0), nature);

    for (u8 i = 1; i < stats.size(); i++)
    {
        auto next = calculateIVs(info, stats.at(i), level.at(i), nature);

        for (u8 j = 0; j < 6; j++)
        {
            first[j].intersect(next[j]);
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
