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

#include "ProfileLoader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

namespace
{
    std::string path;

    Profile getProfile(const nlohmann::json &j)
    {
        Profile profile(j["name"].get<std::string>(), j["tid"].get<u16>(), j["sid"].get<u16>(), j["version"].get<Game>());
        return profile;
    }

    nlohmann::json getJson(const Profile &profile)
    {
        nlohmann::json data;
        data["name"] = profile.getName();
        data["tid"] = profile.getTID();
        data["sid"] = profile.getSID();
        data["version"] = profile.getVersion();
        return data;
    }

    nlohmann::json readJson()
    {
        nlohmann::json j;

        std::ifstream read(path);
        if (read.is_open())
        {
            j = nlohmann::json::parse(read, nullptr, false);
        }

        return j.is_discarded() ? nlohmann::json() : j;
    }

    void writeJson(const nlohmann::json &j)
    {
        std::ofstream write(path);
        write << j.dump();
        write.close();
    }
}

void ProfileLoader::init(const std::string &location)
{
    path = location;
}

std::vector<Profile> ProfileLoader::getProfiles()
{
    std::vector<Profile> profiles;

    nlohmann::json j = readJson();
    const auto &gen8 = j["gen8"];
    std::transform(gen8.begin(), gen8.end(), std::back_inserter(profiles), [](const nlohmann::json &j) { return getProfile(j); });

    return profiles;
}

void ProfileLoader::addProfile(const Profile &profile)
{
    nlohmann::json j = readJson();

    auto &gen8 = j["gen8"];
    gen8.emplace_back(getJson(profile));

    writeJson(j);
}

void ProfileLoader::removeProfile(const Profile &remove)
{
    nlohmann::json j = readJson();

    auto &gen8 = j["gen8"];
    for (size_t i = 0; i < gen8.size(); i++)
    {
        Profile profile = getProfile(gen8[i]);

        if (profile == remove)
        {
            gen8.erase(gen8.begin() + i);

            writeJson(j);
            break;
        }
    }
}

void ProfileLoader::updateProfile(const Profile &update, const Profile &original)
{
    nlohmann::json j = readJson();

    auto &gen8 = j["gen8"];
    for (auto &i : gen8)
    {
        Profile profile = getProfile(i);

        if (original == profile && original != update)
        {
            i = getJson(update);

            writeJson(j);
            break;
        }
    }
}
