/**
 * Used under the LGPL license from the https://github.com/sandeepmistry/arduino-nRF5 project
 * Source: https://github.com/sandeepmistry/arduino-nRF5/blob/master/cores/nRF5/SDK/components/softdevice/s132/toolchain/armgcc/armgcc_s132_nrf52832_xxaa.ld
 */

/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x1c000, LENGTH = 0x64000
  RAM (rwx) :  ORIGIN = 0x20002080, LENGTH = 0xdf80
}

SECTIONS
{
  .fs_data :
  {
    PROVIDE(__start_fs_data = .);
    KEEP(*(.fs_data))
    PROVIDE(__stop_fs_data = .);
  } > RAM
} INSERT AFTER .data;

INCLUDE "nrf5x_common.ld"