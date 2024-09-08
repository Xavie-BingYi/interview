/*
 * io.c
 *
 *  Created on: Sep 8, 2024
 *      Author: Xavier
 */

#include <stdint.h>


void io_write(register uint32_t addr, register uint32_t val) {
    /**
     * r is put the value to general register
     * Qo is let the input be memory address, arm specific
     */
    asm volatile("str %1, %0"
            : : "Qo" (*(volatile uint32_t *) addr), "r" (val));
}

uint32_t io_read(register uint32_t addr) {
    /**
     * r is put the value to general register
     * Qo is let the input be memory address, arm specific
     */
	uint32_t val;
    asm volatile("ldr %0, %1"
            : "=r" (val)
            : "Qo" (*(volatile uint32_t *) addr));
    return val;
}

void io_writeMask(int addr, int data, int mask) {
	io_write(addr, (io_read(addr) & ~mask) | (data & mask));

	/*
	 * addr: 1000_0000
	 * data: 0x5
	 * mask: 0x7
	 *
	 * io_read(addr) = 0x7
	 *
	 *
	 *
	 *
	 * */
}
