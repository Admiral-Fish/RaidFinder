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

#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <Core/Util/Global.hpp>
#include <string>
#include <vector>

namespace Translator
{
    void init(const std::string &locale);
    std::string getAbility(u16 ability);
    std::string getCharacteristic(u8 characteristic);
    std::string getLocation(u8 location);
    std::vector<std::string> getNatures();
    std::string getNature(u8 nature);
    std::string getSpecie(u16 specie);
}

#endif // TRANSLATOR_HPP
