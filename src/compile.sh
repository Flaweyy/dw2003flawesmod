#!/bin/bash

# Move into the src directory
cd "$(dirname "$0")"
# Create the output folder
mkdir ../compiled/ -p

# Standard PSX/MIPS compiler flags
COMPILER="mips-g++"

# Flags explanation:
# -O3                               -> highest optimization level
# -std=c++23                        -> use C++23
# -T linker.ld                      -> custom linker script, merged .bss into .data section
# -mabi=32                          -> 32 bit ABI
# -march=r3000                      -> target R3000 CPU, used in PSX
# -r                                -> relocateable output/partial linking
# -mel                              -> little endian
# -nostdlib                         -> don't use stdlib, since we don't have one on PSX
# -mexplicit-relocs                 -> use explicit relocations, required by -mno-gpopt
# -mno-shared                       -> don't use position independent code, might be useless?
# -fno-zero-initialized-in-bss      -> prevent the use of GP relative memory and memory sections that are not inserted
# -mno-gpopt                        -> same as above
# -fno-inline-functions             -> disable automatic inlining of functions, to save space and guarantee the symbols
# -fno-inline-small-functions          use 'inline' or 'constexpr' keywords to mark functions for explicit inline
# -msoft-float                      -> we don't have a floating point unit, will cause linking errors when using floats
# -fno-exceptions                   -> we don't have exceptions
# -mno-check-zero-division          -> don't emit trap instructions, saves space
# -Wno-builtin-declaration-mismatch -> don't warn about custom implemented standard functions
# -fno-jump-tables                  -> don't create jump tables for large switch blocks as these cause problems
FLAGS="-O3 -std=c++23 -T linker.ld -mabi=32 -march=r3000 -r -mel -nostdlib -mexplicit-relocs -mno-shared -fno-zero-initialized-in-bss -mno-gpopt -fno-inline-functions -msoft-float -fno-inline-small-functions -fno-exceptions -mno-check-zero-division -Wno-builtin-declaration-mismatch -fno-jump-tables"

echo "Compiling questlog.cpp..."
$COMPILER questlog.cpp -o ../compiled/questlog.o $FLAGS
echo "Compiling FastTravel.cpp..."
$COMPILER FastTravel.cpp -o ../compiled/FastTravel.o $FLAGS
echo "Compiling Battle.cpp..."
$COMPILER Battle.cpp -o ../compiled/Battle.o $FLAGS

echo "Done!"
cd -