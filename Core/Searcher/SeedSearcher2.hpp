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

#ifndef SEEDSEARCHER2_HPP
#define SEEDSEARCHER2_HPP

#include <Core/Searcher/SeedSearcher.hpp>

class SeedSearcher2 final : public SeedSearcher
{
public:
    SeedSearcher2(const QVector<Pokemon> &pokemon, const QVector<int> &ivCount, const QVector<u8> &templateIVs, bool firstResult);
    void startSearch(int minRolls, int maxRolls, int threads) override final;

private:
    bool searchSeed(u64 &seed) const override final;
};

#endif // SEEDSEARCHER35_HPP
