/*
 * uart.c
 *
 *  Created on: Aug 31, 2024
 *      Author: Xavier
 */

#include <uart.h>
#include <gpio.h>

void usart_rcc_init(void){
	int apb2_rcc = RCC_REGISTER + RCC_APB2_ENR;
	int rcc_status = io_read(apb2_rcc);
	io_write(apb2_rcc , rcc_status|INIT_USART1);  // INIT_USART1 RCC enabled
}

void usart_init(void){

	usart_rcc_init();
	gpio_init(INIT_PORT_A); // IO port A RCC enabled

	gpio_set_dir(INIT_PORT_A , GPIO_PIN_9 , GPIO_HIGH_ALTERNATE);
	gpio_alternate_function(INIT_PORT_A , GPIO_PIN_9 , ALTERNATE_AF7);  // PA9 OPEN USART

	gpio_set_dir(INIT_PORT_A , GPIO_PIN_10 , GPIO_HIGH_ALTERNATE);
	gpio_alternate_function(INIT_PORT_A , GPIO_PIN_10 , ALTERNATE_AF7);  // PA10 OPEN USART
}


void usart_setBaudrate(int base_addr, int clk_rate, int baudrate){

}
