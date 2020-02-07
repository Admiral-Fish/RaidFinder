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

#include "DenLoader.hpp"
#include <QApplication>
#include <QFile>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Normal hash, rare hash, location, x, y
constexpr u64 denInfo[100][5] = {
    { 0x173f0456dc5dfc52, 0xba83e1671012ebcd, 12, 185, 977 }, // 16 52  ; 0
    { 0x17458556dc634333, 0xba8745671015cb90, 12, 125, 1005 }, // 37 64
    { 0x17458b56dc634d65, 0x450421d99cf882c1, 12, 114, 936 }, // 31 90
    { 0x17428156dc610690, 0xba805467100fc65f, 12, 311, 998 }, // 3 51
    { 0x17428856dc611275, 0xba805767100fcb78, 12, 233, 948 }, // 4 46
    { 0x17458956dc6349ff, 0xba8747671015cef6, 12, 337, 996 }, // 33 62
    { 0x17459356dc635afd, 0xba8746671015cd43, 12, 209, 976 }, // 39 65
    { 0x17428356dc6109f6, 0xba805967100fcede, 12, 136, 906 }, // 1 48
    { 0x17458b56dc634d65, 0x450421d99cf882c1, 12, 252, 905 }, // 31 90
    { 0x17491656dc666f6d, 0xba83da671012dfe8, 2, 30, 927 }, // 26 59
    { 0x17490856dc6657a3, 0x4500a2d99cf5751d, 2, 12, 851 }, // 28 79    ; 10
    { 0x17491656dc666f6d, 0xba83db671012e19b, 2, 12, 861 }, // 26 58
    { 0x17428856dc611275, 0x45041fd99cf87f5b, 2, 20, 913 }, // 4 92
    { 0x17491656dc666f6d, 0xba83da671012dfe8, 2, 40, 878 }, // 26 59
    { 0x17428256dc610843, 0xba805367100fc4ac, 15, 52, 776 }, // 2 50
    { 0x17428656dc610f0f, 0xba805867100fcd2b, 15, 68, 652 }, // 6 47
    { 0x0000000000000000, 0x0000000000000000, 15, 50, 700 }, // Special
    { 0x17428556dc610d5c, 0xba805d67100fd5aa, 4, 217, 913 }, // 7 44
    { 0x17459356dc635afd, 0xba8746671015cd43, 4, 158, 705 }, // 39 65
    { 0x17458b56dc634d65, 0xba83d8671012dc82, 4, 220, 759 }, // 31 61
    { 0x17458b56dc634d65, 0xba83d8671012dc82, 4, 248, 852 }, // 31 61   ; 20
    { 0x17428b56dc61178e, 0xba8a4e67101810b2, 16, 129, 818 }, // 9 75
    { 0x17428b56dc61178e, 0xba8a4e67101810b2, 16, 131, 735 }, // 9 75
    { 0x17501a56dc6c94e7, 0xba805d67100fd5aa, 16, 105, 907 }, // 44 42
    { 0x17428556dc610d5c, 0xba805d67100fd5aa, 16, 50, 909 }, // 7 44
    { 0x17428b56dc61178e, 0x450420d99cf8810e, 16, 98, 750 }, // 9 91
    { 0x17458756dc634699, 0xba8748671015d0a9, 16, 107, 652 }, // 35 63
    { 0x17459256dc63594a, 0xba8745671015cb90, 0, 186, 816 }, // 38 64
    { 0x17428c56dc611941, 0x450420d99cf8810e, 13, 310, 824 }, // 8 91
    { 0x17501b56dc6c969a, 0xba8a496710180833, 13, 359, 980 }, // 41 76
    { 0x17501b56dc6c969a, 0xba8a496710180833, 13, 393, 962 }, // 41 76  ; 30
    { 0x17428756dc6110c2, 0xba805767100fcb78, 13, 328, 761 }, // 5 46
    { 0x17428356dc6109f6, 0xba805c67100fd3f7, 13, 352, 765 }, // 1 43
    { 0x173f0356dc5dfa9f, 0xba805467100fc65f, 7, 443, 895 }, // 15 51
    { 0x173f0056dc5df586, 0xba805e67100fd75d, 7, 388, 817 }, // 12 45
    { 0x173eff56dc5df3d3, 0xba805a67100fd091, 7, 443, 830 }, // 11 49
    { 0x173f0356dc5dfa9f, 0x45009ed99cf56e51, 7, 410, 952 }, // 15 83
    { 0x173eff56dc5df3d3, 0x450097d99cf5626c, 7, 447, 815 }, // 11 84
    { 0x173efe56dc5df220, 0xba805967100fcede, 11, 393, 781 }, // 10 48
    { 0x17501b56dc6c969a, 0xba8a496710180833, 11, 314, 755 }, // 41 76
    { 0x17490756dc6655f0, 0xba83d9671012de35, 11, 440, 658 }, // 29 60  ; 40
    { 0x17501b56dc6c969a, 0xba8a496710180833, 11, 281, 717 }, // 41 76
    { 0x17490756dc6655f0, 0xba83d9671012de35, 11, 440, 703 }, // 29 60
    { 0x17490756dc6655f0, 0x450425d99cf8898d, 4, 310, 654 }, // 29 86
    { 0x173efe56dc5df220, 0xba805967100fcede, 11, 443, 792 }, // 10 48
    { 0x173f0256dc5df8ec, 0xba805367100fc4ac, 10, 412, 533 }, // 14 50
    { 0x17458a56dc634bb2, 0xba83d9671012de35, 10, 345, 537 }, // 30 60
    { 0x17491456dc666c07, 0xba83dd671012e501, 10, 365, 559 }, // 24 56
    { 0x17501c56dc6c984d, 0xba8746671015cd43, 10, 408, 570 }, // 40 65
    { 0x17458656dc6344e6, 0x45009dd99cf56c9e, 1, 193, 295 }, // 34 82
    { 0x173f0156dc5df739, 0x450424d99cf887da, 1, 274, 321 }, // 13 87   ; 50
    { 0x17428c56dc611941, 0xba805d67100fd5aa, 1, 328, 330 }, // 8 44
    { 0x173f0456dc5dfc52, 0xba83e1671012ebcd, 1, 370, 452 }, // 16 52
    { 0x17501c56dc6c984d, 0xba8746671015cd43, 1, 224, 282 }, // 40 65
    { 0x17428856dc611275, 0x45041fd99cf87f5b, 1, 342, 312 }, // 4 92
    { 0x17428756dc6110c2, 0xba805767100fcb78, 1, 340, 269 }, // 5 46
    { 0x17458456dc634180, 0xba8748671015d0a9, 1, 305, 323 }, // 36 63
    { 0x17491556dc666dba, 0xba83da671012dfe8, 1, 334, 368 }, // 27 59
    { 0x17501c56dc6c984d, 0xba874b671015d5c2, 14, 343, 222 }, // 40 66
    { 0x17428356dc6109f6, 0x45009cd99cf56aeb, 14, 348, 195 }, // 1 81
    { 0x17428156dc610690, 0xba805467100fc65f, 14, 200, 244 }, // 3 51   ; 60
    { 0x173f0756dc5e016b, 0x45009bd99cf56938, 14, 305, 183 }, // 19 80
    { 0x17428656dc610f0f, 0xba805c67100fd3f7, 14, 348, 180 }, // 6 43
    { 0x17491556dc666dba, 0xba83da671012dfe8, 14, 260, 199 }, // 27 59
    { 0x17428256dc610843, 0xba805367100fc4ac, 14, 211, 205 }, // 2 50
    { 0x173f0056dc5df586, 0x450098d99cf5641f, 14, 303, 242 }, // 12 85
    { 0x17491256dc6668a1, 0xba83de671012e6b4, 14, 324, 209 }, // 22 55
    { 0x173f0256dc5df8ec, 0xba805367100fc4ac, 14, 326, 219 }, // 14 50
    { 0x173f0656dc5dffb8, 0xba83df671012e867, 14, 330, 215 }, // 18 54
    { 0x17458756dc634699, 0xba8748671015d0a9, 14, 282, 195 }, // 35 63
    { 0x173f0556dc5dfe05, 0x45041ed99cf87da8, 3, 265, 139 }, // 17 93   ; 70
    { 0x173f0556dc5dfe05, 0xba83e1671012ebcd, 3, 307, 93 }, // 17 52
    { 0x173f0356dc5dfa9f, 0x45041ed99cf87da8, 3, 331, 84 }, // 15 93
    { 0x17428b56dc61178e, 0xba8a4e67101810b2, 3, 219, 99 }, // 9 75
    { 0x173eff56dc5df3d3, 0xba805a67100fd091, 3, 243, 120 }, // 11 49
    { 0x173efe56dc5df220, 0xba805967100fcede, 3, 262, 174 }, // 10 48
    { 0x17490f56dc666388, 0xba83de671012e6b4, 3, 283, 98 }, // 21 55
    { 0x17491056dc66653b, 0xba83df671012e867, 3, 304, 112 }, // 20 54
    { 0x17490856dc6657a3, 0xba805c67100fd3f7, 6, 306, 145 }, // 28 43
    { 0x17458456dc634180, 0x450423d99cf88627, 3, 214, 168 }, // 36 88
    { 0x17501a56dc6c94e7, 0xba874c671015d775, 6, 334, 145 }, // 42 67   ; 80
    { 0x17458456dc634180, 0xba874a671015d40f, 6, 347, 103 }, // 36 69
    { 0x17491556dc666dba, 0xba874f671015dc8e, 6, 363, 88 }, // 27 70
    { 0x17491356dc666a54, 0xba8a4d6710180eff, 6, 338, 122 }, // 25 72
    { 0x173f0056dc5df586, 0xba805e67100fd75d, 8, 339, 35 }, // 12 45
    { 0x17458856dc63484c, 0xba83d8671012dc82, 8, 310, 65 }, // 32 61
    { 0x17458a56dc634bb2, 0xba83dc671012e34e, 8, 202, 34 }, // 30 57
    { 0x173f0756dc5e016b, 0xba83e0671012ea1a, 8, 237, 67 }, // 19 53
    { 0x17491156dc6666ee, 0xba8a4c6710180d4c, 8, 183, 47 }, // 23 73
    { 0x17458956dc6349ff, 0xba8747671015cef6, 8, 221, 50 }, // 33 62
    { 0x173f0256dc5df8ec, 0xba8749671015d25c, 8, 354, 60 }, // 14 68    ; 90
    { 0x17458a56dc634bb2, 0xba83d9671012de35, 5, 181, 185 }, // 30 60
    { 0x17491156dc6666ee, 0xba83de671012e6b4, 5, 199, 145 }, // 23 55
    { 0x173f0656dc5dffb8, 0xba8750671015de41, 5, 193, 173 }, // 18 71
    { 0x17458856dc63484c, 0x450422d99cf88474, 5, 202, 95 }, // 32 89
    { 0x17491456dc666c07, 0x4500a1d99cf5736a, 5, 185, 135 }, // 24 78
    { 0x17491356dc666a54, 0xba83dd671012e501, 9, 170, 35 }, // 25 56
    { 0x173f0656dc5dffb8, 0x4500a0d99cf571b7, 9, 128, 58 }, // 18 77
    { 0x17428c56dc611941, 0xba805d67100fd5aa, 9, 161, 80 }, // 8 44
    { 0x17458656dc6344e6, 0xba8a4f6710181265, 9, 143, 39 }, // 34 74
};

