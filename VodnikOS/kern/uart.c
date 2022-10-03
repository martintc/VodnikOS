#include "uart.h"
#include "mmio.h"
#include "mailbox.h"
#include "delay.h"

/**
 * Where the GPIO Base starts relative to MMIO
 */
#define GPIO_BASE 0x200000

/**
 * Where the UART base starts relative to MMIO
 */
#define UART_BASE (GPIO_BASE + 0x1000)

/**
 * Controls the actuation of pull up/down pins of all GPIO pins
 */
#define GPPUD (GPIO_BASE + 0x94)

/**
 * Controls actuaion of pull up/down for specific GPIO pins
 */
#define GPPUDCLK (GPIO_BASE + 0x98)

/**
 * Registers for UART
 */
#define UART_DR     (UART_BASE + 0x00)
#define UART_RSRECR (UART_BASE + 0x04)
#define UART_FR     (UART_BASE + 0x18)
#define UART_ILPR   (UART_BASE + 0x20)
#define UART_IBRD   (UART_BASE + 0x24)
#define UART_FBRD   (UART_BASE + 0x28)
#define UART_LCRH   (UART_BASE + 0x2C)
#define UART_CR     (UART_BASE + 0x30)
#define UART_IFLS   (UART_BASE + 0x34)
#define UART_IMSC   (UART_BASE + 0x38)
#define UART_RIS    (UART_BASE + 0x3C)
#define UART_MIS    (UART_BASE + 0x40)
#define UART_ICR    (UART_BASE + 0x44)
#define UART_DMACR  (UART_BASE + 0x48)
#define UART_ITCR   (UART_BASE + 0x80)
#define UART_ITIP   (UART_BASE + 0x84)
#define UART_ITOP   (UART_BASE + 0x88)
#define UART_TDR    (UART_BASE + 0x8C)

#define PIN_14 14
#define PIN_15 15

void uart_initialize() {
  // disable UART
  mmio_write(UART_CR, 0);
  // set up GPIO pins
  mmio_write(GPPUD, 0);
  // need a delay here
  delay(150);
  // Write 0 to GPPUDCLK to make the change
  mmio_write(GPPUDCLK, (1 << PIN_14) | (1 << PIN_15));
  // delay needed here
  delay(150);
  // write 0 to GPPUDCLK
  mmio_write(GPPUDCLK, 0);

  // clear pending interrupts
  mmio_write(UART_ICR, 0x7FF);

  // set clock
  mailbox_uart_clock_initialize();

  // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
  mmio_write(UART_IBRD, 1);
  // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
  mmio_write(UART_FBRD, 40);
 
  // Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
  mmio_write(UART_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
  // Mask all interrupts.
  mmio_write(UART_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                 (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
 
  // Enable UART0, receive & transfer part of UART.
  mmio_write(UART_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

/**
 * Write a single character to the UART
 * arguments:
 *   c - a single character to be written
 */
void uart_putc(unsigned char c) {
  // check that the uart is ready to receive a character to write
  while(mmio_read(UART_FR) & (1 << 5));
  mmio_write(UART_DR, c);
}

/**
 * Write a character array to the UART
 * arguments:
 *   str - a character array to write to the UART
 */
void uart_puts(const char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    uart_putc((unsigned char)str[i]);
  }
}

/**
 * Read from UART input
 * returns:
 *   a single character received via the UART
 */
unsigned char uart_getc() {
  // wait for input on UART
  while(mmio_read(UART_FR) & (1 << 4));
  return mmio_read(UART_DR);
}
