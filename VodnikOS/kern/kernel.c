/**
 * Main entry point into kernel file
 */

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

  
  
    while(1); // infinite loop
}
