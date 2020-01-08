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

#include <Core/Global.hpp>
#include <Core/Results/Pokemon.hpp>
#include <QVector>

class SeedSearcher
{
public:
    explicit SeedSearcher(const QVector<Pokemon> &pokemon, int fixedIVs, int ivCount[2], bool firstResult);
    void setIVs(const QVector<u8> &templateIVs);
    void startSearch(int maxRolls, int threads);
    QVector<u64> getResults() const;

private:
    bool ivFlag;
    int fixedIVs;
    int ivCount[2];
    int ivOffset;
    bool firstResult;
    QVector<Pokemon> pokemon;
    QVector<u8> templateIVs;

    u64 tempMatrix[256];
    u64 inputMatrix[64];
    u64 constantTermVector;
    u64 coefficient[64];
    u64 answerFlag[64];
    int freeBit[64];
    int freeId[64];
    u64 coefficientData[0x4000];
    u64 searchPattern[0x4000];

    QVector<u64> results;

    void prepare(int offset);
    void initializeTransformationMatrix();
    void proceedTransformationMatrix();
    u64 getMatrixMultipler(int index);
    u16 getMatrixConst(int index);
    void calculateInverseMatrix(int length);
    void calculateCoefficientData(int length);
    void search(u64 &seed);
};

#endif // SEEDSEARCHER_HPP
