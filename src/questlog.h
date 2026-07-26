#pragma once

#include "extern/dw2003.hpp"
#include "extern/libc.hpp"
#include "extern/stddef.hpp"
#include "map.h"

// Map the labels from SLES_labels.asm
extern "C"
{
    extern uint8_t MAIN_STORY_FLAG;       // 0x8004B370
    extern uint8_t SUB_STORY_FLAG_1;      // 0x8004B3B6
    extern uint8_t SUB_STORY_FLAG_1_B;    // 0x8004B3B7
    extern uint8_t SUB_STORY_FLAG_2;      // 0x8004B3E0
    extern uint8_t SUB_STORY_FLAG_3;      // 0x8004B3BD
    extern uint8_t SUB_STORY_FLAG_4;      // 0x0004B3AD
    extern uint8_t SUB_STORY_FLAG_5;      // 0x8004B3AE
    extern uint8_t SUB_STORY_FLAG_6;      // 0x8004B3AC
    extern uint8_t SUB_STORY_FLAG_7;      // 0x8004B3B1
    extern uint8_t SUB_STORY_FLAG_8;      // 0x8004B3BE
    extern uint8_t SUB_STORY_FLAG_9;      // 0x8004B3AC
    extern uint8_t SUB_STORY_FLAG_10;     // 0x8004B3F2
    extern uint8_t MASK_HUNT_FLAG_2;      // 0x8004B3AF
    extern uint8_t CUTSCENE_FLAG_1;       // 0x8004B3DE
    extern uint8_t ITEM_8LUE_CARD_FLAG;   // 0x80048F3F
    extern uint8_t ITEM_BLUE_CARD_FLAG;   // 0x80048F3E
    extern uint8_t ITEM_AGUMON_SUIT_FLAG; // 0x80048DBF
    extern uint8_t ITEM_TNT_BALL;         // 0x80048E0A
    extern uint8_t ITEM_TNT_CHIP;         // 0x80048DBE
    extern uint8_t SEWERS_EVENT_FLAG;     // 0x80048DC5

    extern uint8_t OPENED_ITEM_BOXES[]; // 0x8004B378
    extern uint8_t NPC1_TABLE[];        // 0x8004B3AC
    extern uint8_t NPC2_TABLE[];        // 0x8004B3B5
    extern uint8_t STORY_FLAGS[];       // 0x8004B3DE
    extern uint8_t VISITED_TABLE[];     // 0x8004B3C0

    void drawUIBox(void* structPtr, int boxType, int32_t x, int32_t y);
    void internalDrawText(void* structPtr);
    void* allocateTextStruct(int16_t a0, int32_t a1, int32_t x, int32_t y);
}

// Update the helper to handle the full MapID
static inline int checkVisited(MapID id)
{
    // 1. Extract the bit-index (The last byte of the Map ID)
    // 0x0226 becomes 0x26
    uint32_t bitIndex = (uint32_t)id & 0xFF;

    // 2. Perform the standard bit-table math
    uint32_t byteIdx = bitIndex >> 3;
    uint32_t bitPos  = bitIndex & 7;

    return (VISITED_TABLE[byteIdx] & (1 << bitPos)) != 0;
}

static inline bool isBitSet(uint8_t* table, uint32_t bitIndex)
{
    // Simple bit-check logic (mimics the game's CheckBitSet)
    return (table[bitIndex >> 3] & (1 << (bitIndex & 7))) != 0;
}

struct TextStruct
{
    uint8_t padding[0x5C];
    const char* textPtr;
    uint8_t padding2[0xA4 - 0x5C - 4];
    uint32_t width;
    uint8_t padding3[8];
    uint8_t x;
    uint8_t pad;
    uint8_t y;
    uint8_t padding4[0xE];
    uint8_t visible;
    uint8_t padding5[0xB2];
    // offset 0x174 is pointer to original struct minus 0xC (need to add 0xC to get correct address)
    uint32_t originalStructPtr;
};

