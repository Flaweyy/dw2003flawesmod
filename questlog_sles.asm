.psx
.open "work/DIGIMON/SLES_039.36", 0x8000F800 

; --- Patch File System for ESSTATUS ---

; Offset 0x35A34 -> RAM Address 0x80045234 (0x8000F800 + 0x35A34)
.org 0x80045234
    .word 0x000439DC  ; Sets New LBA (Hex 439A5) +sector size of STATUS.PRO above it, + 55 (Decimal) so 439DC

; Offset 0x37E08 -> RAM Address 0x80047608 (0x8000F800 + 0x37E08)
.org 0x80047608
    .halfword 0x0015      ; Sets New Size to 21 Sectors -> 15 00

; =========================================================
; 1. THE PERMANENT HOOKS
; These are always active. They jump to our "Hub" at C000.
; =========================================================

; Hook 1: Menu Init
.org 0x80012228
    jal 0x8000B500

; Hook 2: Menu Draw Box
.org 0x80013100 
    jal 0x8000B600


; =========================================================
; 2. THE ONE-SHOT COPIER (Triggered at Boot/Map Load)
; This moves the Hub Code from SLES storage to 0x8000B500
; =========================================================
.org 0x8001D608 
    j SECTION_COPIER
    nop

.org 0x800A4730 ; 0x800A5730
SECTION_COPIER:
    addiu sp, sp, -32
    sw    t0, 0(sp)
    sw    t1, 4(sp)
    sw    t3, 12(sp)
    sw    t4, 16(sp)
    sw    t5, 20(sp)

    li  t0, 0x8000B500      ; Dest: Mod Hub Home
    li  t3, HUB_CODE_STORAGE ; Source: SLES Padding
    li  t4, 200             ; Words to copy (800 bytes)

copy_hub_loop:
    lw    t5, 0(t3)
    nop
    sw    t5, 0(t0)
    addiu t3, t3, 4
    addiu t0, t0, 4
    addiu t4, t4, -1
    bgtz  t4, copy_hub_loop
    nop

    ; Restore original code at 8001D608 (The Copier Trigger)
    li  t0, 0x8001D608
    li  t1, 0x8C820034      ; lw v0, 0x34(a0)
    sw  t1, 0(t0)
    li  t1, 0x3C038006      ; lui v1, 0x8006
    sw  t1, 4(t0)

    lw    t0, 0(sp)
    lw    t1, 4(sp)
    lw    t3, 12(sp)
    lw    t4, 16(sp)
    lw    t5, 20(sp)
    addiu sp, sp, 32
    j   0x8001D608          ; Return and run the restored instructions
    nop

; =========================================================
; 3. THE HUB CODE STORAGE (SLES Padding)
; This code will be copied to 0x8000B500 at runtime.
; =========================================================
.org 0x800A3D00
HUB_CODE_STORAGE:

; --- b500: INIT ENTRANCE ---
    ; This logic runs when Hook 1 (0x80012228) is triggered
    lui     t0, 0x8004
    addiu   t0, t0, 0x4B3C  ; Asset Table
    li      t1, 64          ; Loop count
    li      t2, 0x00B2      ; ID: ESSTATUS2.PRO
find_init:
    lw      t3, 4(t0)
    nop
    beq     t3, t2, jump_init
    nop
    addiu   t0, t0, 16
    addiu   t1, t1, -1
    bnez    t1, find_init
    nop
    ; If not found, jump to original init (allocateTextStruct)
    j       0x8001ae38    
    nop
jump_init:
    lw      t9, 12(t0)       ; Pass RAM Base in a0
    nop
    addiu   t4, t9, 0xB10   ; OFFSET TO INIT PAYLOAD
    jr      t4
    nop

.org HUB_CODE_STORAGE + 0x100
; --- b600: DRAW ENTRANCE ---
    ; This logic runs when Hook 2 (0x80013100) is triggered
    lui     t0, 0x8004
    addiu   t0, t0, 0x4B3C
    li      t1, 64
    li      t2, 0x00B2
find_draw:
    lw      t3, 4(t0)
    nop
    beq     t3, t2, jump_draw
    nop
    addiu   t0, t0, 16
    addiu   t1, t1, -1
    bnez    t1, find_draw
    nop
    j       0x8001ED6C // call draw function
    nop
jump_draw:
    lw      t1, 12(t0)
    nop
    addiu   t4, t1, 0xB90   ; jump to box
    nop
    jr      t4

.close