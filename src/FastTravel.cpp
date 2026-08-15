#include "FastTravel.hpp"

#include "extern/dw2003.hpp"
#include "extern/libc.hpp"
#include "extern/stddef.hpp"
#include "map.h"
#include "questlog.h"

// Load IDs (Full 32-bit packed ID for CD Loading)
#define LOAD_ID_AS 0x079A0000
#define LOAD_ID_AM 0x079C0000

// Search IDs (16-bit Archive ID for the Search Loop)
#define SEARCH_ID_AS 0x079A
#define SEARCH_ID_AM 0x079C

extern "C"
{
    void updateMapVisuals(MapState* mapStatePtr, bool isAmaterasu)
    {
        STATIC_VARS_BASE.DEBUG_PRINT = (uint32_t)mapStatePtr;
        // A. Clear Icons
        bzero(mapStatePtr->iconMemory, 46 * sizeof(uint32_t));

        // B. Set Flags
        mapStatePtr->islandDrawFlag = isAmaterasu ? 1 : 0;

        // C. Set Names Offset
        mapStatePtr->namesOffset = CURRENT_LANG_ID + (isAmaterasu ? 1 : 8);

        // D. Temp Patch & Icon Set
        uint16_t originalPatch       = MENU_CURRENT_MAP_STORE.mapId;
        MENU_CURRENT_MAP_STORE.mapId = isAmaterasu ? 633 : 522;
        setIconData(&mapStatePtr->iconMemory, 2);
        MENU_CURRENT_MAP_STORE.mapId = originalPatch;

        // E. Head Icon Visibility
        int actualServer = getCurrentServer();
        int viewedServer = isAmaterasu ? 1 : 0;

        if (actualServer != viewedServer)
        {
            // Hide: Backup and move to 9999
            if (mapStatePtr->headX != 9999)
            {
                STATIC_VARS_BASE.BACKUP_HEAD_X = mapStatePtr->headX;
                STATIC_VARS_BASE.BACKUP_HEAD_Y = mapStatePtr->headY;
                mapStatePtr->headX             = 9999;
                mapStatePtr->headY             = 9999;
            }
        }
        else
        {
            // Show: Restore from backup
            if (mapStatePtr->headX == 9999)
            {
                mapStatePtr->headX = STATIC_VARS_BASE.BACKUP_HEAD_X;
                mapStatePtr->headY = STATIC_VARS_BASE.BACKUP_HEAD_Y;
            }
        }
    }

    void refreshMapFull(MapState* mapStatePtr, bool targetIsAmaterasu)
    {
        uint32_t mapId     = targetIsAmaterasu ? LOAD_ID_AM : LOAD_ID_AS;
        uint8_t* mapRamPtr = (uint8_t*)loadArchiveResource(mapId);
        if (!mapRamPtr) return;

        // Backup scratchpad since the VRAM functions use it for temporary storage
        // static uint32_t scratchpadBackup[16];
        // memcpy(scratchpadBackup, (void*)0x1F8003A0, 64);

        // initRlenTextureContext((void*)0x1F8003A4);
        // initRlenTextureContext((void*)0x8000b260);
        initRlenTextureContext((void*)0x1F800000); // use start of scratchpad for temp context to avoid conflicts

        // Upload Islands
        uint32_t islandOffset  = *(uint32_t*)(mapRamPtr - 8);
        uint8_t* islandDataPtr = (mapRamPtr - 12) + islandOffset;
        setRlenVramPosition(0x380, 0x100);
        processRlenToVram(islandDataPtr);

        // Upload Map
        setRlenVramPosition(0x280, 0);
        setRlenVramDimensions(0x140, 0x100);
        processRlenToVram((void*)mapRamPtr);

        // memcpy((void*)0x1F8003A0, scratchpadBackup, 64);

        // Call the light update to finish the UI
        updateMapVisuals(mapStatePtr, targetIsAmaterasu);
    }

    void syncServerState(MapState* mapStatePtr)
    {
        uint8_t foundMask = 0;
        AssetEntry* table = (AssetEntry*)ASSET_TABLE;

        // 1. search_loop: Identify what is loaded in RAM
        for (int i = 0; i < 64; i++)
        {
            if (table[i].id == SEARCH_ID_AS) foundMask |= 1;
            if (table[i].id == SEARCH_ID_AM) foundMask |= 2;
        }

        // 2. Sync Intent Flag to physical reality
        if (foundMask == 1)
        {
            STATIC_VARS_BASE.SERVER_INTENT_FLAG = 1; // AS ONLY
            return;
        }
        else if (foundMask == 2)
        {
            STATIC_VARS_BASE.SERVER_INTENT_FLAG = 2; // AM ONLY
            return;
        }
        else if (foundMask == 0)
        {
            STATIC_VARS_BASE.SERVER_INTENT_FLAG = 0; // RESET_FLAG
            return;
        }

        // 3. fix_intermediary_state: If both loaded, initialize flag if unset
        if (foundMask == 3 && STATIC_VARS_BASE.SERVER_INTENT_FLAG == 0) { STATIC_VARS_BASE.SERVER_INTENT_FLAG = 1; }

        // 4. compare_now: Check for Desync
        bool targetIsAmaterasu = (STATIC_VARS_BASE.SERVER_INTENT_FLAG == 2);

        // Calculate expected value based on flag (AM = +1, AS = +8)
        uint32_t expectedNamesOffset = CURRENT_LANG_ID + (targetIsAmaterasu ? 1 : 8);

        if (mapStatePtr->namesOffset != expectedNamesOffset)
        {
            // DESYNC DETECTED: Call LIGHT update only (no CD/VRAM)
            updateMapVisuals(mapStatePtr, targetIsAmaterasu);
        }
    }

    bool isMapBlocked(uint16_t mapId)
    {
        if (mapId == MAP_ASUKA_CITY_A)
        {
            if (MAIN_STORY_FLAG == 6 && !(SUB_STORY_FLAG_2 & Phase::BlueCard::ASUKA_INN))
                return true; // blocked for keith first battle at asuka bridge
            if (MAIN_STORY_FLAG >= 20 && MAIN_STORY_FLAG <= 23) return true; // blocked during datamon quest arc
        }
        if (mapId == MAP_ASUKA_CITY_B)
        {
            // TODO block while knightmon activated
        }
        if (mapId == MAP_MOBIUS_DESERT_B || mapId == MAP_MIRAGE_TOWER_B)
        {
            if (MAIN_STORY_FLAG >= 34 && MAIN_STORY_FLAG <= 36) return true; // blocked while AOA occupy
        }
        return false;
    }

    void applyFastTravelRedirect(uint16_t& mapId, int32_t& warpX, int32_t& warpY, uint16_t& headX, uint16_t& headY)
    {
        if (mapId == MAP_ASUKA_CITY_A && MAIN_STORY_FLAG == 6 && !(SUB_STORY_FLAG_2 & Phase::BlueCard::ASUKA_INN))
        {
            // keith battle at bridge
            mapId = 0x0202; // asuka bridge
            warpX = 163380; // not instant trigger is 155845; // override X
            warpY = 76748;  // not instant trigger is 80200;  // override Y
        }
        if (mapId == MAP_ASUKA_CITY_A && MAIN_STORY_FLAG >= 20 && MAIN_STORY_FLAG <= 23)
        {
            // datamon quest, asuka city locked
            mapId = 0x0202; // asuka bridge
            warpX = 187816; // override X
            warpY = 63328;  // override Y
        }
        if (mapId == MAP_SUZAKU_CITY_A && MAIN_STORY_FLAG == 25 && !isBitSet(NPC2_TABLE, 81))
        {
            // earthquake trigger at phoenix bay (not actually required but is a nice touch)
            mapId = MAP_PHOENIX_BAY_A;
            warpX = 330176; // override X
            warpY = 56800;  // override Y
        }
        if (mapId == MAP_ASUKA_CITY_B && MAIN_STORY_FLAG <= 40) // todo find actual flag here
        {
            // block while knightmon activated
            mapId = 0x0272; // asuka bridge AM
            warpX = 97992;  // override X
            warpY = 109722; // override Y
        }
        if (mapId == MAP_MOBIUS_DESERT_B || mapId == MAP_MIRAGE_TOWER_B)
        {
            // blocked while AOA occupy
            if (MAIN_STORY_FLAG >= 34 && MAIN_STORY_FLAG <= 36)
            {
                mapId = 704;    // S Noise Desert
                warpX = 89826;  // override X
                warpY = 146074; // override Y
            }
        }
    }

    bool handleFastTravel(MapState* mapStatePtr)
    {
        if (!mapStatePtr->hoverFlag) { return false; } // return if not currently hovering an icon

        // 2. Determine Viewed Server
        int intent = STATIC_VARS_BASE.SERVER_INTENT_FLAG;
        int server = 0; // 0 = AS, 1 = AM

        if (intent != 0)
        {
            server = (intent == 2) ? 1 : 0; // Convert 1->0, 2->1
        }
        else
        {
            server = getCurrentServer(); // Fallback to actual server
        }

        // 3. Area Router: Find matching Hover ID
        // uint32_t currentHover      = mapStatePtr->hoverId;
        const FastTravelDest* dest = nullptr;

        for (int i = 0; i < 46; i++)
        {
            if (warpTable[i].hoverId == mapStatePtr->hoverId)
            {
                dest = &warpTable[i];
                break;
            }
        }

        // 4. Executor
        if (dest)
        {
            uint16_t newMapId = (server == 1) ? dest->amatId : dest->asukaId;
            int32_t newWarpX  = dest->warpX;
            int32_t newWarpY  = dest->warpY;
            uint16_t newHeadX = dest->headX;
            uint16_t newHeadY = dest->headY;

            applyFastTravelRedirect(newMapId, newWarpX, newWarpY, newHeadX, newHeadY);

            // if (isMapBlocked(newMapId))
            // {
            //     playSfx(0x800450B2); // blocked sound
            //     return false;
            // }

            // Select correct Map ID based on viewed server
            MENU_CURRENT_MAP_STORE.mapId = newMapId;
            MENU_CURRENT_MAP_STORE.x     = newWarpX;
            MENU_CURRENT_MAP_STORE.y     = newWarpY;

            // Update Head Visuals (s1 + 0x98)
            mapStatePtr->headX = newHeadX;
            mapStatePtr->headY = newHeadY;

            playSfx(0x8004503C); // success sound

            return true;
        }

        return false;
    }

    void handleServerToggle(MapState* mapStatePtr)
    {
        // Toggle Intent
        if (STATIC_VARS_BASE.SERVER_INTENT_FLAG == 0)
        {
            STATIC_VARS_BASE.SERVER_INTENT_FLAG = (getCurrentServer() == 0) ? 2 : 1;
        }
        else
        {
            STATIC_VARS_BASE.SERVER_INTENT_FLAG ^= 3;
        }

        // Button was pressed: Perform HEAVY update
        refreshMapFull(mapStatePtr, (STATIC_VARS_BASE.SERVER_INTENT_FLAG == 2));
    }

    void mainInputRouter(MapState* mapStatePtr)
    {
        // Always check state first (Guardian logic)
        syncServerState(mapStatePtr);

        uint16_t pressedButton = getPadPressed(0);

        if (pressedButton & BUTTON_SQUARE)
        {
            handleServerToggle(mapStatePtr);
            return;
        }

        // Fast Travel Logic
        if ((pressedButton & BUTTON_CROSS)) { handleFastTravel(mapStatePtr); }
    }

    void* handleHook(MapState* mapStatePtr, void* a1, void* a2, void* a3)
    {
        void* hookOutput = hookFunction(mapStatePtr, a1, a2, a3);
        mainInputRouter(mapStatePtr);
        return hookOutput;
    }
}