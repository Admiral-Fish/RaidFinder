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

#include "PersonalLoader.hpp"
#include <QFile>
#include <QVector>

namespace
{
    QVector<PersonalInfo> info;
}

namespace PersonalLoader
{
    void init()
    {
        QFile file(":/personal/personal_swsh");
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray data = file.readAll();
            file.close();

            for (auto i = 0; i < data.size(); i += 168)
            {
                u8 hp = static_cast<u8>(data[i]);
                u8 atk = static_cast<u8>(data[i + 1]);
                u8 def = static_cast<u8>(data[i + 2]);
                u8 spe = static_cast<u8>(data[i + 3]);
                u8 spa = static_cast<u8>(data[i + 4]);
                u8 spd = static_cast<u8>(data[i + 5]);
                u8 genderRatio = static_cast<u8>(data[i + 18]);
                u16 ability1 = (static_cast<u8>(data[i + 25]) << 8) | static_cast<u8>(data[i + 24]);
                u16 ability2 = (static_cast<u8>(data[i + 27]) << 8) | static_cast<u8>(data[i + 26]);
                u16 abilityH = (static_cast<u8>(data[i + 29]) << 8) | static_cast<u8>(data[i + 28]);
                u16 formStatIndex = (static_cast<u8>(data[i + 31]) << 8) | static_cast<u8>(data[i + 30]);
                u8 formCount = static_cast<u8>(data[i + 32]);
                bool included = (static_cast<u8>(data[i + 33]) >> 6) & 1;

                info.append(PersonalInfo(hp, atk, def, spa, spd, spe, genderRatio, ability1, ability2, abilityH, formCount, formStatIndex,
                                         included));
            }
        }
    }

    PersonalInfo getInfo(u16 species, u8 form)
    {
        PersonalInfo base = info.at(species);

        u16 formIndex = base.getFormStatIndex();

        if (form == 0 || formIndex == 0)
        {
            return base;
        }

        return info.at(formIndex + form - 1);
    }
}
