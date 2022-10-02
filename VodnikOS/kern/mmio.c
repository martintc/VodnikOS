#include "mmio.h"

static unsigned int MMIO_BASE;

/**
 * Initialize the MMIO
 * This is setting the base address of the MMIO depending on the RPI version
 * arguments:
 *  rpi - integer representing the RPI version 
 */
void mmio_initialize(unsigned int rpi) {
  switch (rpi) {
  case 3:
    MMIO_BASE = 0x3F000000;
    break;
  case 4:
    MMIO_BASE = 0xFE000000;
    break;
  default:
    break;
  }
}


/**
 * Writes a 32-bit unsigned value into a register in the MMIO
 * arguments:
 *   reg - register offset
 *   data - data to be written
 */
void mmio_write(unsigned int reg, unsigned int data) {
  *(volatile unsigned int*)(MMIO_BASE + reg) = data;
}

/**
  * Read data from a register in the MMIO
  * arguments:
  *   reg - the register offset to read
  * returns:
  *   An unsigned int of data stored at address
  */
unsigned int mmio_read(unsigned int reg) {
  return *(volatile unsigned int*)(MMIO_BASE + reg);
}