namespace Phase
{
    namespace Seiryu
    {
        const uint8_t GO_TO_RUINS    = 0x80;
        const uint8_t MASTER_TYRANNO = 0x01;
        const uint8_t DEFEAT_LEADER  = 0x02;
    } // namespace Seiryu

    namespace EastStation
    {
        const uint8_t GO_TO_STATION = 0x04; // Highest bit of 0x07
    } // namespace EastStation

    namespace BlueCard
    {
        const uint8_t ASUKA_INN     = 0x40; // 0x07 -> 0x47 (New bit is 0x40)
        const uint8_t FOREST_INN    = 0x10;
        const uint8_t ZEPHYR_TOWER  = 0x20; // 0x10 -> 0x30 (New bit is 0x20)
        const uint8_t SEIRYU        = 0x40; // 0x30 -> 0x70 (New bit is 0x40)
        const uint8_t FOREST_INN_BM = 0x80; // 0x70 -> 0xF0 (New bit is 0x80)
    } // namespace BlueCard

    namespace MaskHunt
    {
        const uint8_t TALKED_GATOMON  = 0x08; // 0xF0 -> 0xF8 (New bit is 0x08)
        const uint8_t TALKED_SEPIKMON = 0x80; // MASK_HUNT_FLAG_1

        // MASK_HUNT_FLAG_2
        const uint8_t TALKED_BARONMON        = 0x01;
        const uint8_t TALKED_SEPIKMON_BRIDGE = 0x02; // 0x01 -> 0x03
        const uint8_t TALKED_ETEMON          = 0x04; // 0x03 -> 0x07
    } // namespace MaskHunt

    namespace EtemonChase
    {
        const uint8_t IN_SEWERS   = 0x08; // MASK_HUNT_FLAG_2: 0x07 -> 0x0F
        const uint8_t RETURN_MASK = 0x01; // SEWERS_EVENT_FLAG
    } // namespace EtemonChase

    namespace Suzaku
    {
        const uint8_t SCARED_ZANBAMON = 0x02; // SUB_STORY_FLAG_1: 0x80 -> 0x82
    } // namespace Suzaku

    namespace BecomeDigimon
    {
        const uint8_t TALKED_MASTER_JOHN       = 0x10; // SUB_STORY_FLAG_3: 0xC3 -> 0xD3
        const uint8_t TALKED_OLD_COUPLE        = 0x10; // SUB_STORY_FLAG_5: 0x81 -> 0x91
        const uint8_t TALKED_MISCHIEVIOUS_MATT = 0x20; // SUB_STORY_FLAG_5: 0x91 -> 0xB1
        const uint8_t TALKED_KAIL              = 0x40; // SUB_STORY_FLAG_5: 0xB1 -> 0xF1
        const uint8_t ENTERED_ADMIN_CENTER     = 0x10; // SUB_STORY_FLAG_1: 0x8B -> 0x9B
    } // namespace BecomeDigimon

    namespace SincerityEgg
    {
        const uint8_t TALKED_NAMI     = 0x04; // SUB_STORY_FLAG_6: 0x11 -> 0x15
        const uint8_t FOUND_SHRINE    = 0x04; // SUB_STORY_FLAG_7: 0x10 -> 0x14
        const uint8_t TALKED_SEPIKMON = 0x08; // SUB_STORY_FLAG_7: 0x14 -> 0x1C
        const uint8_t TALKED_BARONMON = 0x80; // SUB_STORY_FLAG_8: 0x01 -> 0x81
        const uint8_t INSIDE_SHRINE   = 0x80; // CUTSCENE_FLAG_1: 0x55 -> 0xD5
    } // namespace SincerityEgg
    namespace EscapeCave
    {
        const uint8_t TALKED_GUARD = 0x08; // SUB_STORY_FLAG_9: 0x15 -> 0x1D
        const uint8_t ESCAPED_CAVE = 0x04; // SUB_STORY_FLAG_10: 0x02 -> 0x06
    } // namespace EscapeCave
    namespace NumemonHunt
    {
        const uint8_t TALKED_GUARD = 0x08; // SUB_STORY_FLAG_9: 0x15 -> 0x1D
    } // namespace NumemonHunt
} // namespace Phase