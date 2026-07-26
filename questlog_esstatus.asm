.psx
.open "work/DIGIMON/EXTENDED_FILES/ESSTATUS2.BIN", 0

.include "SLES_labels.asm"

; =========================================================
; 2. WORKING TEXT STRUCT (96 Words / 384 Bytes)
; Offset: 0x990
; =========================================================
.org 0x990
TextStructWorkingSpace:
    .fill 384, 0

; =========================================================
; 3. MAIN MENU INIT PAYLOAD (Offset: 0xB10)
; =========================================================
.org 0xB10
    b    wrapCreateStruct

// --- THE OVERLAY LOGIC ---
.org 0xB90
    b   drawQuestBoxes
    nop


// 2. THE C++ OBJECT (Follows immediately after the bridge)
// armips will automatically place the compiled code here.
// It will calculate the address of 'draw_quest_boxes' 
// based on where the bridge ended.
.importobj "compiled/questlog.o"


.close