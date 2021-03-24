import glob
import json
import os


def embed_encounters():
    file = "Resources/Encounters/nests.json"
    with open(file, "r") as f:
        j = json.loads(f.read())

    name = os.path.basename(f.name).replace(".json", "")

    size = 0
    data = ""
    for table in j["Tables"]:
        for raid in table["ShieldEntries"]:
            data += f"{raid['Ability']},"
            data += f"{raid['AltForm']},"
            data += f"{raid['FlawlessIVs']},"
            data += f"{raid['Gender']},"
            data += f"{1 if raid['IsGigantamax'] else 0},"
            data += f"{raid['Species'] >> 8},"
            data += f"{raid['Species'] & 0xff},"
            i = 0
            while i < 5:
                data += f"{raid['Stars'][i]},"
                i += 1
            size += 12

        for raid in table["SwordEntries"]:
            data += f"{raid['Ability']},"
            data += f"{raid['AltForm']},"
            data += f"{raid['FlawlessIVs']},"
            data += f"{raid['Gender']},"
            data += f"{1 if raid['IsGigantamax'] else 0},"
            data += f"{raid['Species'] >> 8},"
            data += f"{raid['Species'] & 0xff},"
            i = 0
            while i < 5:
                data += f"{raid['Stars'][i]},"
                i += 1
            size += 12

        table_id = int(table["TableID"], 16)
        for i in range(8):
            num = (table_id >> ((7 - i) * 8)) & 0xff
            data += f"{num},"
        size += 8

    string = f"constexpr std::array<u8, {size}> {name} = "
    string += " { "
    string += data[:-1]
    string += " };"

    return string


def embed_personal():
    file = "Resources/Personal/personal_swsh"
    with open(file, "rb") as f:
        data = f.read()

    name = os.path.basename(f.name)

    string = f"constexpr std::array<u8, {len(data)}> {name} = "
    string += " { "

    for i in range(len(data)):
        string += str(data[i])
        if i != len(data) - 1:
            string += ","

    string += " };"
    return string


def write_data(arrays):
    f = open("Core\Resources.hpp", "w")

    f.write("#include <Core/Util/Global.hpp>\n")
    f.write("#include <array>\n\n")

    for array in arrays:
        f.write(f"{array}\n\n")


def embed_i18n(paths):
    arrays = []
    for path in paths:
        mapping = {}

        files = glob.glob(f"{path}/*.txt")
        for file in files:
            with open(file, "r", encoding="utf-8") as f:
                data = f.read().split("\n")

            name = os.path.basename(f.name).replace(".txt", "")

            string_data = []
            for line in data:
                line = line.replace('\r', '').replace('\n', '')

                for x in [int(y) for y in bytearray(line, encoding="utf-8")]:
                    string_data.append(x)
                string_data.append(0)

            string = f"constexpr std::array<u8, {len(string_data)}> {name} = "
            string += " { "

            for i in range(len(string_data)):
                string += str(string_data[i])
                if i != len(string_data) - 1:
                    string += ", "

            string += " };"
            mapping[name] = string

        arrays.append(mapping)

    return arrays


def write_i18n(maps):
    f = open("Core\I18n.hpp", "w", encoding="utf-8")

    f.write("#include <Core/Util/Global.hpp>\n")
    f.write("#include <array>\n\n")

    for map in maps:
        for string in map.values():
            f.write(f"{string}\n\n")


def main():
    encounters = embed_encounters()
    personal = embed_personal()
    write_data([encounters, personal])

    maps = embed_i18n(["Resources\Text\de", "Resources\Text\en", "Resources\Text\es", "Resources\Text\\fr", "Resources\Text\it",
                       "Resources\Text\ja", "Resources\Text\ko", "Resources\Text\pt", "Resources\Text\\tw", "Resources\Text\zh", ])
    write_i18n(maps)


if __name__ == "__main__":
    main()
