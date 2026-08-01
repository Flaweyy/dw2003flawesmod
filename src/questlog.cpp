#include "questlog.h"

#include "map.h"

static uint32_t cached_overlay_base = 0;

// A container for the translated hex
template<int N> struct DW3String
{
    uint8_t data[N * 2]; // Enough room for 2-byte characters
};

// The Compile-Time Translator
template<int N> constexpr auto dw3(const char (&src)[N])
{
    DW3String<N> out{};
    int j = 0;
    for (int i = 0; i < N; i++)
    {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') { out.data[j++] = c - 51; }
        else if (c >= 'a' && c <= 'z') { out.data[j++] = c - 57; }
        else if (c >= '0' && c <= '9') { out.data[j++] = c - 44; }
        else if (c == ' ')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x01;
        }
        else if (c == '.')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x05;
        }
        else if (c == ',')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x04;
        }
        else if (c == '!')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x0A;
        }
        else if (c == '?')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x09;
        }
        else if (c == '-')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x0E;
        }
        else if (c == '/')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x0F;
        }
        else if (c == '\'')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x13;
        }
        else if (c == '"')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x14;
        }
        else if (c == '(')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x15;
        }
        else if (c == '(')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x15;
        }
        else if (c == ')')
        {
            out.data[j++] = 0x01;
            out.data[j++] = 0x16;
        }
        else if (c == '\0') { out.data[j++] = 0; }
    }
    return out;
}

template<int N> static inline const char* resolve(const char (&str)[N])
{
    // This creates a unique piece of hex data in the BIN for this specific string
    static constexpr auto translated = dw3_translate(str);

    // This adds the current overlay base to that hex data's offset
    return (const char*)(STATIC_VARS_BASE.QUESTLOG_OVERLAY_PTR + (uint32_t)translated.data);
}

// THE MACRO: Creates static hex data and returns its offset from 0
#define String(str)                                                                                                    \
    (                                                                                                                  \
        []()                                                                                                           \
        {                                                                                                              \
            static constexpr auto s = dw3(str);                                                                        \
            return (uint32_t)s.data;                                                                                   \
        }())

