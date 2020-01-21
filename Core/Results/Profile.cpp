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

#include "Profile.hpp"
#include <QCoreApplication>

Profile::Profile() : name("None"), tid(12345), sid(54321), version(Game::Sword)
{
}

Profile::Profile(const QString &name, u16 tid, u16 sid, Game version) : name(name), tid(tid), sid(sid), version(version)
{
}

QString Profile::getName() const
{
    return name;
}

u16 Profile::getTID() const
{
    return tid;
}

u16 Profile::getSID() const
{
    return sid;
}

u16 Profile::getTSV() const
{
    return (tid ^ sid) >> 4;
}

Game Profile::getVersion() const
{
    return version;
}

QString Profile::getVersionString() const
{
    switch (version)
    {
    case Game::Sword:
        return QCoreApplication::tr("Sword");
    case Game::Shield:
        return QCoreApplication::tr("Shield");
    }
    return "-";
}

bool operator==(const Profile &left, const Profile &right)
{
    return left.getName() == right.getName() && left.getTID() == right.getTID() && left.getSID() == right.getSID()
        && left.getVersion() == right.getVersion();
}

bool operator!=(const Profile &left, const Profile &right)
{
    return !(left == right);
}
