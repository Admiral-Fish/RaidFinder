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

#ifndef FRAMECOMPARE_HPP
#define FRAMECOMPARE_HPP

#include <Core/Results/Frame.hpp>
#include <QVector>

class FrameCompare
{
public:
    FrameCompare() = default;
    FrameCompare(u8 gender, u8 ability, u8 shiny, bool skip, const QVector<u8> &min, const QVector<u8> &max,
        const QVector<bool> &natures);
    bool compareFrame(const Frame &frame) const;

private:
    QVector<u8> min;
    QVector<u8> max;
    u8 gender {};
    u8 ability {};
    QVector<bool> natures;
    u8 shiny {};
    bool skip {};
};

#endif // FRAMECOMPARE_HPP
