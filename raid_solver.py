import z3

class XoroShiro:
    def __init__(self, seed):
        self.seed = [seed, 0x82A2B175229D6A5B]

    @staticmethod
    def rotl(x, k):
        return ((x << k) | (x >> (64 - k))) & 0xFFFFFFFFFFFFFFFF

    @staticmethod
    def nextP2(x):
        x -= 1
        for i in range(6):
            x |= x >> (1<<i)
        return x

    def next(self):
        s0, s1 = self.seed
        result = (s0 + s1) & 0xFFFFFFFFFFFFFFFF

        s1 ^= s0
        self.seed[0] = self.rotl(s0, 24) ^ s1 ^ ((s1 << 16) & 0xFFFFFFFFFFFFFFFF)
        self.seed[1] = self.rotl(s1, 37)

        return result
        
    def nextInt(self, num = 0xFFFFFFFF):
        num2 = self.nextP2(num)
        s = self.next() & num2
        while s >= num:
            s = self.next() & num2
        return s

def sym_xoroshiro128plus(sym_s0, sym_s1, result):
    sym_r = (sym_s0 + sym_s1) & 0xFFFFFFFFFFFFFFFF	
    condition = (sym_r & 0xFFFFFFFF) == result

    sym_s0, sym_s1 = sym_xoroshiro128plusadvance(sym_s0, sym_s1)

    return sym_s0, sym_s1, condition

def sym_xoroshiro128plusadvance(sym_s0, sym_s1):
    s0 = sym_s0
    s1 = sym_s1
    
    s1 ^= s0
    sym_s0 = z3.RotateLeft(s0, 24) ^ s1 ^ ((s1 << 16) & 0xFFFFFFFFFFFFFFFF)
    sym_s1 = z3.RotateLeft(s1, 37)

    return sym_s0, sym_s1

def get_models(s):
    result = []
    while s.check() == z3.sat:
        m = s.model()
        result.append(m)
        
        # Constraint that makes current answer invalid
        d = m[0]
        c = d()
        s.add(c != m[d])

    return result

def find_seeds(ec, pid):
    solver = z3.Solver()
    start_s0 = z3.BitVecs('start_s0', 64)[0]

    sym_s0 = start_s0
    sym_s1 = 0x82A2B175229D6A5B

    # EC call
    sym_s0, sym_s1, condition = sym_xoroshiro128plus(sym_s0, sym_s1, ec)
    solver.add(condition)

    # TID/SID call
    sym_s0, sym_s1 = sym_xoroshiro128plusadvance(sym_s0, sym_s1)

    # PID call
    sym_s0, sym_s1, condition = sym_xoroshiro128plus(sym_s0, sym_s1, pid)
    solver.add(condition)
        
    models = get_models(solver)
    return [ model[start_s0].as_long() for model in models ]

def find_seed(seeds, ivs, iv_count):
    for seed in seeds:
        rng = XoroShiro(seed)

        # ec, tid/sid, pid
        for i in range(3):
            rng.nextInt()

        check_ivs = [None]*6
        count = 0
        while count < iv_count:
            stat = rng.nextInt(6)
            if check_ivs[stat] is None:
                check_ivs[stat] = 31
                count += 1

        for i in range(6):
            if check_ivs[i] is None:
                check_ivs[i] = rng.nextInt(32)

        if ivs == check_ivs:
            return seed

    return None

def main():
    ec = int(input("Enter EC: 0x"), 16)
    pid = int(input("Enter PID: 0x"), 16)
    iv_count = int(input("Enter number of guaranteed 31 IVs: "))
    ivs = [ int(iv) for iv in input("Enter IVs(x.x.x.x.x.x): ").split(".") ]
    print("")

    seeds = find_seeds(ec, pid)    
    if len(seeds) > 0:
        seed = find_seed(seeds, ivs, iv_count)
        if seed != None:
            print(f"Raid seed: {hex(seed)}")
            return True

    seedsXor = find_seeds(ec, pid ^ 0x10000000) # Check for shiny lock
    if len(seedsXor) > 0:
        seed = find_seed(seedsXor, ivs, iv_count)
        if seed != None:
            print(f"Raid seed (shiny locked): {hex(seed)}")
            return True

    return False

if __name__ == "__main__":
   if main() == False:
       print("No raid seed")
       print("This means one of three things")
       print("1. You entered something wrong")
       print("2. This script does not check for forced shiny since that takes a long time to compute. Try again with a non-shiny raid")
       print("3. You encountered an extremely rare edge case (odds are you fall under case 1 though)")     
