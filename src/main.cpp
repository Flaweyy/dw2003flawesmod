#include "game.h"

extern "C" {
    // We name this function for our ASM hook to find
    void test_custom_draw(void* ot_ptr, int32_t flash_val, int32_t original_x, int32_t original_y) {
        
        // 1. Call the original function so the game draws the normal icon border
        Game_DrawBorder(ot_ptr, flash_val, original_x, original_y);

        // 2. Call it again at a custom location (e.g., top left of screen)
        // We reuse the OT pointer and flash values from the game
        // Game_DrawBorder(ot_ptr, 57, 20, 20); 

        // flash is either 57, 56 or 58, 58 is best pick
        Game_DrawBorder(ot_ptr, 58, 20, 180);
    }
}