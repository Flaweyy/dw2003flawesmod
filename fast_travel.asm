.psx
.open "work/DIGIMON/SLES_039.36", 0x8000F800 

; --- Patch File System for STSTATUS ---

; Offset 0x35F88 -> RAM Address 80045788 (0x8000F800 + 0x35F88)
.org 0x80045788
    .word 0x000439A5  ; Sets New LBA (Hex 439A5)

; Offset 0x380B2 -> RAM Address 0x800478B2 (0x8000F800 + 0x380B2)
.org 0x800478B2
    .halfword 0x0037      ; Sets New Size to 55 Sectors -> 37 00

.close

.psx
.open "work/DIGIMON/EXTENDED_FILES/STSTATUS2.PRO", 0x80082cb0

.include "SLES_labels.asm"
.include "STSTATUS_labels.asm"

; .org 0x80098C88 //0x80082cb0 + 0x15FE0 
.org 0x80099304 // parent of above
    jal  handleHook
    

.org 0x8009B720 // start of our extended code space
    .importobj "compiled/FastTravel.o"



.close