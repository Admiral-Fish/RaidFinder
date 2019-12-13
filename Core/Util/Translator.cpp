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

#include "Translator.hpp"
#include <QFile>
#include <QSettings>
#include <QTextStream>

QStringList Translator::getSpecies(const QVector<u16> &nums)
{
    QStringList species;

    QSettings setting;
    QFile file(QString(":/text/species_%1.txt").arg(setting.value("settings/locale", "en").toString()));

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        QStringList input;
        while (!ts.atEnd())
        {
            input << ts.readLine();
        }
        file.close();

        for (const u16 &x : nums)
        {
            species.append(input.at(x - 1));
        }
    }

    return species;
}
