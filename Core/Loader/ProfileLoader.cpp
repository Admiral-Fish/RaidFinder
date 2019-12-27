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

#include "ProfileLoader.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace
{
    Profile getProfile(const QJsonObject &object)
    {
        Profile profile(object["name"].toString(), static_cast<u16>(object["tid"].toInt()), static_cast<u16>(object["sid"].toInt()), static_cast<Game>(object["version"].toInt()));
        return profile;
    }

    QJsonObject getJson(const Profile &profile)
    {
        QJsonObject data;
        data["name"] = profile.getName();
        data["tid"] = profile.getTID();
        data["sid"] = profile.getSID();
        data["version"] = profile.getVersion();
        return data;
    }
}

QVector<Profile> ProfileLoader::getProfiles()
{
    QVector<Profile> profileList;
    QSettings setting;

    QJsonObject profiles(QJsonDocument::fromJson(setting.value("profiles").toByteArray()).object());
    QJsonArray gen8 = profiles["gen8"].toArray();

    for (auto i = 0; i < gen8.size(); i++)
    {
        auto data = gen8[i].toObject();
        profileList.append(getProfile(data));
    }

    return profileList;
}

void ProfileLoader::addProfile(const Profile &profile)
{
    QSettings setting;

    QJsonObject profiles(QJsonDocument::fromJson(setting.value("profiles").toByteArray()).object());
    QJsonArray gen8 = profiles["gen8"].toArray();

    gen8.append(getJson(profile));
    profiles["gen8"] = gen8;

    setting.setValue("profiles", QJsonDocument(profiles).toJson());
}

void ProfileLoader::removeProfile(const Profile &remove)
{
    QSettings setting;

    QJsonObject profiles(QJsonDocument::fromJson(setting.value("profiles").toByteArray()).object());
    QJsonArray gen8 = profiles["gen8"].toArray();

    for (auto i = 0; i < gen8.size(); i++)
    {
        Profile profile = getProfile(gen8[i].toObject());

        if (profile == remove)
        {
            gen8.removeAt(i);
            profiles["gen8"] = gen8;

            setting.setValue("profiles", QJsonDocument(profiles).toJson());
            break;
        }
    }
}

void ProfileLoader::updateProfile(const Profile &update, const Profile &original)
{
    QSettings setting;

    QJsonObject profiles = QJsonDocument::fromJson(setting.value("profiles").toByteArray()).object();
    QJsonArray gen8 = profiles["gen8"].toArray();

    for (auto i = 0; i < gen8.size(); i++)
    {
        Profile profile = getProfile(gen8[i].toObject());

        if (original == profile && original != update)
        {
            gen8[i] = getJson(update);
            profiles["gen8"] = gen8;

            setting.setValue("profiles", QJsonDocument(profiles).toJson());
            break;
        }
    }
}
