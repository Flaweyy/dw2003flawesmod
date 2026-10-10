# Flawe's Mod

See details here: https://www.romhacking.net/hacks/9376/

Some notes on the source code:

* Half way through developing the quest log, I changed the approach to use the isBitSet function rather than manually checking bit addresses, I meant to go back and tidy this up but never got around to it
* To make this work quickly, I ended up expanding the overlays and adding copies of them to the end of the disc, it would be better to extend them in place and rebuild the image using markisha64's build tools
* Some of the ASM files could be converted into C++ to make it easier to read.
