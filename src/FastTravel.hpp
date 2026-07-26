#pragma once

#include "extern/dw2003.hpp"
#include "extern/libc.hpp"
#include "extern/stddef.hpp"
#include "map.h"
#include "questlog.h"


// Based on your 0x8C and 0x98 offsets in ASM
struct MapState
{
    uint8_t padding[0x78];
    uint8_t islandDrawFlag; // 0x78
    uint8_t padding2[0x8C - 0x78 - 1];
    uint32_t namesOffset; // 0x8C
    uint8_t padding3[0x98 - 0x8C - 4];
    int32_t headX; // 0x98
    int32_t headY; // 0x9C
    uint8_t padding4[0xA8 - 0x9C - 4];
    uint32_t iconMemory[46]; // 0xA8 to 0x160
    uint8_t padding5[0x180 - 0x160];
    uint32_t hoverFlag; // 0x180
    uint32_t hoverId; // 0x184
};

// Externs for the SLES functions used in your VRAM logic
extern "C"
{
    

    void* hookFunction(MapState* mapStatePtr, void* a1, void* a2, void* a3);
    void initRlenTextureContext(void* contextBuffer);
    void setRlenVramPosition(int32_t vramX, int32_t vramY);
    void setRlenVramDimensions(int32_t width, int32_t height);
    void processRlenToVram(void* ptr);
    void setIconData(void* iconBase, int mode);
    int getCurrentServer();
}