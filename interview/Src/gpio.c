/*
 * gpio.c
 *
 *  Created on: Aug 30, 2024
 *      Author: Xavier
 */

#include <stdint.h>
#include <gpio.h>


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

int gpio_init(int port_offset){
	int rcc_status = io_read(RCC_REGISTER + RCC_AHB1_ENR);
	io_write(RCC_REGISTER + RCC_AHB1_ENR , rcc_status|port_offset);  // IO port clock enabled
	return 0;
}

int gpio_set_dir(int port_add , int pin , int mode){
	int pin_status = io_read(port_add + GPIO_MODE);
	int pin_mode_offset = mode << pin*2;
	io_write(port_add + GPIO_MODE , pin_status|pin_mode_offset);  // set output mode
	return 0;
}

int gpio_set_data(int port_add , int pin , int data){
	int pin_status = io_read(port_add + GPIO_OUTPUT_DATA);
	int pin_data_offset = data << pin;
	io_write(port_add + GPIO_OUTPUT_DATA , pin_status|pin_data_offset);  // set output voltage
	return 0;
}

int gpio_pull(int port_add , int pin , int pull_mode){
	int pin_status = io_read(port_add + GPIO_PULL);
	int pin_pull_offset = pull_mode << pin*2;
	io_write(port_add + GPIO_PULL , pin_status|pin_pull_offset);  // set output voltage
	return 0;
}

int gpio_alternate_function(int port_add , int pin , int alternate_mode){
	gpio_set_dir(port_add , pin , GPIO_ALTERNATE);
	int mode;
	if (pin <= 7)
		mode = GPIO_LOW_ALTERNATE;
	else{
		mode = GPIO_HIGH_ALTERNATE;
		pin = pin - 8;
	}

	int pin_status = io_read(port_add + mode);
	int alternate_mode_offset = alternate_mode << pin*4;

	io_write(port_add + mode , pin_status|alternate_mode_offset);  // set alternate function mode
	return 0;
}
