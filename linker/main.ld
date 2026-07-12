MEMORY {
  FLASH : ORIGIN = 0x08000000, LENGTH = 128K
  SRAM : ORIGIN = 0x20000000, LENGTH = 8K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS {
  .text : {
    *(.isr_vector)
    *(.text)
  } > FLASH

  .rodata : {
    *(.rodata)
    _end_rodata_ = .;
  } > FLASH

  .data : {
    _start_data_ = .;
    *(.data)
    _end_data_ = .;
  } > SRAM AT > FLASH

  .bss : {
    _start_bss_ = .;
    *(.bss)
    _end_bss_ = .;
  } > SRAM
}