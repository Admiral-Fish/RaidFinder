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

#include "Matrix.hpp"
#include <cstring>

constexpr u64 c_N[256] = {
    67108864ull,
    67108864ull,
    33554432ull,
    33554432ull,
    16777216ull,
    16777216ull,
    8388608ull,
    8388608ull,
    4194304ull,
    4194304ull,
    2097152ull,
    2097152ull,
    1048576ull,
    1048576ull,
    524288ull,
    524288ull,
    262144ull,
    262144ull,
    131072ull,
    131072ull,
    65536ull,
    65536ull,
    32768ull,
    32768ull,
    16384ull,
    16384ull,
    8192ull,
    8192ull,
    4096ull,
    4096ull,
    2048ull,
    2048ull,
    1024ull,
    1024ull,
    512ull,
    512ull,
    256ull,
    256ull,
    128ull,
    128ull,
    64ull,
    64ull,
    32ull,
    32ull,
    16ull,
    16ull,
    8ull,
    8ull,
    4ull,
    4ull,
    2ull,
    2ull,
    1ull,
    1ull,
    9223372036854775808ull,
    9223372036854775808ull,
    4611686018427387904ull,
    4611686018427387904ull,
    2305843009213693952ull,
    2305843009213693952ull,
    1152921504606846976ull,
    1152921504606846976ull,
    576460752303423488ull,
    576460752303423488ull,
    288230376151711744ull,
    288230376151711744ull,
    144115188075855872ull,
    144115188075855872ull,
    72057594037927936ull,
    72057594037927936ull,
    36028797018963968ull,
    36028797018963968ull,
    18014398509481984ull,
    18014398509481984ull,
    9007199254740992ull,
    9007199254740992ull,
    4503599627370496ull,
    4503599627370496ull,
    2251799813685248ull,
    2251799813685248ull,
    1125899906842624ull,
    1125899906842624ull,
    562949953421312ull,
    562949953421312ull,
    281474976710656ull,
    281474976710656ull,
    140737488355328ull,
    140737488355328ull,
    70368744177664ull,
    70368744177664ull,
    35184372088832ull,
    35184372088832ull,
    17592186044416ull,
    17592186044416ull,
    8796093022208ull,
    8796093022208ull,
    4398046511104ull,
    4398046511104ull,
    2199023255552ull,
    2199023255552ull,
    1099511627776ull,
    1099511627776ull,
    549755813888ull,
    549755813888ull,
    274877906944ull,
    274877906944ull,
    137438953472ull,
    137438953472ull,
    68719476736ull,
    68719476736ull,
    34359738368ull,
    34359738368ull,
    17179869184ull,
    17179869184ull,
    8589934592ull,
    8589934592ull,
    4294967296ull,
    4294967296ull,
    2147483648ull,
    2147483648ull,
    1073741824ull,
    1073741824ull,
    536870912ull,
    536870912ull,
    268435456ull,
    268435456ull,
    134217728ull,
    134217728ull,
    9223512774343131136ull,
    9223513324098945024ull,
    4611756387171565568ull,
    4611756662049472512ull,
    2305878193585782784ull,
    2305878331024736256ull,
    1152939096792891392ull,
    1152939165512368128ull,
    576469548396445696ull,
    576469582756184064ull,
    288234774198222848ull,
    288234791378092032ull,
    144117387099111424ull,
    144117395689046016ull,
    72058693549555712ull,
    72058697844523008ull,
    36029346774777856ull,
    36029348922261504ull,
    18014673387388928ull,
    18014674461130752ull,
    9007336693694464ull,
    9007337230565376ull,
    4503668346847232ull,
    4503668615282688ull,
    2251834173423616ull,
    2251834307641344ull,
    1125917086711808ull,
    1125917153820672ull,
    562958543355904ull,
    562958576910336ull,
    281479271677952ull,
    281479288455168ull,
    140739635838976ull,
    140739644227584ull,
    70369817919488ull,
    70369822113792ull,
    35184908959744ull,
    35184911056896ull,
    17592454479872ull,
    17592455528448ull,
    8796227239936ull,
    8796227764224ull,
    4398113619968ull,
    4398113882112ull,
    2199056809984ull,
    2199056941056ull,
    1099528404992ull,
    1099528470528ull,
    549764202496ull,
    549764235264ull,
    274882101248ull,
    274882117632ull,
    137441050624ull,
    137441058816ull,
    68720525312ull,
    68720529408ull,
    34360262656ull,
    34360264704ull,
    17180131328ull,
    17180132352ull,
    8590065664ull,
    8590066176ull,
    4295032832ull,
    4295033088ull,
    2147516416ull,
    2147516544ull,
    1073758208ull,
    1073758272ull,
    536879104ull,
    536879136ull,
    268439552ull,
    268439568ull,
    134219776ull,
    134219784ull,
    67109888ull,
    67109892ull,
    33554944ull,
    33554946ull,
    16777472ull,
    16777473ull,
    8388736ull,
    9223372036863164544ull,
    4194368ull,
    4611686018431582272ull,
    2097184ull,
    2305843009215791136ull,
    1048592ull,
    1152921504607895568ull,
    524296ull,
    576460752303947784ull,
    262148ull,
    288230376151973892ull,
    131074ull,
    144115188075986946ull,
    65537ull,
    72057594037993473ull,
    32768ull,
    36028797018996736ull,
    16384ull,
    18014398509498368ull,
    8192ull,
    9007199254749184ull,
    4096ull,
    4503599627374592ull,
    2048ull,
    2251799813687296ull,
    1024ull,
    1125899906843648ull,
    512ull,
    562949953421824ull,
    256ull,
    281474976710912ull,
    128ull,
    140737488355456ull,
    64ull,
    70368744177728ull,
    32ull,
    35184372088864ull,
    16ull,
    17592186044432ull,
    8ull,
    8796093022216ull,
    4ull,
    4398046511108ull,
    2ull,
    2199023255554ull,
    1ull,
    1099511627777ull,
};

