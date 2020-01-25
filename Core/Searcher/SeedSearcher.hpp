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

#ifndef SEEDSEARCHER_HPP
#define SEEDSEARCHER_HPP

#include <Core/Results/Pokemon.hpp>
#include <Core/Searcher/Matrix.hpp>
#include <QThreadPool>
#include <QVector>
#include <mutex>

class SeedSearcher
{
public:
    SeedSearcher(const QVector<Pokemon> &pokemon, const QVector<int> &ivCount, bool firstResult);
    virtual ~SeedSearcher() = default;
    virtual void startSearch(int minRolls, int maxRolls, int threads) = 0;
    void cancelSearch();
    QVector<u64> getResults() const;

protected:
    QVector<Pokemon> pokemon;
    QVector<int> ivCount;
    bool firstResult;
    int ivOffset;

    QThreadPool pool;
    Matrix matrix;
    QVector<u64> results;
    bool searching;
    std::mutex mutex;

    void search(u32 min, u32 max);
    virtual bool searchSeed(u64 &seed) = 0;
    u8 checkCharacteristic(u8 characteristic, u8 index) const;
};

#endif // SEEDSEARCHER_HPP
