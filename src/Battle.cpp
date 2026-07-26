#include "Battle.hpp"

#include "extern/dw2003.hpp"
#include "extern/libc.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void*
    SpawnBattleMessageTask_Hook(uint32_t returnAddress, uint8_t unk, uint16_t optionalTechEntity, uint16_t attackEntity)
    {
        // 2. Perform the original task creation
        // 0x80098C9C is the address of BattleMessageTaskUpdate
        BattleMessageTask* pTask = taskCreate((void*)0x80098C9C, 0xB4, 8);

        // 3. Set the standard callbacks (Replicating the original logic)
        uint32_t* taskPtr = (uint32_t*)pTask;
        taskPtr[0xAC / 4] = 0x80099588; // FUN_FIGHTSTG__80099588
        taskPtr[0xB0 / 4] = 0x8009A254; // LAB_FIGHTSTG__8009a254

        // 4. THE FILTER: Identify callers we want to "Mute"

        if (returnAddress == 0x8008CCDC // "Agumon attack!" message caller
            && attackEntity != 0x18     // enemy entity is 0x18
        )
        {
            // Mark the task as State 3 (Finished/Dead) immediately.
            pTask->state = 3;
        }
        if (returnAddress == 0x8008F7C8   // Technique message caller (e.g. "Flame Punch!")
            && optionalTechEntity != 0x18 // enemy entity is 0x18
        )
        {
            // Mark the task as State 3 (Finished/Dead) immediately.
            pTask->state = 3;
        }

        // digivolve is 0x800A7880 if we want to remove it
        // item no effect is 0x8008DC4C
        // healed is 0x8008DCAC
        // couldnt run is 0x800A74CC
        // run away is 0x800A8068
        if (returnAddress == 0x800A7930 || // "Agumon Back!" message caller
            returnAddress == 0x800A7AA8 || // "Go for it!" message caller
            returnAddress == 0x8008DB24 || // Use item message caller
            returnAddress == 0x800A7684 || // "Run!" message caller
            returnAddress == 0x800A8FFC || // "Defeated!" message caller
            returnAddress == 0x800A921C)   // "Battle Won" caller
        {
            // Mark the task as State 3 (Finished/Dead) immediately.
            // The parent will still write its data into the struct (safe).
            // But the engine will delete it next frame before it draws anything.
            pTask->state = 3;
        }

        return pTask;
    }

    void closeBox(uint32_t* hardcodedSrcPtr)
    {
        uint8_t* basePtr     = (uint8_t*)(hardcodedSrcPtr[0x5C / 4]);
        uint32_t indexOffset = hardcodedSrcPtr[0x18 / 4];
        basePtr[indexOffset] = 5;

        hardcodedSrcPtr[0xa8 / 4] = hardcodedSrcPtr[0xaa / 4];
        hardcodedSrcPtr[0xc / 4]  = 1;
        hardcodedSrcPtr[0x10 / 4] = 0;
        hardcodedSrcPtr[0x14 / 4] = 0;
        hardcodedSrcPtr[0x18 / 4] = 0;
    }

    static int resultsTimer = 0;

    void BattleResultsMasterSequenceController_Hook(uint32_t* SequenceStructPointer, uint32_t* textStructBasePtr)
    {
        battleResultsMasterSequenceController(SequenceStructPointer, textStructBasePtr);
        int currentState = SequenceStructPointer[4];

        uint32_t* expBitTextHandlerPtr = (uint32_t*)textStructBasePtr[2]; // pointer texthandler

        switch (currentState)
        {
            case 8:
                resultsTimer++;

                // Wait 30 frames (~0.5 seconds)
                if (resultsTimer >= 30)
                {
                    closeBox(expBitTextHandlerPtr);
                    resultsTimer = 0;
                }
                return;
            case 0x19:
                if (SequenceStructPointer[5] == 1) // This checks if we got an item or not
                {
                    return;
                }
                resultsTimer++;

                // Wait 30 frames (~0.5 seconds)
                if (resultsTimer >= 30)
                {
                    closeBox(expBitTextHandlerPtr);
                    resultsTimer = 0;
                }
                return;
        }
    }
    void BattleResultsLevelUpController_Hook(uint32_t* levelUpPointer, uint32_t* textStructBasePtr)
    {
        battleResultsLevelUpController(levelUpPointer, textStructBasePtr);
        uint32_t* levelUpTextHandlerPtr = (uint32_t*)(textStructBasePtr[0x2c / 4]);
        int currentState                = levelUpPointer[4];
        int techTextState               = levelUpPointer[5];
        switch (currentState)
        {
            case 0x19:
            case 0x2d:
                if (techTextState > 0 && techTextState != 3)
                // case 2d and textState 3 is when rookie level up shows
                // case 2d and textState 4 is digivolve learned
                // case 19 and textState 1 is learned tech
                // case 19 and textState 2 is can load tech
                {
                    return;
                }
                resultsTimer++;

                // Wait 30 frames (~0.5 seconds)
                if (resultsTimer >= 30)
                {
                    closeBox(levelUpTextHandlerPtr);
                    resultsTimer = 0;
                }
                return;
        }
    }
}