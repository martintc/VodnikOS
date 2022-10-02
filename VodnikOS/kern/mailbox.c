#include "mailbox.h"
#include "mmio.h"

#define MBOX_BASE   0xB880
#define MBOX_READ   (MBOX_BASE + 0x00)
#define MBOX_STATUS (MBOX_BASE + 0x18)
#define MBOX_WRITE  (MBOX_BASE + 0x20)

// A Mailbox message with set clock rate of PL011 to 3MHz tag
volatile unsigned int  __attribute__((aligned(16))) mbox[9] = {
    9*4, 0, 0x38002, 12, 8, 2, 3000000, 0 ,0
};

void mailbox_uart_clock_initialize() {
  // UART clock to 3 MHz 
  unsigned int r = (((unsigned int)(&mbox) & ~0xF) | 8);
  // wait until we can talk to VC
  while(mmio_read(MBOX_STATUS) & 0x80000000);
  // send message to property channel and wait for response
  mmio_write(MBOX_WRITE, r);
  // wait
  while((mmio_read(MBOX_STATUS) & 0x40000000) || mmio_read(MBOX_READ) != r );
}