inline u64 getSignature(u64 value)
{
    u32 result = static_cast<u32>((value ^ (value >> 32)));
    result = result ^ (result >> 16);
    result = result ^ (result >> 8);
    result = result ^ (result >> 4);
    result = result ^ (result >> 2);
    return (result ^ (result >> 1)) & 1;
}

Matrix::Matrix()
{
    std::memset(tempMatrix, 0, sizeof(tempMatrix));
    std::memset(inputMatrix, 0, sizeof(inputMatrix));
    std::memset(coefficient, 0, sizeof(coefficient));
    std::memset(answerFlag, 0, sizeof(answerFlag));
    std::memset(freeBit, false, sizeof(freeBit));
    std::memset(freeId, 0, sizeof(freeId));
    std::memset(coefficientData, 0, sizeof(coefficientData));
    std::memset(searchPattern, 0, sizeof(searchPattern));
}

void Matrix::prepare(bool ability, int ivOffset)
{
    int length = ability ? 57 : 56;

    constantTermVector = 0;

    initializeTransformationMatrix();
    for (int i = 0; i <= ivOffset + 1; i++)
    {
        proceedTransformationMatrix();
    }

    int bit = 0;
    for (int i = 0; i < 6; i++, bit++)
    {
        int index = 61 + (i / 3) * 64 + (i % 3);
        inputMatrix[bit] = getMatrixMultipler(index);
        if (getMatrixConst(index) != 0)
        {
            constantTermVector |= (1ull << (length - 1 - bit));
        }
    }

    for (int i = 0; i < 5; i++)
    {
        proceedTransformationMatrix();
        for (int j = 0; j < 10; j++, bit++)
        {
            int index = 59 + (j / 5) * 64 + (j % 5);
            inputMatrix[bit] = getMatrixMultipler(index);
            if (getMatrixConst(index) != 0)
            {
                constantTermVector |= (1ull << (length - 1 - bit));
            }
        }
    }

    if (ability)
    {
        proceedTransformationMatrix();

        inputMatrix[56] = getMatrixMultipler(63) ^ getMatrixMultipler(127);
        if ((getMatrixConst(63) ^ getMatrixConst(127)) != 0)
        {
            constantTermVector |= 1;
        }
    }

    calculateInverseMatrix(length);
    calculateCoefficientData(length);
}

void Matrix::prepareSix(int offset, int fixedIVs)
{
    int length = fixedIVs * 10;

    constantTermVector = 0;

    initializeTransformationMatrix();
    for (int i = 0; i <= 9 - fixedIVs + offset; i++)
    {
        proceedTransformationMatrix();
    }

    for (int i = 0; i < fixedIVs; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int index = 59 + (j / 5) * 64 + (j % 5);
            int bit = i * 10 + j;
            inputMatrix[bit] = getMatrixMultipler(index);

            if (getMatrixConst(index) != 0)
            {
                constantTermVector |= (1ull << (length - 1 - bit));
            }
        }
        proceedTransformationMatrix();
    }

    calculateInverseMatrix(length);
    calculateCoefficientData(length);
}

u64 Matrix::getConstantTermVector() const
{
    return constantTermVector;
}

bool Matrix::getFreeBit(int index) const
{
    return freeBit[index];
}

