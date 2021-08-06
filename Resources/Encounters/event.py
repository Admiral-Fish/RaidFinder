import glob
import json
import zlib

if __name__ == "__main__":
    # Look for and compress all .json files
    for file in glob.glob("*.json"):
        # Skip nests.json if running in the same folder
        if file == "nests.json":
            continue

        j = None
        with open(file, "r") as f:
            j = json.load(f)

        j = json.dumps(j, separators=(',', ':')).encode("utf-8")
        size = len(j).to_bytes(4, "big")

        compress = zlib.compress(j, level=-1)
        with open(file.replace(".json", ".bin"), "wb") as f:
            f.write(size)
            f.write(compress)