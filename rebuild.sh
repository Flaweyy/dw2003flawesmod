#!/bin/bash

# Setup work directory
rm -r ./work/
rm -r ./compiled/
cp -r ./extract/ ./work/
cp BUILD.XML ./work/BUILD.XML

# Apply filesystem changes
# mv ./work/DIGIMON/ETCDAT/SYSTEM_W.TIM ./work/DIGIMON/STDDAT/SYSTEM_W.TIM
#make folder EXTENDED_FILES
mkdir -p ./work/DIGIMON/EXTENDED_FILES/
cp ./modified_files/STSTATUS2.PRO ./work/DIGIMON/EXTENDED_FILES/STSTATUS2.PRO
cp ./modified_files/ESSTATUS2.BIN ./work/DIGIMON/EXTENDED_FILES/ESSTATUS2.BIN
cp ./modified_files/FIGHTSTG2.PRO ./work/DIGIMON/EXTENDED_FILES/FIGHTSTG2.PRO

sh ./src/compile.sh

# Apply patches
# ./tools/linux/armips patch.asm -sym syms.txt
./tools/linux/armips questlog_esstatus.asm
./tools/linux/armips questlog_sles.asm
./tools/linux/armips fast_travel.asm
./tools/linux/armips battle.asm
./tools/linux/armips overworld.asm

# create ISO
./tools/linux/mkpsxiso ./work/BUILD.XML -y -q -lba lba.txt

echo "Finished rebuilding"