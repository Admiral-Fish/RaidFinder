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

#include "DenLoader.hpp"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

static const QVector<QPair<u64, u64>> tableHashes = {
    { 0x173f0456dc5dfc52, 0xba83e1671012ebcd }, // 16 52
    { 0x17458556dc634333, 0xba8745671015cb90 }, // 37 64
    { 0x17458b56dc634d65, 0x450421d99cf882c1 }, // 31 90
    { 0x17428156dc610690, 0xba805467100fc65f }, // 3 51
    { 0x17428856dc611275, 0xba805767100fcb78 }, // 4 46
    { 0x17458956dc6349ff, 0xba8747671015cef6 }, // 33 62
    { 0x17459356dc635afd, 0xba8746671015cd43 }, // 39 65
    { 0x17428356dc6109f6, 0xba805967100fcede }, // 1 48
    { 0x17458b56dc634d65, 0x450421d99cf882c1 }, // 31 90
    { 0x17491656dc666f6d, 0xba83da671012dfe8 }, // 26 59
    { 0x17491656dc666f6d, 0x17491556dc666dba }, // 26 58
    { 0x17490856dc6657a3, 0x17491556dc666dba }, // 28 79
    { 0x17428856dc611275, 0x45041fd99cf87f5b }, // 4 92
    { 0x17491656dc666f6d, 0xba83da671012dfe8 }, // 26 59
    { 0x17428256dc610843, 0xba805367100fc4ac }, // 2 50
    { 0x17428656dc610f0f, 0xba805867100fcd2b }, // 6 47
    { 0x17428556dc610d5c, 0xba805d67100fd5aa }, // 7 44
    { 0x17459356dc635afd, 0xba8746671015cd43 }, // 39 65
    { 0x17458b56dc634d65, 0xba83d8671012dc82 }, // 31 61
    { 0x17458b56dc634d65, 0xba83d8671012dc82 }, // 31 61
    { 0x17428b56dc61178e, 0x17428556dc610d5c }, // 9 75
    { 0x17428b56dc61178e, 0x17428556dc610d5c }, // 9 75
    { 0xba805d67100fd5aa, 0x17501a56dc6c94e7 }, // 44 42
    { 0x17428556dc610d5c, 0xba805d67100fd5aa }, // 7 44
    { 0x17428b56dc61178e, 0x450420d99cf8810e }, // 9 91
    { 0x17458756dc634699, 0x17458456dc634180 }, // 35 63
    { 0x17459256dc63594a, 0xba8745671015cb90 }, // 38 64
    { 0x17428c56dc611941, 0x450420d99cf8810e }, // 8 91
    { 0x17501b56dc6c969a, 0xba8a496710180833 }, // 41 76
    { 0x17501b56dc6c969a, 0xba8a496710180833 }, // 41 76
    { 0x17428756dc6110c2, 0xba805767100fcb78 }, // 5 46
    { 0x17428356dc6109f6, 0xba805c67100fd3f7 }, // 1 43
    { 0x173f0356dc5dfa9f, 0xba805467100fc65f }, // 15 51
    { 0x173f0056dc5df586, 0xba805e67100fd75d }, // 12 45
    { 0x173eff56dc5df3d3, 0xba805a67100fd091 }, // 11 49
    { 0x173f0356dc5dfa9f, 0x45009ed99cf56e51 }, // 15 83
    { 0x173eff56dc5df3d3, 0x450097d99cf5626c }, // 11 84
    { 0x173efe56dc5df220, 0xba805967100fcede }, // 10 48
    { 0x17501b56dc6c969a, 0xba8a496710180833 }, // 41 76
    { 0x17490756dc6655f0, 0xba83d9671012de35 }, // 29 60
    { 0x17501b56dc6c969a, 0xba8a496710180833 }, // 41 76
    { 0x17490756dc6655f0, 0xba83d9671012de35 }, // 29 60
    { 0x17490756dc6655f0, 0x450425d99cf8898d }, // 29 86
    { 0x173efe56dc5df220, 0xba805967100fcede }, // 10 48
    { 0x173f0256dc5df8ec, 0xba805367100fc4ac }, // 14 50
    { 0x17458a56dc634bb2, 0xba83d9671012de35 }, // 30 60
    { 0x17491456dc666c07, 0x17491356dc666a54 }, // 24 56
    { 0x17501c56dc6c984d, 0xba8746671015cd43 }, // 40 65
    { 0x17458656dc6344e6, 0x45009dd99cf56c9e }, // 34 82
    { 0x173f0156dc5df739, 0x450424d99cf887da }, // 13 87
    { 0x17428c56dc611941, 0xba805d67100fd5aa }, // 8 44
    { 0x173f0456dc5dfc52, 0xba83e1671012ebcd }, // 16 52
    { 0x17501c56dc6c984d, 0xba8746671015cd43 }, // 40 65
    { 0x17428856dc611275, 0x45041fd99cf87f5b }, // 4 92
    { 0x17428756dc6110c2, 0xba805767100fcb78 }, // 5 46
    { 0x17458456dc634180, 0x17458456dc634180 }, // 36 63
    { 0x17491556dc666dba, 0xba83da671012dfe8 }, // 27 59
    { 0x17501c56dc6c984d, 0xba874b671015d5c2 }, // 40 66
    { 0x17428356dc6109f6, 0x45009cd99cf56aeb }, // 1 81
    { 0x17428156dc610690, 0xba805467100fc65f }, // 3 51
    { 0x173f0756dc5e016b, 0x45009bd99cf56938 }, // 19 80
    { 0x17428656dc610f0f, 0xba805c67100fd3f7 }, // 6 43
    { 0x17491556dc666dba, 0xba83da671012dfe8 }, // 27 59
    { 0x17428256dc610843, 0xba805367100fc4ac }, // 2 50
    { 0x173f0056dc5df586, 0x450098d99cf5641f }, // 12 85
    { 0x17491256dc6668a1, 0xba83de671012e6b4 }, // 22 55
    { 0x173f0256dc5df8ec, 0xba805367100fc4ac }, // 14 50
    { 0x173f0656dc5dffb8, 0x173f0756dc5e016b }, // 18 54
    { 0x17458756dc634699, 0x17458456dc634180 }, // 35 63
    { 0x173f0556dc5dfe05, 0x45041ed99cf87da8 }, // 17 93
    { 0x173f0556dc5dfe05, 0xba83e1671012ebcd }, // 17 52
    { 0x173f0356dc5dfa9f, 0x45041ed99cf87da8 }, // 15 93
    { 0x17428b56dc61178e, 0x17428556dc610d5c }, // 9 75
    { 0x173eff56dc5df3d3, 0xba805a67100fd091 }, // 11 49
    { 0x173efe56dc5df220, 0xba805967100fcede }, // 10 48
    { 0x17490f56dc666388, 0xba83de671012e6b4 }, // 21 55
    { 0x173f0656dc5dffb8, 0x173f0756dc5e016b }, // 20 54
    { 0x17490856dc6657a3, 0xba805c67100fd3f7 }, // 28 43
    { 0x17458456dc634180, 0x450423d99cf88627 }, // 36 88
    { 0x17501a56dc6c94e7, 0xba874c671015d775 }, // 42 67
    { 0x17458456dc634180, 0xba874a671015d40f }, // 36 69
    { 0x17491556dc666dba, 0xba874f671015dc8e }, // 27 70
    { 0x17491356dc666a54, 0xba8a4d6710180eff }, // 25 72
    { 0x173f0056dc5df586, 0xba805e67100fd75d }, // 12 45
    { 0x17458856dc63484c, 0xba83d8671012dc82 }, // 32 61
    { 0x17458a56dc634bb2, 0xba83dc671012e34e }, // 30 57
    { 0x173f0756dc5e016b, 0xba83e0671012ea1a }, // 19 53
    { 0x17491156dc6666ee, 0xba8a4c6710180d4c }, // 23 73
    { 0x17458956dc6349ff, 0xba8747671015cef6 }, // 33 62
    { 0x173f0256dc5df8ec, 0xba8749671015d25c }, // 14 68
    { 0x17458a56dc634bb2, 0xba83d9671012de35 }, // 30 60
    { 0x17491156dc6666ee, 0xba83de671012e6b4 }, // 23 55
    { 0x173f0656dc5dffb8, 0xba8750671015de41 }, // 18 71
    { 0x17458856dc63484c, 0x450422d99cf88474 }, // 32 89
    { 0x17491456dc666c07, 0x4500a1d99cf5736a }, // 24 78
    { 0x17491356dc666a54, 0x17491356dc666a54 }, // 25 56
    { 0x173f0656dc5dffb8, 0xba83e0671012ea1a }, // 18 77
    { 0x17428c56dc611941, 0xba805d67100fd5aa }, // 8 44
    { 0x17458656dc6344e6, 0xba8a4f6710181265 } // 34 74
};

