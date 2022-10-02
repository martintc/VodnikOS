/**
 * Main entry point into kernel file
 */

#include "mmio.h"
#include "uart.h"

/**
 * integer for the board type of the pi
 */
#if RPI3
unsigned int rpi = 3;
#elif RPI4
unsigned int rpi = 4;
#else
unsigned int rpi = 0;
#endif


void kernel_main() {

  // initialize MMIO
  mmio_initialize(3);
  // initialize UART
  uart_initialize();
  // write to serial
  uart_puts("Hello, World!");
  
  while(1); // infinite loop
}
