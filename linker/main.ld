ENTRY(Reset_Handler)

MEMORY {
  FLASH : ORIGIN = 0x08000000, LENGTH = 128K
  SRAM : ORIGIN = 0x20000000, LENGTH = 8K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS {
  .text : {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    *(.text*)
    . = ALIGN(4);
  } > FLASH

  .rodata : {
    . = ALIGN(4);
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  .data : {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > SRAM AT > FLASH

  _sidata = LOADADDR(.data);

  .bss : {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    . = ALIGN(4);
    _ebss = .;
  } > SRAM
}