extern "C"
{
    static void questLogic()
    {
        // 1. Initial Defaults (Simple and clear)
        uint32_t title = String("");
        uint32_t l1    = String("");
        uint32_t l2    = String("");

        // 2. The Router (English-like logic)
        switch (MAIN_STORY_FLAG)
        {
            case 4: // SEIRYU CITY
                // --- PHASE 1: Seiryu City (Default Base State) ---
                title = String("Seiryu City");
                l1    = String("Head East through Wire Forest");
                l2    = String("to reach Seiryu City.");

                if (checkVisited(MAP_SEIRYU_CITY_A))
                {
                    title = String("Repeating Tom");
                    l1    = String("Talk to Repeating Tom in Seiryu Tower");
                    l2    = String("");
                }

                // --- PHASE 2: Protocol Ruins ---
                if (SUB_STORY_FLAG_1 & Phase::Seiryu::GO_TO_RUINS)
                {
                    title = String("Protocol Ruins");
                    l1    = String("Head North of 'East Wire Forest'");
                    l2    = String("to reach Protocol Ruins");

                    // if (checkVisited(MAP_PROTOCOL_RUINS_A))
                    // {
                    //     title = String("Protocol Ruins");
                    //     l1    = String("Return to Protocol Ruins");
                    //     l2    = String("");
                    // }
                    if (MENU_CURRENT_MAP_STORE.mapId == 550)
                    {
                        title = String("Pharaohmon");
                        l1    = String("Take the right path and defeat Pharaohmon");
                        l2    = String("Jump down the ledge and find Seiryu Leader");
                    }
                }

                // --- PHASE 3: Master Tyranno ---
                if (SUB_STORY_FLAG_1_B & Phase::Seiryu::MASTER_TYRANNO)
                {
                    title = String("Tyranno Valley");
                    l1    = String("Head North East of Seiryu City");
                    l2    = String("through Kicking Forest to Tyranno Valley");

                    // if (checkVisited(MAP_TYRANNO_VALLEY_A))
                    if (MENU_CURRENT_MAP_STORE.mapId == MAP_TYRANNO_VALLEY_A)
                    {
                        title = String("Master Tyranno");
                        l1    = String("Climb the valley and Defeat MasterTyranno");
                        l2    = String("");
                    }
                }

                // --- PHASE 4: Defeat Leader ---
                if (SUB_STORY_FLAG_1_B & Phase::Seiryu::DEFEAT_LEADER)
                {
                    title = String("Seiryu Leader");
                    l1    = String("Return to Seiryu Tower and defeat Seiryu Leader");
                    l2    = String("");
                }

                break;

            case 5: // East Station Arc
                // --- BASE: Wind Prairie ---
                title = String("Wind Prairie");
                l1    = String("Head out of the City to Wind Prairie");
                l2    = String("");

                // --- PHASE: East Station ---
                if (SUB_STORY_FLAG_2 & Phase::EastStation::GO_TO_STATION)
                {
                    title = String("East Station");
                    l1    = String("Head South of Kicking Forest");
                    l2    = String("to find East Station");

                    if (MENU_CURRENT_MAP_STORE.mapId == MAP_EAST_STATION_A)
                    {
                        title = String("Gondola");
                        l1    = String("Attempt to activate the gondola");
                        l2    = String("");
                    }
                }
                break;

            case 6: // Blue Card Arc
                // --- BASE: Asuka Bridge ---
                title = String("Asuka Bridge");
                l1    = String("Head to Asuka Bridge and defeat Mystery Player");
                l2    = String("");

                // --- PHASE: Asuka Inn ---
                if (SUB_STORY_FLAG_2 & Phase::BlueCard::ASUKA_INN)
                {
                    title = String("Asuka Inn 2F");
                    l1    = String("Talk to Guilmon in Asuka Inn 2F");
                    l2    = String("");
                }

                // --- PHASE: Forest Inn ---
                if (SUB_STORY_FLAG_3 & Phase::BlueCard::FOREST_INN)
                {
                    title = String("Forest Inn");
                    l1    = String("Head to South in 'East Wire Forest'");
                    l2    = String("to find Forest Inn");

                    if (MENU_CURRENT_MAP_STORE.mapId == MAP_FOREST_INN_A)
                    {
                        title = String("Forest Inn");
                        l1    = String("Talk to Guilmon in Forest Inn");
                        l2    = String("");
                    }
                }

                // --- PHASE: Zephyr Tower ---
                if (SUB_STORY_FLAG_3 & Phase::BlueCard::ZEPHYR_TOWER)
                {
                    if (ITEM_8LUE_CARD_FLAG & 0x01) // Has "8lue" Card
                    {
                        title = String("East Station");
                        l1    = String("Return to East Station and activate the gondola");
                        l2    = String("");
                    }
                    else
                    {
                        title = String("Zephyr Tower");
                        l1    = String("Head back to Seiryu City and talk to");
                        l2    = String("Guilmon in Zephyr Tower beside the Inn");
                    }
                }

                // --- PHASE: Seiryu City ---
                if (SUB_STORY_FLAG_3 & Phase::BlueCard::SEIRYU)
                {
                    title = String("Seiryu City");
                    l1    = String("Talk to Guilmon in Seiryu City beside Seiryu Tower");
                    l2    = String("");
                }

                // --- PHASE: Forest Inn Basement ---
                if (SUB_STORY_FLAG_3 & Phase::BlueCard::FOREST_INN_BM)
                {
                    if (ITEM_BLUE_CARD_FLAG & 0x01) // Has Real Blue Card
                    {
                        title = String("East Station");
                        l1    = String("Return to East Station and activate the gondola");
                        l2    = String("It might be a bumpy ride...");
                    }
                    else
                    {
                        title = String("Forest Inn BF");
                        l1    = String("Find Guilmon in the basement of Forest Inn");
                        l2    = String("");
                    }
                }
                break;

            case 7: // South Sector / Zanbamon
                // --- BASE: Shaman House ---
                title = String("Shaman House");
                l1    = String("Head to the Shaman House in Tranquil Swamp");
                l2    = String("North of Bulk Bridge");

                // --- PHASE: Zanbamon Found ---
                if (checkVisited(MAP_SHAMAN_HOUSE_A)) // Visited trigger
                {
                    title = String("Jungle Grave");
                    l1    = String("Head deep into the Jungle Grave, West of");
                    l2    = String("Bulk Bridge, and talk to Zanbamon");
                }
                break;

            case 8: // Mask Hunt Arc
                // --- BASE: Swamp Inn / Gatomon ---
                title = String("Swamp Inn");
                l1    = String("Head to the inn inside Tranquil Swamp");
                l2    = String("and talk to the Gatomon left of the desk");

                // --- PHASE: Shaman House / Sepikmon ---
                if (SUB_STORY_FLAG_3 & Phase::MaskHunt::TALKED_GATOMON)
                {
                    title = String("Shaman House");
                    l1    = String("Go back to the Shaman House to find Sepikmon");
                    l2    = String("");
                }

                // --- PHASE: Protocol Ruins / Baronmon ---
                if (SUB_STORY_FLAG_5 & Phase::MaskHunt::TALKED_SEPIKMON)
                {
                    title = String("Protocol Ruins");
                    l1    = String("Go to Protocol Ruins in East Sector,");
                    l2    = String("head left and talk to Baronmon");
                }

                // --- PHASE: Asuka Bridge / Sepikmon ---
                if (MASK_HUNT_FLAG_2 & Phase::MaskHunt::TALKED_BARONMON)
                {
                    title = String("Asuka Bridge");
                    l1    = String("Look for Sepikmon at Asuka Bridge");
                    l2    = String("");
                }

                // --- PHASE: Asuka City / Etemon ---
                if (MASK_HUNT_FLAG_2 & Phase::MaskHunt::TALKED_SEPIKMON_BRIDGE)
                {
                    title = String("Asuka City");
                    l1    = String("Look for Etemon in the lower level of Asuka City");
                    l2    = String("beside the big tree");
                }

                // --- PHASE: Divermon's Lake / Find Nick ---
                if (MASK_HUNT_FLAG_2 & Phase::MaskHunt::TALKED_ETEMON)
                {
                    title = String("Divermon's Lake");
                    l1    = String("Find Nick at Divermon's Lake, on the South side");
                    l2    = String("of the bridge and then West, behind some trees");
                }
                break;

            case 9: // Etemon Chase Arc
                // --- BASE: Return to Asuka ---
                title = String("Asuka City");
                l1    = String("Return to lower Asuka City");
                l2    = String("and confront Etemon again");

                // --- PHASE: Chase into Sewers ---
                if (MASK_HUNT_FLAG_2 & Phase::EtemonChase::IN_SEWERS)
                {
                    title = String("Asuka Sewers");
                    l1    = String("Chase Etemon into the sewers");
                    l2    = String("and get the Mask back!");
                }

                // --- PHASE: Return Mask to Sepikmon ---
                if (SEWERS_EVENT_FLAG & Phase::EtemonChase::RETURN_MASK)
                {
                    title = String("Shaman House");
                    l1    = String("Return to the South Sector and give");
                    l2    = String("the mask to Sepikmon");
                }
                break;

            case 10: // Suzaku City Arc
                // --- BASE: Scare Zanbamon ---
                title = String("Jungle Grave");
                l1    = String("Head back to the Jungle Grave and");
                l2    = String("scare away Zanbamon");

                // --- PHASE: Travel to Suzaku City ---
                if (SUB_STORY_FLAG_1 & Phase::Suzaku::SCARED_ZANBAMON)
                {
                    title = String("Suzaku City");
                    l1    = String("Continue through Jungle Grave to Phoenix Bay");
                    l2    = String("and then head East to reach Suzaku City");

                    if (checkVisited(MAP_SUZAKU_CITY_A))
                    {
                        title = String("Suzaku Leader");
                        l1    = String("Enter Suzaku Hall and defeat the");
                        l2    = String("Suzaku Leader");
                    }
                }
                break;
            case 12: // I wanna be a digimon
                title = String("Lamb Chop");
                l1    = String("Talk to Master John at the Lamb Chop");
                l2    = String("");
                if (SUB_STORY_FLAG_4 & Phase::BecomeDigimon::TALKED_MASTER_JOHN)
                {
                    title = String("Old Couple");
                    l1    = String("Talk to Kail in Asuka City");
                    l2    = String("Then talk to the 'Old Couple' in Lower Asuka");
                }
                if (SUB_STORY_FLAG_5 & Phase::BecomeDigimon::TALKED_OLD_COUPLE)
                {
                    title = String("Etemon");
                    l1    = String("Find 'Etemon' in the Sewers");
                    l2    = String("");
                }
                if (SUB_STORY_FLAG_5 & Phase::BecomeDigimon::TALKED_MISCHIEVIOUS_MATT)
                {
                    title = String("Kail");
                    l1    = String("Find Kail in the main lobby");
                    l2    = String("");
                }
                if (SUB_STORY_FLAG_5 & Phase::BecomeDigimon::TALKED_KAIL)
                {
                    title = String("Mischievious Matt");
                    l1    = String("Return to Mischievious Matt in the sewers");
                    l2    = String("");
                }
                if (ITEM_AGUMON_SUIT_FLAG & 0x01)
                {
                    title = String("Kail");
                    l1    = String("Climb the ladder and talk to Kail");
                    l2    = String("");
                }
                break;
            case 13:
                title = String("Admin Center");
                l1    = String("Enter the Admin Center");
                l2    = String("");
                if (SUB_STORY_FLAG_1 & Phase::BecomeDigimon::ENTERED_ADMIN_CENTER)
                {
                    title = String("Kail");
                    l1    = String("Head North into Admin Center 2F");
                    l2    = String("Talk to Kail at the top of the stairs");
                }
                if (SUB_STORY_FLAG_1 & Phase::BecomeDigimon::ENTERED_ADMIN_CENTER)
                {
                    title = String("Kail");
                    l1    = String("Head North into Admin Center 2F");
                    l2    = String("Talk to Kail at the top of the stairs");
                }
                break;
            case 14: // Digi-Egg of Sincerity
                title = String("Phoenix Bay");
                l1    = String("Go to Phoenix Bay and find a girl called Nami");
                l2    = String("She is on a small island in the middle");
                if (SUB_STORY_FLAG_6 & Phase::SincerityEgg::TALKED_NAMI)
                {
                    title = String("Bios Swamp");
                    l1    = String("Head South in Bulk Bridge to find Bios Swamp");
                    l2    = String("");
                    if (MENU_CURRENT_MAP_STORE.mapId == MAP_BIOS_SWAMP_A)
                    {
                        title = String("Bios Shrine");
                        l1    = String("Find the shrine to the East");
                        l2    = String("Walk behind the shrine and press X");
                    }
                }

                if (SUB_STORY_FLAG_7 & Phase::SincerityEgg::FOUND_SHRINE)
                {
                    title = String("Sepikmon");
                    l1    = String("Return to the Shaman House and talk to Sepikmon");
                    l2    = String("");
                }
                if (SUB_STORY_FLAG_7 & Phase::SincerityEgg::TALKED_SEPIKMON)
                {
                    title = String("Baronmon");
                    l1    = String("Head to Protocol Ruins and talk to Baronmon");
                    l2    = String("");
                }
                if (SUB_STORY_FLAG_8 & Phase::SincerityEgg::TALKED_BARONMON)
                {
                    title = String("Baronmon");
                    l1    = String("Talk to Baronmon again to receive the TNT Chip");
                    l2    = String("");
                    if (ITEM_TNT_BALL == 0)
                    {
                        title = String("TNT Ball");
                        l1    = String("Defeat a few Triceramon until you get a TNT Ball");
                        l2    = String("You can find them in Tyranno Valley");
                    }
                }
                if (ITEM_TNT_CHIP > 0)
                {
                    title = String("Bios Swamp");
                    l1    = String("Return to the shrine in Bios Swamp and");
                    l2    = String("blow a hole in the back of it!");
                }

                if (CUTSCENE_FLAG_1 & Phase::SincerityEgg::INSIDE_SHRINE)
                {
                    title = String("Reliability Spot");
                    l1    = String("Climb down the ladder and get the");
                    l2    = String("Digi-Egg of Sincerity");
                }

                break;
            case 15: // West sector
                title = String("South Cape");
                l1    = String("Head to through the Ether Jungle (West of ");
                l2    = String("Phoenix Bay) until you reach South Cape");
                if (MENU_CURRENT_MAP_STORE.mapId == MAP_SOUTH_CAPE_A)
                {
                    title = String("West Sector");
                    l1    = String("Head North to the lily pad dock and call");
                    l2    = String("Submarimon");
                }
                if (MENU_CURRENT_MAP_STORE.mapId == 0x02E3 || MENU_CURRENT_MAP_STORE.mapId == 0x02E6 ||
                    MENU_CURRENT_MAP_STORE.mapId == 0x02E2)
                {
                    title = String("Underwater");
                    l1    = String("Head West until you reach the exit");
                    l2    = String("");
                }
                if (checkVisited(MAP_SOUTH_BADLAND_A))
                {
                    title = String("Byakko City");
                    l1    = String("Head North to the Noise Desert, then further");
                    l2    = String("North to reach Byakko City");
                }

                if (checkVisited(MAP_BYAKKO_CITY_A))
                {
                    title = String("Byakko Leader");
                    l1    = String("Find Byakko Leader in Byakko Dome and");
                    l2    = String("defeat him");
                }
                break;
            case 16: // Escape Cave
                title = String("Underground Cave");
                l1    = String("See if the guards will let you out");
                l2    = String("");

                if (SUB_STORY_FLAG_9 & Phase::EscapeCave::TALKED_GUARD)
                {
                    title = String("Underground Cave");
                    l1    = String("Find the hole in the wall with Numemon in it");
                    l2    = String("");
                }

                if (SUB_STORY_FLAG_10 & Phase::EscapeCave::ESCAPED_CAVE)
                {
                    title = String("Mobius Desert");
                    l1    = String("Head West to return to Noise Desert then head");
                    l2    = String("South through Noise Desert to find Mobius Desert");
                }
                if (MENU_CURRENT_MAP_STORE.mapId == 0x0258 ||
                    MENU_CURRENT_MAP_STORE.mapId == 0x0259) // These are the two map ids for mobius desert
                {
                    title = String("The Mirage");
                    l1    = String("From the first screen, take lower left exit twice");
                    l2    = String("then take upper left exit three times");
                }
                if (checkVisited(MAP_MIRAGE_TOWER_A))
                {
                    title = String("Mirage Tower");
                    l1    = String("Enter the tower and find the real Byakko Leader");
                    l2    = String("");
                }
                break;
            case 17:
                title = String("Dum Dum Factory");
                l1    = String("Head back to Bullet Valley and then North");
                l2    = String("to find Dum Dum Factory");
                if (MENU_CURRENT_MAP_STORE.mapId == MAP_DUM_DUM_FACTORY_A)
                {
                    title = String("Numemon Hunt");
                    l1    = String("Head further in to chase down Numemon!");
                    l2    = String("");
                }
                if (isBitSet(STORY_FLAGS, 33)) // story flag 33 is first numemon
                {
                    title = String("Numemon Hunt");
                    l1    = String("Enter Duct Room 01 and take the item");
                    l2    = String("");
                }
                if (isBitSet(OPENED_ITEM_BOXES, 13)) // first item in duct 01
                {
                    title = String("Numemon Hunt");
                    l1    = String("Climb up the right ladder and then go left and up");
                    l2    = String("Find Numemon in front of Duct Room 04");
                }
                if (isBitSet(STORY_FLAGS, 52))
                {
                    title = String("Numemon Hunt");
                    l1    = String("Enter Duct Room 04 and take the item");
                    l2    = String("");
                }
                if (isBitSet(OPENED_ITEM_BOXES, 14))
                {
                    title = String("Numemon Hunt");
                    l1    = String("Head back to the right ladder then go right and up");
                    l2    = String("to the intersection on the right side of the box");
                }
                if (isBitSet(STORY_FLAGS, 53))
                {
                    title = String("Numemon Hunt");
                    l1    = String("Enter Duct Room 02 and take the item");
                    l2    = String("");
                }
                if (isBitSet(OPENED_ITEM_BOXES, 15))
                {
                    title = String("Numemon Hunt");
                    l1    = String("Climb the ladder in front of Duct Room 02 and");
                    l2    = String("head up towards the yellow stripes");
                }
                if (isBitSet(STORY_FLAGS, 54))
                {
                    title = String("Numemon Hunt");
                    l1    = String("Enter Duct Room 03 and take the item");
                    l2    = String("");
                }
                if (isBitSet(OPENED_ITEM_BOXES, 16))
                {
                    title = String("Numemon Hunt");
                    l1    = String("Head left of Duct Room 03 and go to the");
                    l2    = String("intersection on the left side of the box");
                }
                if (isBitSet(STORY_FLAGS, 55))
                {
                    title = String("Numemon Hunt");
                    l1    = String("Follow Numemon upstairs and talk to HiAndromon");
                    l2    = String("");
                }
                break;
            case 18: // Secret room
                title = String("Secrets");
                l1    = String("Head down the left path of the Secret Stairs");
                l2    = String("Continue further into the sewers");
                break;
            case 20: // Asuka city locked
                title = String("Asuka Bridge");
                l1    = String("Go to Asuka Bridge");
                l2    = String("");
                if (MENU_CURRENT_MAP_STORE.mapId == 0x0202)
                {
                    title = String("Asuka Dock");
                    l1    = String("Head to the dock on the right and go underwater");
                    l2    = String("");
                }
                if (MENU_CURRENT_MAP_STORE.mapId == 0x02e1 || MENU_CURRENT_MAP_STORE.mapId == 0x02e0)
                {
                    title = String("Underwater");
                    l1    = String("Travel to the exit on the right");
                    l2    = String("");
                }
                if (MENU_CURRENT_MAP_STORE.mapId == 0x021b)
                {
                    title = String("Asuka Sewers");
                    l1    = String("Head West until you find the Control Room");
                    l2    = String("");
                }
                if (MENU_CURRENT_MAP_STORE.mapId == 0x021c)
                {
                    title = String("Control Room");
                    l1    = String("Talk to Datamon");
                    l2    = String("");
                }

                if (isBitSet(BOSSES, 3)) // defeated Datamon
                {
                    title = String("Control Room");
                    l1    = String("Talk to Datamon again");
                    l2    = String("");
                }
                if (isBitSet(NPC2_TABLE, 27)) // talked Datamon again
                {
                    title = String("Staff Pass Hunt");
                    l1    = String("Return to the Secret Room inside the");
                    l2    = String("Dum Dum Factory Sewers");
                }
                if (isBitSet(NPC2_TABLE, 28)) // talked Lisa
                {
                    title = String("Staff Pass Hunt");
                    l1    = String("Head to Suzaku Hall, go past Renamon then");
                    l2    = String("exit the Hall and head underwater at the dock");
                    if (MENU_CURRENT_MAP_STORE.mapId == 0x02e2 || MENU_CURRENT_MAP_STORE.mapId == 0x02e0)
                    {
                        title = String("Staff Pass Hunt");
                        l1    = String("Continue right to find the UG Lake");
                        l2    = String("");
                    }
                    if (MENU_CURRENT_MAP_STORE.mapId == 0x0241)
                    {
                        title = String("Staff Pass Hunt");
                        l1    = String("Climb to the second level and go to the far left");
                        l2    = String("to find Nick and Keith");
                    }
                }
                // ug lake is 0x0241
                break;
            case 21: // found DO staff pass
                title = String("Backdoor");
                l1    = String("Return to the Control Room and talk to Datamon");
                l2    = String("");
                break;
            case 22:
                title = String("Backdoor");
                l1    = String("Enter the big door to the North");
                l2    = String("");
                if (MENU_CURRENT_MAP_STORE.mapId == 0x021a)
                {
                    title = String("Admin Center B1F");
                    l1    = String("Battle your way through to the next level");
                    l2    = String("");
                }
                break;
        }

        // if (MENU_CURRENT_MAP_STORE.mapId == MAP_EAST_STATION_A)
        // {
        //     title = String("Gondola");
        //     l1    = String("Attempt to activate the gondola");
        //     l2    = String("");
        // }

        // if (checkVisited(MAP_SOUTH_CAPE_A))
        // {
        //     title = String("West Sector");
        //     l1    = String("Head North to the lily pad dock and call");
        //     l2    = String("Submarimon");
        // }

        // 3. COMMIT: We save the OFFSETS to the registry
        STATIC_VARS_BASE.QUESTLOG_TITLE_OFFSET = title;
        STATIC_VARS_BASE.QUESTLOG_STR1_OFFSET  = l1;
        STATIC_VARS_BASE.QUESTLOG_STR2_OFFSET  = l2;
    };

    void* wrapCreateStruct(int16_t a0, int32_t a1, int32_t x, int32_t y)
    {
        // 1. Call the game's actual function
        void* new_struct = allocateTextStruct(a0, a1, x, y);

        if (y != 0x31)
        {
            return new_struct; // only run the code below on the first time this is called, which is when y = 0x31
        }

        // 2. Find the current overlay base address
        uint32_t current_base = 0;
        for (int i = 0; i < 64; i++)
        {
            if (ASSET_TABLE[i].id == 0xB2) // ESSTATUS2.PRO
            {
                current_base = ASSET_TABLE[i].ramAddr;
                break;
            }
        }

        // 3. Save to static registry
        STATIC_VARS_BASE.QUESTLOG_OVERLAY_PTR = current_base;

        // The last of the 6 calls will be the final version in your workspace
        if (new_struct != nullptr) { memcpy((void*)(current_base + 0x990), new_struct, 384); }

        // String logic here
        questLogic();

        // 4. Return the pointer so the game can continue its loop
        return new_struct;
    }

    static void drawString(TextStruct* s, uint32_t overlay_base, uint32_t offset, int x, int y, int width)
    {
        if (offset == 0) return;
        s->textPtr = (const char*)(overlay_base + offset);
        s->x       = x;
        s->y       = y;
        s->width   = width;
        internalDrawText(s);
    }

    static void drawStrings()
    {
        uint32_t overlay_base = STATIC_VARS_BASE.QUESTLOG_OVERLAY_PTR;
        TextStruct* s1        = (TextStruct*)(overlay_base + 0x990); // struct lives at 0x990

        // Load original struct pointer and check if it is visible before drawing text
        TextStruct* originalStruct =
            (TextStruct*)(s1->originalStructPtr +
                          0xC); // originalStructPtr is actually originalStruct - 0xC, so add 0xC to get correct address
        if (originalStruct == nullptr || originalStruct->visible == 0) { return; }

        // Draw Strings using the helper
        drawString(s1, overlay_base, STATIC_VARS_BASE.QUESTLOG_TITLE_OFFSET, 180, 171, 20);
        drawString(s1, overlay_base, STATIC_VARS_BASE.QUESTLOG_STR1_OFFSET, 16, 193, 60);
        drawString(s1, overlay_base, STATIC_VARS_BASE.QUESTLOG_STR2_OFFSET, 16, 212, 60);
    }

    void drawQuestBoxes(void* structPtr, int boxType, int x, int y)
    {
        drawUIBox(structPtr, boxType, x, y); // draw default box

        // check if title string is empty (check actual address of string in overlay + offset),
        // return early if it is to avoid drawing the extra boxes
        if (*(uint8_t*)(STATIC_VARS_BASE.QUESTLOG_OVERLAY_PTR + STATIC_VARS_BASE.QUESTLOG_TITLE_OFFSET) == 0)
        {
            return;
        }

        drawUIBox(structPtr, 69, 0, 184);
        drawUIBox(structPtr, 65, 169, 169);

        drawStrings();
    }
}