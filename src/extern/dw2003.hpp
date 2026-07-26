#pragma once

#include "libc.hpp"
#include "stddef.hpp"

extern "C"
{
    enum InputButtons : uint16_t
    {
        // High Byte (Face Buttons & Shoulders)
        BUTTON_SQUARE   = 0x8000,
        BUTTON_TRIANGLE = 0x4000,
        BUTTON_CROSS    = 0x2000,
        BUTTON_CIRCLE   = 0x1000,
        BUTTON_R1       = 0x0800,
        BUTTON_L1       = 0x0400,
        BUTTON_R2       = 0x0200,
        BUTTON_L2       = 0x0100,

        // Low Byte (D-Pad & System)
        BUTTON_LEFT   = 0x0080,
        BUTTON_DOWN   = 0x0040,
        BUTTON_RIGHT  = 0x0020,
        BUTTON_UP     = 0x0010,
        BUTTON_START  = 0x0008,
        BUTTON_SELECT = 0x0001,
    };

    struct StaticVars
    {
        // quest log
        uint32_t QUESTLOG_OVERLAY_PTR;  // Pointer to the Questlog Overlay in RAM, used for drawing the questlog
        uint32_t QUESTLOG_TITLE_OFFSET; // File Offset (Number)
        uint32_t QUESTLOG_STR1_OFFSET;  // File Offset
        uint32_t QUESTLOG_STR2_OFFSET;  // File Offset

        // fast travel
        uint32_t SERVER_INTENT_FLAG;
        int32_t BACKUP_HEAD_X;   // Used to store the player's head direction when warping
        int32_t BACKUP_HEAD_Y;   // Used to store the player's head direction when warping
        uint8_t _padding[3]; // Padding to align the struct to 32 bytes, not strictly necessary but keeps things clean
        uint32_t DEBUG_PRINT;
        uint32_t DEBUG_PRINT2;
    };

    struct AssetEntry
    {
        uint32_t flags;   // Offset 0x00 (Usually flags or status)
        uint32_t id;      // Offset 0x04 (The file ID, e.g., 0xB2)
        uint32_t size;    // Offset 0x08 (Likely file size or LBA)
        uint32_t ramAddr; // Offset 0x0C (The RAM address where it was loaded)
    };

    struct MenuCurrentMapData
    {
        uint16_t mapId;   // 0x00
        uint16_t padding; // 0x02
        int32_t x;        // 0x04
        int32_t y;        // 0x08
    };

    extern StaticVars STATIC_VARS_BASE;
    extern AssetEntry ASSET_TABLE[];
    extern uint16_t PAD_INPUT;

    extern uint32_t CURRENT_LANG_ID;
    extern MenuCurrentMapData MENU_CURRENT_MAP_STORE;

    /**
     * Ensures the specified archive is in RAM and returns the pointer to a specific file.
     * If the archive is not in RAM, it triggers a CD load and waits.
     * @param resourceId Packed ID (Upper 16: Archive ID, Lower 16: File Index)
     * @return RAM pointer to the requested resource data.
     */
    void* loadArchiveResource(uint32_t resourceId);
    void playSfx(uint32_t sfxId);
    uint16_t getPadPressed(uint32_t param1);
}
