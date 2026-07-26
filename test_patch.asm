.psx
// We will just create a blank file to see if the code imports
.create "test_output.bin", 0x80000000

.org 0x80000000
// This is the magic command Syd uses
.importobj "compiled/test.o"

.close