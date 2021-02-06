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

#include "Translator.hpp"
#include <Core/I18n.hpp>

namespace
{
    std::string language;
    std::vector<std::string> abilities;
    std::vector<std::string> characteristics;
    std::vector<std::string> locations;
    std::vector<std::string> natures;
    std::vector<std::string> species;

    std::vector<std::string> readFile(const std::string &name)
    {
        const u8 *data = nullptr;
        size_t size = 0;

        if (language == "de")
        {
            if (name == "abilities")
            {
                data = abilities_de.data();
                size = abilities_de.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_de.data();
                size = characteristics_de.size();
            }
            else if (name == "locations")
            {
                data = locations_de.data();
                size = locations_de.size();
            }
            else if (name == "natures")
            {
                data = natures_de.data();
                size = natures_de.size();
            }
            else if (name == "species")
            {
                data = species_de.data();
                size = species_de.size();
            }
        }
        else if (language == "en")
        {
            if (name == "abilities")
            {
                data = abilities_en.data();
                size = abilities_en.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_en.data();
                size = characteristics_en.size();
            }
            else if (name == "locations")
            {
                data = locations_en.data();
                size = locations_en.size();
            }
            else if (name == "natures")
            {
                data = natures_en.data();
                size = natures_en.size();
            }
            else if (name == "species")
            {
                data = species_en.data();
                size = species_en.size();
            }
        }
        else if (language == "es")
        {
            if (name == "abilities")
            {
                data = abilities_es.data();
                size = abilities_es.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_es.data();
                size = characteristics_es.size();
            }
            else if (name == "locations")
            {
                data = locations_es.data();
                size = locations_es.size();
            }
            else if (name == "natures")
            {
                data = natures_es.data();
                size = natures_es.size();
            }
            else if (name == "species")
            {
                data = species_es.data();
                size = species_es.size();
            }
        }
        else if (language == "fr")
        {
            if (name == "abilities")
            {
                data = abilities_fr.data();
                size = abilities_fr.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_fr.data();
                size = characteristics_fr.size();
            }
            else if (name == "locations")
            {
                data = locations_fr.data();
                size = locations_fr.size();
            }
            else if (name == "natures")
            {
                data = natures_fr.data();
                size = natures_fr.size();
            }
            else if (name == "species")
            {
                data = species_fr.data();
                size = species_fr.size();
            }
        }
        else if (language == "it")
        {
            if (name == "abilities")
            {
                data = abilities_it.data();
                size = abilities_it.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_it.data();
                size = characteristics_it.size();
            }
            else if (name == "locations")
            {
                data = locations_it.data();
                size = locations_it.size();
            }
            else if (name == "natures")
            {
                data = natures_it.data();
                size = natures_it.size();
            }
            else if (name == "species")
            {
                data = species_it.data();
                size = species_it.size();
            }
        }
        else if (language == "ja")
        {
            if (name == "abilities")
            {
                data = abilities_ja.data();
                size = abilities_ja.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_ja.data();
                size = characteristics_ja.size();
            }
            else if (name == "locations")
            {
                data = locations_ja.data();
                size = locations_ja.size();
            }
            else if (name == "natures")
            {
                data = natures_ja.data();
                size = natures_ja.size();
            }
            else if (name == "species")
            {
                data = species_ja.data();
                size = species_ja.size();
            }
        }
        else if (language == "ko")
        {
            if (name == "abilities")
            {
                data = abilities_ko.data();
                size = abilities_ko.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_ko.data();
                size = characteristics_ko.size();
            }
            else if (name == "locations")
            {
                data = locations_ko.data();
                size = locations_ko.size();
            }
            else if (name == "natures")
            {
                data = natures_ko.data();
                size = natures_ko.size();
            }
            else if (name == "species")
            {
                data = species_ko.data();
                size = species_ko.size();
            }
        }
        else if (language == "pt")
        {
            if (name == "abilities")
            {
                data = abilities_pt.data();
                size = abilities_pt.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_pt.data();
                size = characteristics_pt.size();
            }
            else if (name == "locations")
            {
                data = locations_pt.data();
                size = locations_pt.size();
            }
            else if (name == "natures")
            {
                data = natures_pt.data();
                size = natures_pt.size();
            }
            else if (name == "species")
            {
                data = species_pt.data();
                size = species_pt.size();
            }
        }
        else if (language == "tw")
        {
            if (name == "abilities")
            {
                data = abilities_tw.data();
                size = abilities_tw.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_tw.data();
                size = characteristics_tw.size();
            }
            else if (name == "locations")
            {
                data = locations_tw.data();
                size = locations_tw.size();
            }
            else if (name == "natures")
            {
                data = natures_tw.data();
                size = natures_tw.size();
            }
            else if (name == "species")
            {
                data = species_tw.data();
                size = species_tw.size();
            }
        }
        else if (language == "zh")
        {
            if (name == "abilities")
            {
                data = abilities_zh.data();
                size = abilities_zh.size();
            }
            else if (name == "characteristics")
            {
                data = characteristics_zh.data();
                size = characteristics_zh.size();
            }
            else if (name == "locations")
            {
                data = locations_zh.data();
                size = locations_zh.size();
            }
            else if (name == "natures")
            {
                data = natures_zh.data();
                size = natures_zh.size();
            }
            else if (name == "species")
            {
                data = species_zh.data();
                size = species_zh.size();
            }
        }

        std::vector<std::string> strings;
        size_t start = 0;
        for (size_t i = 0; i < size; i++)
        {
            if (data[i] == 0)
            {
                strings.emplace_back((const char *)&data[start], i - start);
                start = i + 1;
            }
        }

        return strings;
    }
}

namespace Translator
{
    void init(const std::string &locale)
    {
        language = locale;
        abilities = readFile("abilities");
        characteristics = readFile("characteristics");
        locations = readFile("locations");
        natures = readFile("natures");
        species = readFile("species");
    }

    std::string getAbility(u16 ability)
    {
        return abilities[ability];
    }

    std::string getCharacteristic(u8 characteristic)
    {
        return characteristics[characteristic];
    }

    std::string getLocation(u8 location)
    {
        return locations[location];
    }

    std::string getSpecie(u16 specie)
    {
        return species[specie - 1];
    }

    std::vector<std::string> getNatures()
    {
        return natures;
    }

    std::string getNature(u8 nature)
    {
        return natures[nature];
    }
}
