import struct
import sys
import z3

class PK8:
    def __init__(self, data):
        self.data = data

    def getEC(self):
        return struct.unpack("<I", self.data[0x0:0x4])[0]

    def getTID(self):
        return struct.unpack("<H", self.data[0xC:0xE])[0]

    def getSID(self):
        return struct.unpack("<H", self.data[0xE:0x10])[0]

    def getPID(self):
        return struct.unpack("<I", self.data[0x1C:0x20])[0]

    def getIV(self):
        return struct.unpack("<I", self.data[0x8C:0x90])[0]

    def getHP(self):
        return self.getIV() & 0x1F

    def getAtk(self):
        return (self.getIV() >> 5) & 0x1F

    def getDef(self):
        return (self.getIV() >> 10) & 0x1F

    def getSpA(self):
        return (self.getIV() >> 20) & 0x1F

    def getSpD(self):
        return (self.getIV() >> 25) & 0x1F

    def getSpe(self):
        return (self.getIV() >> 15) & 0x1F

class XoroShiro:
    def __init__(self, seed):
        self.s0 = seed
        self.s1 = 0x82A2B175229D6A5B

    def rotl(self, x, k):
        return ((x << k) | (x >> (64 - k))) & 0xFFFFFFFFFFFFFFFF

    def next(self):
        result = (self.s0 + self.s1) & 0xFFFFFFFFFFFFFFFF

        self.s1 ^= self.s0
        self.s0 = self.rotl(self.s0, 24) ^ self.s1 ^ ((self.s1 << 16) & 0xFFFFFFFFFFFFFFFF)
        self.s1 = self.rotl(self.s1, 37)

        return result
        
    def nextInt(self, value, mask):
        result = self.next() & mask
        while result >= value:
            result = self.next() & mask
        return result

def get_shiny_type(tid, sid, pid):
    tsv = tid ^ sid
    psv = (pid >> 16) ^ (pid & 0xffff)

    if tsv == psv:
        return 2 # Square
    elif (tsv ^ psv) < 16:
        return 1 # Star
    else:
        return 0 # Not shiny

def sym_xoroshiro128plus(sym_s0, sym_s1):
    sym_r = (sym_s0 + sym_s1) & 0xFFFFFFFF

    sym_s1 ^= sym_s0
    sym_s0 = z3.RotateLeft(sym_s0, 24) ^ sym_s1 ^ ((sym_s1 << 16) & 0xFFFFFFFFFFFFFFFF)
    sym_s1 = z3.RotateLeft(sym_s1, 37)

    return sym_s0, sym_s1, sym_r

def get_results(s):
    result = []
    while s.check() == z3.sat:
        m = s.model()
        result.append(m)
        
        # Constraint that makes current answer invalid
        d = m[0]
        c = d()
        s.add(c != m[d])

    return result

def find_origin_seeds(ec, pid, tid, sid, shiny):
    solver = z3.Solver()
    start_s0 = z3.BitVecs('start_s0', 64)[0]

    sym_s0 = start_s0
    sym_s1 = 0x82A2B175229D6A5B

    # EC call
    sym_s0, sym_s1, sym_ec = sym_xoroshiro128plus(sym_s0, sym_s1)

    # TID/SID call
    sym_s0, sym_s1, sym_sidtid = sym_xoroshiro128plus(sym_s0, sym_s1)

    # PID call
    sym_s0, sym_s1, sym_pid = sym_xoroshiro128plus(sym_s0, sym_s1)

    # Validate EC
    solver.add(sym_ec == ec)

    # Validate pid type
    sym_shiny = (sym_sidtid >> 16) ^ (sym_sidtid & 0xffff) ^ (sym_pid >> 16) ^ (sym_pid & 0xffff)
    if shiny == 0:
        solver.add(sym_shiny >= 16)
    elif shiny == 1:
        solver.add(sym_shiny < 16)
    else:
        solver.add(sym_shiny == 0)

    # Validate PID
    if shiny != 0:
        high = (sym_pid & 0xffff) ^ tid ^ sid ^ (2 - shiny)
        sym_pid = (high << 16) | (sym_pid & 0xffff)
    solver.add(sym_pid == pid)

    return [ result[start_s0].as_long() for result in get_results(solver) ]

def find_valid_seeds(seeds, ivs):
    results = []

    for seed in seeds:
        for iv_count in range(1, 6):
            rng = XoroShiro(seed)

            # ec, tid/sid, pid
            for i in range(3):
                rng.nextInt(0xffffffff, 0xffffffff)

            check_ivs = [None]*6
            count = 0
            while count < iv_count:
                stat = rng.nextInt(6, 7)
                if check_ivs[stat] is None:
                    check_ivs[stat] = 31
                    count += 1

            for i in range(6):
                if check_ivs[i] is None:
                    check_ivs[i] = rng.nextInt(32, 31)

            if ivs == check_ivs:
                results.append((seed, iv_count))

    return results

def search(pkm):
    ec = pkm.getEC()
    pid = pkm.getPID()
    tid = pkm.getTID()
    sid = pkm.getSID()
    ivs = [ pkm.getHP(), pkm.getAtk(), pkm.getDef(), pkm.getSpA(), pkm.getSpD(), pkm.getSpe() ]

    for flag in [ False, True ]:
        # !flag: checks normal and shiny
        # flag: checks shiny locked        
        
        if flag:
            pid ^= 0x10000000
        
        shiny = get_shiny_type(tid, sid, pid)

        origin_seeds = find_origin_seeds(ec, pid, tid, sid, shiny)
        if len(origin_seeds) > 0:
            valid_seeds = find_valid_seeds(origin_seeds, ivs)
            if len(valid_seeds) > 0:
                for seed, iv_count in valid_seeds:
                    print(f"Raid seed: {hex(seed)} with IV count of {iv_count}")
                return True

    return False

def main():
    if len(sys.argv) > 1:
        file_name = sys.argv[1]
        with open(file_name, "rb") as f:
            data = f.read()
        pkm = PK8(data)

        return search(pkm)
    else:
        print("You must provide a pk8 file")
        return False

if __name__ == "__main__":
    if main() == False:
        print("No raid seed")
    
    input("\nPress ENTER to exit")
