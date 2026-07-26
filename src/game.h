#pragma once
#include "extern/stddef.hpp"

// The Global Drawing function you found
// a0 = OT Pointer, a1 = Flash Settings, a2 = X, a3 = Y
typedef void (*DrawBorderFn)(void* ot, int32_t flash, int32_t x, int32_t y);
static DrawBorderFn Game_DrawBorder = (DrawBorderFn)0x8001ED6C;