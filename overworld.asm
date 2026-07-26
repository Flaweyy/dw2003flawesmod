.psx
.open "work/DIGIMON/SLES_039.36", 0x8000F800 

.org 0x8001CBCC
    bne s0, zero, 0x8001cbec // skip slow auto scrolling on text boxes

.close
