// =========================================================
// STSTATUS.PRO OVERLAY FUNCTIONS (0x80082cb0 + Offset)
// =========================================================

// v0: 0(AS) or 1(AM) based on current game state
.definelabel getCurrentServer,     0x8009A670

// Arg0: Icon Base, Arg1: Mode (2)
.definelabel setIconData,          0x8009A6B8

; .definelabel hookFunction,          0x8001D6F4
.definelabel hookFunction,          0x80098b84 // a0 is pointer to map struct

