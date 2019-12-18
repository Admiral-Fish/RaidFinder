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
#include <QTextStream>

static QStringList locations;
static QStringList natures;
static QStringList species;

QStringList readFile(QString name)
{
    QFile file(name);

    QStringList input;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        while (!ts.atEnd())
        {
            input << ts.readLine();
        }
        file.close();
    }
    return input;
}

void Translator::init(QString locale)
{
    locations = readFile(QString(":/text/locations_%1.txt").arg(locale));
    natures = readFile(QString(":/text/natures_%1.txt").arg(locale));
    species = readFile(QString(":/text/species_%1.txt").arg(locale));
}

QStringList Translator::getLocations()
{
    return locations;
}

QString Translator::getLocation(u8 location)
{
    return locations.at(location);
}

QStringList Translator::getSpecies()
{
    return species;
}

QString Translator::getSpecie(u16 specie)
{
    return species.at(specie - 1);
}

QStringList Translator::getNatures()
{
    return natures;
}

QString Translator::getNature(u8 nature)
{
    return natures.at(nature);
}
