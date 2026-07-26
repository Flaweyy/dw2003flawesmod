.psx
.open "work/DIGIMON/SLES_039.36", 0x8000F800 

; --- Patch File System for STSTATUS ---

; Offset 0x35d08 -> RAM Address 80045508 (0x8000F800 + 0x35d08)
.org 0x80045508
    .word 0x000439EC  ; Sets New LBA (Hex 439EC)

; Offset 0x37F72 -> RAM Address 0x80047772 (0x8000F800 + 0x37F72)
.org 0x80047772
    .halfword 0x0046      ; Sets New Size to 70 Sectors -> 49 00

.close

.psx
.open "work/DIGIMON/EXTENDED_FILES/FIGHTSTG2.PRO", 0x80082cb0 

.include "SLES_labels.asm"
.include "STSTATUS_labels.asm"

// These two are to pass a3 from another function in the 'Tech' message spawner 
// function into the SpawnBattleMessageTask_Hook
.org 0x8008F7BC
    move $t5, $a3
.org 0x8008F7C4
    move $a2, $t5

.org 0x8009a298 
    j  SpawnBattleMessageTask_Hook
    move $a0, $ra // Pass the Return Address as an argument to our hook (for filtering)
    

.org 0x800A47c0 // start of our temp code space
; .org 0x800A39a0 // start of our temp code space, we have total of 5664 bytes
    .importobj "compiled/Battle.o"

.org 0x80098b88
    addiu v0, zero, 2 // skip from case 2 to case 4 of battle message drawing so auto closes

.org 0x80098c4c
    lw v1, 0x84(s0) // Choose the correct timer so that our message box doesnt close too early

; .org 0x80098c58 // timer speed
    ; slti v0, v0, 21 // default

.close

.psx
.open "work/DIGIMON/AAA/PRO/STFGTREP.PRO", 0x80082CB0

.org 0x80085664
    bne s0, zero, 0x800857d0 // skip slow auto scrolling on text boxes in result screen for bit/exp

.org 0x800844dc
    bne s0, zero, 0x8008488c // skip slow auto scrolling on text boxes in result screen for level up

.org 0x80085ad0
    jal BattleResultsMasterSequenceController_Hook

.org 0x800849e0
    jal BattleResultsLevelUpController_Hook

.close