u64 Matrix::getModifiedAnswerFlag(int index, u64 target) const
{
    return getSignature(answerFlag[index] & target);
}

u64 Matrix::getAnswerFlag(int index) const
{
    return answerFlag[index];
}

u64 Matrix::getCoefficientData(u64 index) const
{
    return coefficientData[index];
}

u64 Matrix::getSearchPattern(u64 index) const
{
    return searchPattern[index];
}

void Matrix::initializeTransformationMatrix()
{
    std::memcpy(tempMatrix, c_N, sizeof(tempMatrix));
}

void Matrix::proceedTransformationMatrix()
{
    u64 temp[256];
    std::memcpy(temp, tempMatrix, sizeof(temp));

    for (int i = 0; i < 128; i++)
    {
        tempMatrix[i * 2] = 0;
        tempMatrix[i * 2 + 1] = 0;
        for (int j = 0; j < 64; j++)
        {
            u64 t0 = 0;
            u64 t1 = 0;
            for (int k = 0; k < 64; k++)
            {
                if ((c_N[i * 2] & (1ull << (63 - k))) != 0 && (temp[k * 2] & (1ull << (63 - j))) != 0)
                {
                    t0 = 1 - t0;
                }
                if ((c_N[i * 2 + 1] & (1ull << (63 - k))) != 0 && (temp[(k + 64) * 2] & (1ull << (63 - j))) != 0)
                {
                    t0 = 1 - t0;
                }

                if ((c_N[i * 2] & (1ull << (63 - k))) != 0 && (temp[k * 2 + 1] & (1ull << (63 - j))) != 0)
                {
                    t1 = 1 - t1;
                }
                if ((c_N[i * 2 + 1] & (1ull << (63 - k))) != 0 && (temp[(k + 64) * 2 + 1] & (1ull << (63 - j))) != 0)
                {
                    t1 = 1 - t1;
                }
            }
            tempMatrix[i * 2] |= (t0 << (63 - j));
            tempMatrix[i * 2 + 1] |= (t1 << (63 - j));
        }
    }
}

u64 Matrix::getMatrixMultipler(int index) const
{
    return tempMatrix[index * 2 + 1];
}

u16 Matrix::getMatrixConst(int index) const
{
    return static_cast<u16>(getSignature(tempMatrix[index * 2] & 0x82a2b175229d6a5b));
}

void Matrix::calculateInverseMatrix(int length)
{
    for (int i = 0; i < length; i++)
    {
        answerFlag[i] = (1ull << (length - 1 - i));
    }

    std::memset(freeBit, false, sizeof(freeBit));

    int skip = 0;
    for (int rank = 0; rank + skip < 64;)
    {
        u64 top = (1ull << (63 - (rank + skip)));
        bool flag = false;
        for (int i = rank; i < length; i++)
        {
            if ((inputMatrix[i] & top) != 0)
            {
                for (int j = 0; j < length; j++)
                {
                    if (j == i)
                    {
                        continue;
                    }

                    if ((inputMatrix[j] & top) != 0)
                    {
                        inputMatrix[j] ^= inputMatrix[i];
                        answerFlag[j] ^= answerFlag[i];
                    }
                }

                u64 tempM = inputMatrix[rank];
                u64 tempF = answerFlag[rank];
                inputMatrix[rank] = inputMatrix[i];
                answerFlag[rank] = answerFlag[i];
                inputMatrix[i] = tempM;
                answerFlag[i] = tempF;

                rank++;
                flag = true;
                break;
            }
        }

        if (!flag)
        {
            freeBit[rank + skip] = true;
            freeId[skip] = rank + skip;
            skip++;
        }
    }

    for (int i = 0; i < length; i++)
    {
        coefficient[i] = 0;
        for (int j = 0; j < 64 - length; j++)
        {
            coefficient[i] |= (inputMatrix[i] & (1ull << (63 - freeId[j]))) >> ((length + j) - freeId[j]);
        }
    }
}

void Matrix::calculateCoefficientData(int length)
{
    u32 max = (1 << (64 - length)) - 1;

    for (u32 search = 0; search <= max; search++)
    {
        coefficientData[search] = 0;
        searchPattern[search] = 0;

        int offset = 0;
        for (int i = 0; i < length; i++)
        {
            while (freeBit[i + offset])
            {
                offset++;
            }
            coefficientData[search] |= getSignature(coefficient[i] & search) << (63 - (i + offset));
        }

        for (int i = 0; i < 64 - length; i++)
        {
            searchPattern[search] |= (search & (1ull << (64 - length - 1 - i))) << ((length + i) - freeId[i]);
        }
    }
}
