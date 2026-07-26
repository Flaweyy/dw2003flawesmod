#include "questlog.h"

static uint32_t cached_overlay_base = 0;

extern "C"
{
    // These link to the .string labels in your ASM strings file
    extern char Q04_01_T[], Q04_01_1[], Q04_01_2[];
    extern char Q04_02_T[], Q04_02_1[], Q04_02_2[];
    extern char Q04_03_T[], Q04_03_1[], Q04_03_2[];
    extern char Q04_04_T[], Q04_04_1[], Q04_04_2[];
    extern char Str_Quest_Title_Def[], Str_Quest_Empty_1[], Str_Quest_Empty_2[];

    static inline const char* resolve(void* label)
    {
        return (const char*)(StaticVarsBase.questlogOverlayPtr + (uint32_t)label);
    }

    static void quest_string_logic()
    {
        // 1. Initial Defaults (Simple and clear)
        const char* title = resolve(Str_Quest_Title_Def);
        const char* l1    = resolve(Str_Quest_Empty_1);
        const char* l2    = resolve(Str_Quest_Empty_2);

        // 2. The Router (English-like logic)
        switch (MainStoryFlag)
        {
            case 4: // Seiryu Arc

                if (SubStoryFlag1 == 0x80) // Protocol ruins
                {
                    if (!check_visited(0x26))
                    {
                        title = resolve(Q04_03_T);
                        l1    = resolve(Q04_03_1);
                        l2    = resolve(Q04_03_2);
                        break;
                    }
                    title = resolve(Q04_04_T);
                    l1    = resolve(Q04_04_1);
                    l2    = resolve(Q04_04_2);
                    break;
                }
                // fall through to default which is Seiryu city
                if (!check_visited(0x2E))
                {
                    title = resolve(Q04_01_T);
                    l1    = resolve(Q04_01_1);
                    l2    = resolve(Q04_01_2);
                    break;
                }
                title = resolve(Q04_02_T);
                l1    = resolve(Q04_02_1);
                l2    = resolve(Q04_02_2);
                break;

            case 5: // East Station
                // Very easy to add new cases now!
                break;
        }

        // 3. COMMIT
        StaticVarsBase.questlogTitlePtr = title;
        StaticVarsBase.questlogStr1Ptr  = l1;
        StaticVarsBase.questlogStr2Ptr  = l2;
    };

    void* wrap_create_struct(int16_t a0, int32_t a1, int32_t x, int32_t y)
    {
        // 1. Call the game's actual function
        void* new_struct = AllocateTextStruct(a0, a1, x, y);

        // 2. Find the current overlay base address
        uint32_t current_base = 0;
        for (int i = 0; i < 64; i++)
        {
            if (AssetTable[i].id == 0xB2) // ESSTATUS2.PRO
            {
                current_base = AssetTable[i].ramAddr;
                break; // Stop searching once found
            }
        }

        // 3. Save to static registry
        StaticVarsBase.questlogOverlayPtr = current_base;

        // String logic here
        quest_string_logic();

        // The last of the 6 calls will be the final version in your workspace
        if (new_struct != nullptr && current_base != 0) { memcpy((void*)(current_base + 0x990), new_struct, 384); }
        // 4. Return the pointer so the game can continue its loop
        return new_struct;
    }

    // Standard helper function instead of a Lambda
    static void draw_line(TextStruct* s, const char* str_ptr, int x, int y, int width)
    {
        s->textPtr = (const char*)(str_ptr);
        s->x       = x;
        s->y       = y;
        s->width   = width;
        InternalDrawText(s);
    }

    static void draw_strings()
    {
        uint32_t overlay_base = StaticVarsBase.questlogOverlayPtr;
        TextStruct* s1        = (TextStruct*)(overlay_base + 0x990); // struct lives at 0x990

        // Draw Strings using the helper
        draw_line(s1, StaticVarsBase.questlogTitlePtr, 180, 171, 20);
        draw_line(s1, StaticVarsBase.questlogStr1Ptr, 16, 193, 60);
        draw_line(s1, StaticVarsBase.questlogStr2Ptr, 16, 212, 60);
    }

    void draw_quest_boxes(void* structPtr, int boxType, int x, int y)
    {
        DrawUIBox(structPtr, boxType, x, y); // draw default box
        DrawUIBox(structPtr, 69, 0, 184);
        DrawUIBox(structPtr, 65, 169, 169);

        draw_strings();
    }
}