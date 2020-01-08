# RaidFinder

This is an RNG tool for Pokemon Sword/Shield. Due to the nature of the games only raids are RNGable.

# Getting the Raid Seed

There are currently two ways to get the raid seed if you have access to CFW or a RCM vulnerable switch.

Option 1: Viewing game RAM (CFW only)

1. Setup [CaptureSight](https://github.com/zaksabeast/CaptureSight)

2. Use CaptureSight to view the raid seed.

Option 2: Dumping the savefile (CFW or RCM)

1. Dump the save file. I will not cover how to do this since there are other resources that cover this.

2. Install and setup [PKHeX](https://github.com/kwsch/PKHeX) and the [Raid Plugin for PKHeX](https://github.com/Leanny/PKHeX_Raid_Plugin).

3. Open the save file you dumped in PKHeX and open the Raid Plugin. It will tell you all the relevant information of each den in your save file.

There are currently two ways to get the raid seed if you don't have access to CFW. These both require finding someone with CFW. Option 1 is the better of the two since it saves time on the person checking the raid seed for you.

Option 1: Trading the raid

1. Capture the raid pokemon and don't save the game.

2. Initiate a trade with this person and have them view the raid pokemon using [CaptureSight](https://github.com/zaksabeast/CaptureSight). They will be able to tell you the EC, PID, and IVs of the pokmon.

3. Install [z3](https://pypi.org/project/z3-solver/) which raid_solver.py uses. `pip install z3-solver`

4. Use the [raid_solver.py](https://github.com/Admiral-Fish/RaidFinder/blob/master/raid_solver.py) script that comes with RaidFinder and input the information it asks for. It will calculate your raid seed and output it to you.

Option 2: Joining the raid

1. Have this person join your raid and catch the pokemon. Afterwards they should either send you the .pk8 file or tell you the EC/PID/IVs of the pokemon.

2. Install [z3](https://pypi.org/project/z3-solver/) which raid_solver.py uses. `pip install z3-solver`

3. Use the [raid_solver.py](https://github.com/Admiral-Fish/RaidFinder/blob/master/raid_solver.py) script that comes with RaidFinder and input the information it asks for. It will calculate your raid seed and output it to you.

# Credits

[Leanny](https://github.com/leanny): Labeling den map and den hashes in the [PKHeX Raid Plugin](https://github.com/Leanny/PKHeX_Raid_Plugin). Also providing me with event dens.

[Kaphotics](https://github.com/kwsch): Easy dumping of raid tables with [pkNX](https://github.com/kwsch/pkNX). Also providing me with event dens.

[rusted-coil](https://github.com/rusted-coil/): Providing a great open source seed searcher called [OneStar](https://github.com/rusted-coil/OneStar) from which I based my seed searcher.
