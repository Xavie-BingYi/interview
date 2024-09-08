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

#define USART_HIGH_VAL 1
#define USART_LOW_VAL  0

#define USART_SR  0x00
#define USART_DR  0x04
#define USART_BRR 0x08
#define USART_CR1 0x0C
#define USART_CR2 0x10

#define USART6_DEFAULT_FREQUENCE 16000000
#define BRR_115200 115200

#define USART_SR_TC  6
#define USART_SR_TXE 6

#define USART_CR1_UE 13
#define USART_CR1_TE 3

#define USART_CR2_STOP_HIGH 13
#define USART_CR2_STOP_LOW  12


// USART initialization
void usart_rcc_enable(int bus, int usart);
void usart_dr(int bus, uint8_t data);
void usart_cr1(int bus, int bit, int status);
void usart_cr2(int bus, int bit, int status);
void usart_init(void);

// USART reset
//void usart_reset(void);

// set uart baudrate
void usart_setBaudrate(int bus, int clk_rate, int baudrate);

// send n-bytes data
void usart_txData(int bus, uint8_t tx_buffer);

// receive 1 byte data
//u8 usart_rxByte(void);

#endif /* UART_H_ */
