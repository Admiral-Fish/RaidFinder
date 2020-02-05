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

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <Core/Util/Global.hpp>

class Matrix
{
public:
    Matrix();
    void prepare(bool ability, int rerolls);
    void prepareSix(int offset, int fixedIVs);
    u64 getConstantTermVector() const;
    int getFreeBit(int index) const;
    u64 getModifiedAnswerFlag(int index, u64 target) const;
    u64 getAnswerFlag(int index) const;
    u64 getCoefficientData(u64 index) const;
    u64 getSearchPattern(u64 index) const;

private:
    u64 tempMatrix[256];
    u64 inputMatrix[64];
    u64 constantTermVector;
    u64 coefficient[64];
    u64 answerFlag[64];
    int freeBit[64];
    int freeId[64];
    u64 coefficientData[0x4000];
    u64 searchPattern[0x4000];

    void initializeTransformationMatrix();
    void proceedTransformationMatrix();
    u64 getMatrixMultipler(int index) const;
    u16 getMatrixConst(int index) const;
    void calculateInverseMatrix(int length);
    void calculateCoefficientData(int length);
};

#endif // MATRIX_HPP
