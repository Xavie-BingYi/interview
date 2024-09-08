/*
 * gpio.c
 *
 *  Created on: Aug 30, 2024
 *      Author: Xavier
 */

#include <stdint.h>
#include <gpio.h>
#include <io.h>

int gpio_init(int port_offset){
	int data = 1 << port_offset;
	io_writeMask(RCC_REGISTER + RCC_AHB1_ENR, data, data);  // IO port clock enabled
	return 0;
}

int gpio_set_dir(int port_addr , int pin , int mode){
	int gpio_mode_addr = port_addr + GPIO_MODE;
	int mode_offset = mode << pin*2;
	int mask = mask_bit(2) << pin*2;
	io_writeMask(gpio_mode_addr, mode_offset, mask);  // set output mode
	return 0;
}

int gpio_set_data(int port_addr , int pin , int data){
	int gpio_output_addr = port_addr + GPIO_OUTPUT_DATA;
	int data_offset = data << pin;
	int mask = mask_bit(1) << pin;
	io_writeMask(gpio_output_addr, data_offset, mask);  // set output voltage
	return 0;
}

int gpio_pull(int port_addr , int pin , int pull_mode){
	int gpio_pull_addr = port_addr + GPIO_PULL;
	int pull_mode_offset = pull_mode << pin*2;
	int mask = mask_bit(2) << pin*2;
	io_writeMask(gpio_pull_addr, pull_mode_offset, mask);  // set pull
	return 0;
}

int gpio_alternate_function(int port_addr , int pin , int alternate_mode){
	gpio_set_dir(port_addr , pin , GPIO_ALTERNATE);
	int mode;
	if (pin <= 7)
		mode = GPIO_LOW_ALTERNATE;
	else{
		mode = GPIO_HIGH_ALTERNATE;
		pin = pin - 8;
	}

	int gpio_alternate_addr = port_addr + mode;
	int alternate_mode_offset = alternate_mode << pin*4;
	int mask = mask_bit(4) << pin*4;
	io_writeMask(gpio_alternate_addr , alternate_mode_offset, mask);  // set pull

	return 0;
}
