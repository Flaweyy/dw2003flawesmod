SECTIONS {
  /* Ensure the text (code) comes first */
  .text : { *(.text*) }
  
  /* Merge all data and bss into one block so zeros are saved to the file */
  .data : {
    *(.data*)
    *(.rodata*)
    *(.bss*)
    *(.sbss*)
  }
}