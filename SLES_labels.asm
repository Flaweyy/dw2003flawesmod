// =========================================================
// GLOBAL RAM (Always Static)
// =========================================================
.definelabel STATIC_VARS_BASE,       0x8000B200 // user defined, for use in all hacks

.definelabel OPENED_ITEM_BOXES,     0x8004b378
.definelabel NPC1_TABLE,     0x8004b3ac
.definelabel NPC2_TABLE,     0x8004b3b5
.definelabel VISITED_TABLE,     0x8004B3C0
.definelabel STORY_FLAGS,     0x8004b3de
.definelabel BOSSES,     0x0004B38E
.definelabel ASSET_TABLE,       0x80044B3C 
.definelabel PAD_INPUT,            0x8004B81C
.definelabel CURRENT_LANG_ID,      0x8005CCA8
.definelabel MENU_CURRENT_MAP_STORE,       0x80048D68

// Flags
.definelabel MAIN_STORY_FLAG,    0x8004B370
.definelabel SUB_STORY_FLAG_1,    0x8004B3B6
.definelabel SUB_STORY_FLAG_1_B,  0x8004B3B7
.definelabel SUB_STORY_FLAG_2,     0x8004B3E0
.definelabel SUB_STORY_FLAG_3,       0x8004B3BD
.definelabel SUB_STORY_FLAG_4,       0x0004B3AD
.definelabel SUB_STORY_FLAG_5,     0x8004B3AE
.definelabel SUB_STORY_FLAG_6,     0x8004B3AC
.definelabel SUB_STORY_FLAG_7,     0x8004B3B1
.definelabel SUB_STORY_FLAG_8,     0x8004B3BE
.definelabel SUB_STORY_FLAG_9,     0x8004B3AC
.definelabel SUB_STORY_FLAG_10,     0x8004B3F2
.definelabel MASK_HUNT_FLAG_2,     0x8004B3AF
.definelabel CUTSCENE_FLAG_1,     0x8004B3DE
.definelabel ITEM_8LUE_CARD_FLAG,  0x80048F3F
.definelabel ITEM_BLUE_CARD_FLAG,  0x80048F3E
.definelabel ITEM_AGUMON_SUIT_FLAG,  0x80048DBF
.definelabel ITEM_TNT_BALL,  0x80048E0A
.definelabel ITEM_TNT_CHIP,  0x80048DBE
.definelabel SEWERS_EVENT_FLAG,    0x80048DC5


// =========================================================
// SLES INTERNAL FUNCTIONS
// =========================================================
.definelabel drawUIBox,         0x8001ED6C
.definelabel internalDrawText,  0x800197A4
.definelabel allocateTextStruct, 0x8001ae38
.definelabel loadArchiveResource, 0x800140D8
.definelabel playSfx, 0x8002006c // a0 is sfx id

// These return pad inputs in different ways, useful for different contexts (menu vs gameplay)
.definelabel getPadPressed, 0x80018854
.definelabel getPadHeld, 0x80018878
.definelabel getPadRepeat, 0x8001889c // not sure about this one



// --- VRAM / Texture Functions ---
.definelabel initRlenTextureContext,    0x8001FFA8
.definelabel setRlenVramPosition,    0x8001FD28
.definelabel setRlenVramDimensions,    0x8001FD3C
.definelabel processRlenToVram,     0x8001FE38 // Decompress_And_Upload_RLEN_Textures in ghidra
.definelabel vramUploadMap,        0x8001FD3C


//battle to move later
.definelabel taskCreate,          0x80014504
.definelabel battleResultsMasterSequenceController,          0x80084ec4
.definelabel battleResultsLevelUpController,          0x80083e74


// libc
; .definelabel strcat,  0x800911cc
; .definelabel strcmp,  0x800911dc
; .definelabel strncmp, 0x800911ec
.definelabel strcpy,  0x8002518c
.definelabel strncpy, 0x8002529c
; .definelabel strlen,  0x8009121c
; .definelabel strrchr, 0x8009122c
.definelabel memcpy,  0x8002514c
; .definelabel memset,  0x8009125c
; .definelabel sprintf, 0x8009129c
; .definelabel printf,  0x8009128c
; .definelabel rand,    0x8009127c
.definelabel bzero,   0x8002511c