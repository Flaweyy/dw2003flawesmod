.psx
// Assume your expanded overlay is opened here
.open "work/DIGIMON/AAA/PRO/STSTATUS.PRO", 0x80082cb0 

// 1. Find the spot where the game calls the original draw function
// Original: jalr t9 (pointing to 8001ed6c)
.org 0x80098DB0 
    jal test_custom_draw


// 2. Import the C++ object into your new space
.org 0x8009AFB0
.importobj "compiled/main.o"

.close