namespace
{
    QHash<u64, Den> dens;
}

void DenLoader::init()
{
    // Normal and rare dens
    QFile f(":/encounters/nests.bin");
    if (f.open(QIODevice::ReadOnly))
    {
        QJsonObject doc(QJsonDocument::fromJson(qUncompress(f.readAll())).object());
        f.close();

        QJsonArray tables = doc["Tables"].toArray();
        for (auto i : tables)
        {
            QJsonObject table = i.toObject();
            QJsonArray swordEntries = table["SwordEntries"].toArray();
            QJsonArray shieldEntries = table["ShieldEntries"].toArray();

            QVector<Raid> swordRaids;
            QVector<Raid> shieldRaids;
            for (u8 j = 0; j < 12; j++)
            {
                QJsonObject swordEntry = swordEntries[j].toObject();
                QJsonObject shieldEntry = shieldEntries[j].toObject();

                u8 swordAbility = static_cast<u8>(swordEntry["Ability"].toInt());
                u8 swordAltform = static_cast<u8>(swordEntry["AltForm"].toInt());
                u8 swordIVCount = static_cast<u8>(swordEntry["FlawlessIVs"].toInt());
                u8 swordGender = static_cast<u8>(swordEntry["Gender"].toInt());
                bool swordGigantamax = swordEntry["IsGigantamax"].toBool();
                u16 swordSpecies = static_cast<u16>(swordEntry["Species"].toInt());

                bool swordStar[5];
                for (u8 k = 0; k < 5; k++)
                {
                    swordStar[k] = swordEntry["Stars"].toArray()[k].toBool();
                }

                u8 shieldAbility = static_cast<u8>(shieldEntry["Ability"].toInt());
                u8 shieldAltform = static_cast<u8>(shieldEntry["AltForm"].toInt());
                u8 shieldIVCount = static_cast<u8>(shieldEntry["FlawlessIVs"].toInt());
                u8 shieldGender = static_cast<u8>(shieldEntry["Gender"].toInt());
                bool shieldGigantamax = shieldEntry["IsGigantamax"].toBool();
                u16 shieldSpecies = static_cast<u16>(shieldEntry["Species"].toInt());

                bool shieldStar[5];
                for (u8 k = 0; k < 5; k++)
                {
                    shieldStar[k] = shieldEntry["Stars"].toArray()[k].toBool();
                }

                swordRaids.append(Raid(swordAbility, swordAltform, swordIVCount, swordGender, swordGigantamax, swordSpecies, swordStar));
                shieldRaids.append(
                    Raid(shieldAbility, shieldAltform, shieldIVCount, shieldGender, shieldGigantamax, shieldSpecies, shieldStar));
            }

            u64 hash = table["TableID"].toString().toULongLong(nullptr, 16);
            dens[hash] = Den(swordRaids, shieldRaids);
        }
    }

    // Event Dens
    f.setFileName(QApplication::applicationDirPath() + "/nests_event.json");
    if (f.open(QIODevice::ReadOnly))
    {
        QJsonObject doc(QJsonDocument::fromJson(f.readAll()).object());
        f.close();

        QJsonArray tables = doc["Tables"].toArray();
        QJsonArray swordEntries = tables.at(0).toObject()["Entries"].toArray();
        QJsonArray shieldEntries = tables.at(1).toObject()["Entries"].toArray();

        QVector<Raid> swordRaids;
        QVector<Raid> shieldRaids;
        for (u8 j = 0; j < 30; j++)
        {
            QJsonObject swordEntry = swordEntries[j].toObject();
            QJsonObject shieldEntry = shieldEntries[j].toObject();

            u8 swordAbility = static_cast<u8>(swordEntry["Ability"].toInt());
            u8 swordAltform = static_cast<u8>(swordEntry["AltForm"].toInt());
            u8 swordShinyType = static_cast<u8>(swordEntry["Field_12"].toInt());
            u8 swordIVCount = static_cast<u8>(swordEntry["FlawlessIVs"].toInt());
            u8 swordGender = static_cast<u8>(swordEntry["Gender"].toInt());
            bool swordGigantamax = swordEntry["IsGigantamax"].toBool();
            u16 swordSpecies = static_cast<u16>(swordEntry["Species"].toInt());

            bool swordStar[5];
            for (u8 k = 0; k < 5; k++)
            {
                swordStar[k] = swordEntry["Probabilities"].toArray()[k].toInt() != 0;
            }

            u8 shieldAbility = static_cast<u8>(shieldEntry["Ability"].toInt());
            u8 shieldAltform = static_cast<u8>(shieldEntry["AltForm"].toInt());
            u8 shieldShinyType = static_cast<u8>(swordEntry["Field_12"].toInt());
            u8 shieldIVCount = static_cast<u8>(shieldEntry["FlawlessIVs"].toInt());
            u8 shieldGender = static_cast<u8>(shieldEntry["Gender"].toInt());
            bool shieldGigantamax = shieldEntry["IsGigantamax"].toBool();
            u16 shieldSpecies = static_cast<u16>(shieldEntry["Species"].toInt());

            bool shieldStar[5];
            for (u8 k = 0; k < 5; k++)
            {
                shieldStar[k] = shieldEntry["Probabilities"].toArray()[k].toInt() != 0;
            }

            swordRaids.append(
                Raid(swordAbility, swordAltform, swordIVCount, swordGender, swordGigantamax, swordSpecies, swordStar, swordShinyType));
            shieldRaids.append(Raid(shieldAbility, shieldAltform, shieldIVCount, shieldGender, shieldGigantamax, shieldSpecies, shieldStar,
                                    shieldShinyType));
        }
        dens[0x17e59bbd874fd95c] = Den(swordRaids, shieldRaids);
    }
}

Den DenLoader::getDen(u8 index, u8 rarity)
{
    u64 hash;
    if (index == 100)
    {
        hash = 0x17e59bbd874fd95c;
    }
    else
    {
        hash = denInfo[index][rarity];
    }

    return dens[hash];
}

u8 DenLoader::getLocation(u8 index)
{
    return static_cast<u8>(denInfo[index][2]);
}

QVector<u16> DenLoader::getCoordinates(u8 index)
{
    return { static_cast<u16>(denInfo[index][3]), static_cast<u16>(denInfo[index][4]) };
}