QVector<Den> DenLoader::getDens(u8 index, Game version)
{
    QVector<Den> dens;
    QPair<u64, u64> hashes = tableHashes.at(index);

    QFile f(":/encounters/nests.json");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonObject data(QJsonDocument::fromJson(f.readAll()).object());

        QJsonArray tables = data["Tables"].toArray();
        for (auto i = 0; i < tables.size(); i++)
        {
            QJsonObject table = tables[i].toObject();

            u64 hash = table["TableID"].toString().toULongLong();
            Game gameVersion = table["GameVersion"].toInt() == 1 ? Game::Sword : Game::Shield;
            if ((hash == hashes.first || hash == hashes.second) && version == gameVersion)
            {
                QVector<Raid> raids;
                QJsonArray entries = table["Entries"].toArray();
                for (auto j = 0; j < entries.size(); j++)
                {
                    QJsonObject entry = entries[j].toObject();

                    u8 ability = static_cast<u8>(entry["Ability"].toInt());
                    u8 altform = static_cast<u8>(entry["AltForm"].toInt());
                    u8 ivCount = static_cast<u8>(entry["FlawlessIVs"].toInt());
                    u8 gender = static_cast<u8>(entry["Gender"].toInt());
                    u8 genderRatio = static_cast<u8>(entry["GenderRatio"].toInt());
                    bool gigantamax = entry["IsGigantamax"].toBool();
                    u16 species = static_cast<u16>(entry["Species"].toInt());

                    raids.append(Raid(ability, altform, ivCount, gender, genderRatio, gigantamax, species));
                }

                dens.append(Den(raids, gameVersion, hash));
            }
        }
    }

    return dens;
}
