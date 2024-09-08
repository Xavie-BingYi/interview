/*
 * io.h
 *
 *  Created on: Sep 8, 2024
 *      Author: Xavier
 */

#ifndef IO_H_
#define IO_H_

#include <stdint.h>


void io_write(register uint32_t addr, register uint32_t val);
uint32_t io_read(register uint32_t addr);
void io_writeMask(int addr, int data, int mask);

#endif /* IO_H_ */
