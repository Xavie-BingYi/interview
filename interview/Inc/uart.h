/*
 * uart.h
 *
 *  Created on: Aug 31, 2024
 *      Author: Xavier
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define INIT_USART6 0x020
#define INIT_USART1 0x010

void usart_rcc_init(void);

// USART initialization
void usart_init(void);

// USART reset
//void usart_reset(void);

// set uart baudrate
void usart_setBaudrate(int base_addr, int clk_rate, int baudrate);

// send n-bytes data
//void usart_txData(u8ptr_tx_buffer);

// receive 1 byte data
//u8 usart_rxByte(void);

#endif /* UART_H_ */
