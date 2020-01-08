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

#include "SeedSearcher.hpp"
#include <Core/RNG/XoroShiro.hpp>
#include <QtConcurrent>
#include <memory>

const u64 c_N[256] = {
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

constexpr u8 toxtricityAmpedNatures[13] = { 3, 4, 2, 8, 9, 19, 22, 11, 13, 14, 0, 6, 24 };

inline u64 getSignature(u64 value)
{
    u32 result = static_cast<u32>((value ^ (value >> 32)));
    result = result ^ (result >> 16);
    result = result ^ (result >> 8);
    result = result ^ (result >> 4);
    result = result ^ (result >> 2);
    return (result ^ (result >> 1)) & 1;
}

SeedSearcher::SeedSearcher(const QVector<Pokemon> &pokemon, int fixedIVs, int ivCount[], bool firstResult) :
    ivFlag(fixedIVs == 3),
    fixedIVs(fixedIVs == 2 ? 6 : 5),
    firstResult(firstResult),
    pokemon(pokemon)
{
    for (u8 i = 0; i < 2; i++)
    {
        this->ivCount[i] = ivCount[i];
    }

    std::memset(tempMatrix, 0, sizeof(u64) * 256);
    std::memset(inputMatrix, 0, sizeof(u64) * 64);
    std::memset(coefficient, 0, sizeof(u64) * 64);
    std::memset(answerFlag, 0, sizeof(u64) * 64);
    std::memset(freeBit, 0, sizeof(int) * 64);
    std::memset(freeId, 0, sizeof(int) * 64);
    std::memset(coefficientData, 0, sizeof(u64) * 0x4000);
    std::memset(searchPattern, 0, sizeof(u64) * 0x4000);
}

void SeedSearcher::setIVs(const QVector<u8> &templateIVs)
{
    this->templateIVs = templateIVs;
}

void SeedSearcher::startSearch(int maxRolls, int threads)
{
    QThreadPool::globalInstance()->setMaxThreadCount(threads);
    for (int i = 0; i <= maxRolls; i++)
    {
        prepare(i);

        u8 max = ivFlag ? 1 : 32;
        for (u8 j = 0; j < max; j++)
        {
            QVector<u64> seeds;
            seeds.reserve(0x2000000);
            for (u32 k = 0x2000000u * j; k < 0x2000000u * (j + 1); k++)
            {
                seeds.append(k);
            }

            QtConcurrent::blockingMap(seeds, [this](u64 &seed) { search(seed); });

            for (u64 seed : seeds)
            {
                if (seed)
                {
                    results.append(seed - (0x82a2b175229d6a5b * 5));
                }
            }

            if (firstResult && !results.isEmpty())
            {
                return;
            }
        }
    }
}

QVector<u64> SeedSearcher::getResults() const
{
    return results;
}

void SeedSearcher::prepare(int offset)
{
    int length = fixedIVs * 10;

    ivOffset = offset;
    constantTermVector = 0;

    initializeTransformationMatrix();
    for (int i = 0; i <= 9 - fixedIVs + ivOffset; i++)
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

void SeedSearcher::initializeTransformationMatrix()
{
    std::memcpy(tempMatrix, c_N, sizeof(u64) * 256);
}

void SeedSearcher::proceedTransformationMatrix()
{
    u64 temp[256];
    std::memcpy(temp, tempMatrix, sizeof(u64) * 256);

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

u64 SeedSearcher::getMatrixMultipler(int index)
{
    return tempMatrix[index * 2 + 1];
}

u16 SeedSearcher::getMatrixConst(int index)
{
    return static_cast<u16>(getSignature(tempMatrix[index * 2] & 0x82a2b175229d6a5b));
}

void SeedSearcher::calculateInverseMatrix(int length)
{
    for (int i = 0; i < length; i++)
    {
        answerFlag[i] = (1ull << (length - 1 - i));
    }

    std::memset(freeBit, 0, sizeof(int) * 64);

    int skip = 0;
    for (int rank = 0; rank < length;)
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
            }
        }

        if (!flag)
        {
            freeBit[rank + skip] = 1;
            freeId[skip] = rank + skip;
            skip++;
        }
    }

    for (int i = length + skip; i < 64; i++)
    {
        freeBit[i] = 1;
        freeId[i - length] = i;
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

void SeedSearcher::calculateCoefficientData(int length)
{
    u16 max = static_cast<u16>((1 << (64 - length)) - 1);

    for (u16 search = 0; search <= max; search++)
    {
        coefficientData[search] = 0;
        searchPattern[search] = 0;

        int offset = 0;
        for (int i = 0; i < length; i++)
        {
            while (freeBit[i + offset] > 0)
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

void SeedSearcher::search(u64 &seed)
{
    /*{
        std::lock_guard<std::mutex> lock(mutex);
        progress++;
    }*/

    int length = fixedIVs * 10;

    XoroShiro rng;

    u64 target = 0;

    if (fixedIVs == 6)
    {
        target |= (seed & 0x3E000000ul) << 30;
        target |= (seed & 0x1F00000ul) << 25;
        target |= (seed & 0xF8000ul) << 20;
        target |= (seed & 0x7C00ul) << 15;
        target |= (seed & 0x3E0ul) << 10;
        target |= (seed & 0x1Ful) << 5;

        target |= ((32ul + templateIVs.at(0) - ((seed & 0x3E000000ul) >> 25)) & 0x1F) << 50;
        target |= ((32ul + templateIVs.at(1) - ((seed & 0x1F00000ul) >> 20)) & 0x1F) << 40;
        target |= ((32ul + templateIVs.at(2) - ((seed & 0xF8000ul) >> 15)) & 0x1F) << 30;
        target |= ((32ul + templateIVs.at(3) - ((seed & 0x7C00ul) >> 10)) & 0x1F) << 20;
        target |= ((32ul + templateIVs.at(4) - ((seed & 0x3E0ul) >> 5)) & 0x1F) << 10;
        target |= ((32ul + templateIVs.at(5) - (seed & 0x1Ful)) & 0x1F);
    }
    else if (fixedIVs == 5)
    {
        target |= (seed & 0x1F00000ul) << 25;
        target |= (seed & 0xF8000ul) << 20;
        target |= (seed & 0x7C00ul) << 15;
        target |= (seed & 0x3E0ul) << 10;
        target |= (seed & 0x1Ful) << 5;

        target |= ((32ul + templateIVs.at(0) - ((seed & 0x1F00000ul) >> 20)) & 0x1F) << 40;
        target |= ((32ul + templateIVs.at(1) - ((seed & 0xF8000ul) >> 15)) & 0x1F) << 30;
        target |= ((32ul + templateIVs.at(2) - ((seed & 0x7C00ul) >> 10)) & 0x1F) << 20;
        target |= ((32ul + templateIVs.at(3) - ((seed & 0x3E0ul) >> 5)) & 0x1F) << 10;
        target |= ((32ul + templateIVs.at(4) - (seed & 0x1Ful)) & 0x1F);
    }
    else
    {
        seed = 0;
        return;
    }

    target ^= constantTermVector;

    u64 processedTarget = 0;
    int offsetTarget = 0;
    for (int i = 0; i < length; i++)
    {
        while (freeBit[i + offsetTarget] > 0)
        {
            offsetTarget++;
        }
        processedTarget |= (getSignature(answerFlag[i] & target) << (63 - (i + offsetTarget)));
    }

    u64 max = ((1 << (64 - length)) - 1);
    for (u64 search = 0; search <= max; search++)
    {
        u64 searchSeed = (processedTarget ^ coefficientData[search]) ^ searchPattern[search];

        rng.setSeed(searchSeed);

        u32 ec = rng.nextInt(0xffffffff, 0xffffffff);

        {
            u8 characteristic = ec % 6;
            for (u8 i = 0; i < 6; i++)
            {
                characteristic = (characteristic + i) % 6;
                if (pokemon[0].checkCharacteristic(characteristic))
                {
                    break;
                }
            }

            if (characteristic != pokemon[0].getCharacteristic())
            {
                continue;
            }
        }

        {
            u8 characteristic = ec % 6;
            for (u8 i = 0; i < 6; i++)
            {
                characteristic = (characteristic + i) % 6;
                if (pokemon[1].checkCharacteristic(characteristic))
                {
                    break;
                }
            }

            if (characteristic != pokemon[1].getCharacteristic())
            {
                continue;
            }
        }

        rng.nextInt(0xffffffff, 0xffffffff); // SIDTID
        rng.nextInt(0xffffffff, 0xffffffff); // PID
        {
            u8 ivs[6] = { 255, 255, 255, 255, 255, 255 };
            int count = 0;
            int offset = -(8 - fixedIVs);

            do
            {
                u8 stat;
                do
                {
                    stat = static_cast<u8>(rng.nextInt(7));
                    offset++;
                } while (stat >= 6);

                if (ivs[stat] == 255)
                {
                    ivs[stat] = 31;
                    count++;
                }
            } while (count < (8 - fixedIVs));

            if (offset != ivOffset)
            {
                continue;
            }

            bool flag = true;
            for (u8 i = 0; i < 6; i++)
            {
                if (ivs[i] != 31)
                {
                    ivs[i] = static_cast<u8>(rng.nextInt(31));
                }

                if (ivs[i] != pokemon[0].getIV(i))
                {
                    flag = false;
                    break;
                }
            }

            if (!flag)
            {
                continue;
            }

            u8 ability;
            if (pokemon[0].getAllowHiddenAbility())
            {
                ability = static_cast<u8>(rng.nextInt(3, 3));
            }
            else
            {
                ability = static_cast<u8>(rng.nextInt(1));
            }
            if (pokemon[0].getAbility() != 255 && pokemon[0].getAbility() != ability)
            {
                continue;
            }

            if (!pokemon[0].getGenderLocked())
            {
                rng.nextInt(253, 255);
            }

            u8 nature;
            if (pokemon[0].getSpecies() != 849)
            {
                nature = static_cast<u8>(rng.nextInt(25, 31));
            }
            else
            {
                nature = toxtricityAmpedNatures[rng.nextInt(13, 15)];
            }

            if (nature != pokemon[0].getNature())
            {
                continue;
            }
        }

        bool flag = true;
        for (u8 i = 1; i < 3; i++, searchSeed += 0x82a2b175229d6a5bull)
        {
            rng.setSeed(searchSeed);

            ec = rng.nextInt(0xffffffff, 0xffffffff);
            rng.nextInt(0xffffffff, 0xffffffff); // SIDTID
            rng.nextInt(0xffffffff, 0xffffffff); // PID

            if (i > 1)
            {
                u8 characteristic = ec % 6;
                for (u8 j = 0; j < 6; j++)
                {
                    characteristic = (characteristic + j) % 6;
                    if (pokemon[i].checkCharacteristic(characteristic))
                    {
                        break;
                    }
                }

                if (characteristic != pokemon[i].getCharacteristic())
                {
                    flag = false;
                    break;
                }
            }

            u8 ivs[6] = { 255, 255, 255, 255, 255, 255 };
            int count = 0;
            do
            {
                u8 stat = static_cast<u8>(rng.nextInt(6, 7));

                if (ivs[stat] == 255)
                {
                    ivs[stat] = 31;
                    count++;
                }
            } while (count < ivCount[i - 1]);

            for (u8 j = 0; j < 6; j++)
            {
                if (ivs[j] != 31)
                {
                    ivs[j] = static_cast<u8>(rng.nextInt(31));
                }

                if (ivs[j] != pokemon[i].getIV(j))
                {
                    flag = false;
                    break;
                }
            }
            if (!flag)
            {
                break;
            }

            u8 ability;
            if (pokemon[i].getAllowHiddenAbility())
            {
                ability = static_cast<u8>(rng.nextInt(3, 3));
            }
            else
            {
                ability = static_cast<u8>(rng.nextInt(1));
            }
            if ((pokemon[i].getAbility() != ability) || (pokemon[i].getAbility() == 255 && ability >= 2))
            {
                flag = false;
                break;
            }

            if (!pokemon[i].getGenderLocked())
            {
                rng.nextInt(253, 255);
            }

            u8 nature;
            if (pokemon[i].getSpecies() != 849)
            {
                nature = static_cast<u8>(rng.nextInt(25, 31));
            }
            else
            {
                nature = toxtricityAmpedNatures[rng.nextInt(13, 15)];
            }

            if (nature != pokemon[i].getNature())
            {
                flag = false;
                break;
            }
        }

        if (flag)
        {
            seed = searchSeed;
            return;
        }
    }
    seed = 0;
    return;
}
