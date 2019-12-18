# RaidFinder

This is an RNG tool for Pokemon Sword/Shield. Due to the nature of the games only raids are RNGable.

# Getting the Raid Seed

There are currently two ways to get the raid seed.

Option 1 (dumping the savefile):

1. This step requires a switch that either has CFW or is vulnerable to RCM. I will not cover how to dump a save from the switch as there are other resources for that.

2. You must install and setup [PKHeX](https://github.com/kwsch/PKHeX) and the [Raid Plugin for PKHeX](https://github.com/Leanny/PKHeX_Raid_Plugin).

3. Open the save file you dumped in PKHeX and open the Raid Plugin. It will tell you all the relevant information of each den in your save file.

Option 2 (dumping the pkm):

1. This step does not require a switch that either has CFW or is vulnerable to RCM. It does however require finding someone else that does have those capabilities.

2. Have this person join your raid and catch the pokemon. Afterwards they should either send you the .pk8 file or tell you the EC/PID/IVs of the pokemon.

3. Install [z3](https://pypi.org/project/z3-solver/) which raid_solver.py uses. `pip install z3-solver`

4. Use the [raid_solver.py](https://github.com/Admiral-Fish/RaidFinder/blob/master/raid_solver.py) script that comes with RaidFinder and input the information it asks for. It will calculate your raid seed and output it to you.
