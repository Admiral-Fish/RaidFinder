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

#include "Profile.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QVector>

Profile::Profile()
{
    name = "None";
    tid = 12345;
    sid = 54321;
    version = Game::Sword;
}

Profile::Profile(const QString &name, u16 tid, u16 sid, Game version)
{
    this->name = name;
    this->tid = tid;
    this->sid = sid;
    this->version = version;
}

Profile::Profile(QJsonObject data)
{
    name = data["name"].toString();
    tid = data["tid"].toInt();
    sid = data["sid"].toInt();
    version = static_cast<Game>(data["version"].toInt());
}

QJsonObject Profile::getJson()
{
    QJsonObject data;
    data["name"] = name;
    data["tid"] = tid;
    data["sid"] = sid;
    data["version"] = version;
    return data;
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
        return "Sword";
    case Game::Shield:
        return "Shield";
    }
    return "-";
}

QVector<Profile> Profile::loadProfileList()
{
    QVector<Profile> profileList;
    QSettings setting;

    QJsonObject profiles(QJsonDocument::fromJson(setting.value("profiles").toByteArray()).object());
    QJsonArray gen8 = profiles["gen8"].toArray();

    for (const auto &&i : gen8)
    {
        auto data = i.toObject();
        profileList.append(Profile(data));
    }

    return profileList;
}

void Profile::saveProfile()
{
    QSettings setting;

    QJsonObject profiles(QJsonDocument::fromJson(setting.value("profiles").toByteArray()).object());
    QJsonArray gen8 = profiles["gen8"].toArray();

    gen8.append(getJson());
    profiles["gen8"] = gen8;

    setting.setValue("profiles", QJsonDocument(profiles).toJson());
}

void Profile::deleteProfile()
{
    QSettings setting;

    QJsonObject profiles(QJsonDocument::fromJson(setting.value("profiles").toByteArray()).object());
    QJsonArray gen8 = profiles["gen8"].toArray();

    for (u8 i = 0; i < gen8.size(); i++)
    {
        Profile profile(gen8[i].toObject());

        if (profile == *this)
        {
            gen8.removeAt(i);
            profiles["gen8"] = gen8;

            setting.setValue("profiles", QJsonDocument(profiles).toJson());
            break;
        }
    }
}

void Profile::updateProfile(const Profile &original)
{
    QSettings setting;

    QJsonObject profiles = QJsonDocument::fromJson(setting.value("profiles").toByteArray()).object();
    QJsonArray gen8 = profiles["gen8"].toArray();

    for (auto &&i : gen8)
    {
        Profile profile(i.toObject());

        if (original == profile && original != *this)
        {
            i = getJson();
            profiles["gen8"] = gen8;

            setting.setValue("profiles", QJsonDocument(profiles).toJson());
            break;
        }
    }
}

bool operator==(const Profile &left, const Profile &right)
{
    return left.name == right.name && left.tid == right.tid && left.sid == right.sid && left.version == right.version;
}

bool operator!=(const Profile &left, const Profile &right)
{
    return !(left == right);
}
