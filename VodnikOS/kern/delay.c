#include "delay.h"

/**
 * A delay function
 */
void delay(unsigned int count) {
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}
