#ifndef UART_H

/**
 * Initialize the uart0 on the rapberry pi
 * arguments:
 *   rpi - the integer representing what board
 */
void uart_initialize();

/**
 * Write a single character to the UART
 * arguments:
 *   c - a single character to be written
 */

void uart_putc(unsigned char c);

/**
 * Write a character array to the UART
 * arguments:
 *   str - a character array to write to the UART
 */
void uart_puts(const char* str);

/**
 * Read from UART input
 * returns:
 *   a single character received via the UART
 */
unsigned char uart_getc();

#endif
