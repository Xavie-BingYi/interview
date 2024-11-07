/*
 * uart.charactor
 *
 *  Created on: Aug 31, 2024
 *      Author: Xavier
 */

#include <uart.h>
#include <gpio.h>
#include <io.h>

void usart_rcc_enable(int bus, int usart){
	int usart_rcc = RCC_REGISTER + bus;
	int rcc_status = io_read(usart_rcc);
	io_write(usart_rcc , rcc_status|usart);  // uartRCC enabled
}

void usart_dr(int bus, uint8_t data){
	int usart_dr = bus + USART_DR;
	io_write(usart_dr , data);
}

void usart_cr1(int bus, int bit, int status){
	int usart_cr1 = bus + USART_CR1;
	int cr1_function = status << bit;
	int cr1_status = io_read(usart_cr1);
	if (status == 0)
		cr1_status = cr1_status & ~cr1_function;
	else
		cr1_status = cr1_status | cr1_function;
	io_write(usart_cr1 , cr1_status);  // SET USART CR1
}

void usart_cr2(int bus, int bit, int status){
	int usart_cr2 = bus + USART_CR2;
	int cr2_function = status << bit;
	int cr2_status = io_read(usart_cr2);
	if (status == 0)
		cr2_status = cr2_status - cr2_function;
	else
		cr2_status = cr2_status|cr2_function;
	io_write(usart_cr2 , cr2_status);  // SET USART CR1
}

void usart_init(void){

	gpio_init(INIT_PORT_A); // IO port A RCC enabled

	gpio_alternate_function(GPIO_PORT_A , GPIO_PIN_9 , ALTERNATE_AF7);  // PA9 OPEN USART
	gpio_pull(GPIO_PORT_A, GPIO_PIN_9, GPIO_PULL_UP);

	gpio_alternate_function(GPIO_PORT_A , GPIO_PIN_10 , ALTERNATE_AF7);  // PA10 OPEN USART
	gpio_pull(GPIO_PORT_A, GPIO_PIN_10, GPIO_PULL_UP);

	usart_rcc_enable(RCC_APB2_ENR, INIT_USART1);

	/* set USART configuration*/
	usart_cr1(USART1_REGISTER, USART_CR1_UE, USART_HIGH_VAL);  // SET USART CR1 UE
	usart_cr2(USART1_REGISTER, USART_CR2_STOP_LOW, USART_LOW_VAL);  // SET USART CR2 stop bits
	usart_cr2(USART1_REGISTER, USART_CR2_STOP_HIGH, USART_LOW_VAL);  // SET USART CR2 stop bits
	usart_setBaudrate(USART1_REGISTER, USART6_DEFAULT_FREQUENCE, BRR_115200);  // set USART baud rate
	usart_cr1(USART1_REGISTER, USART_CR1_TE, USART_HIGH_VAL);  // SET USART CR1 TE

}

void usart_setBaudrate(int bus, int clk_rate, int baudrate){
	int usart_brr = bus + USART_BRR;
	int usart_div_mantissa = (clk_rate / (16 * baudrate)) << 4;
	int usart_div_fraction = ((clk_rate % (16 * baudrate)) * 16) / (16 * baudrate); // formula : (remainder/dividend) / (1/16)
	int usart_div = usart_div_mantissa + usart_div_fraction;
	io_write(usart_brr , usart_div);  // set USART baud rate
}

void usart_txData(int bus, uint8_t tx_buffer){
	int usart_srAdd = bus + USART_SR;
	int usart_sr = io_read(usart_srAdd);

	while ((usart_sr&0x80) == 0)  // wait until TXE = 1
		usart_sr = io_read(usart_srAdd);
	usart_dr(bus, tx_buffer);  // Write the data to send in the USART_DR register

	usart_sr = io_read(usart_srAdd);
	while ((usart_sr&0x40) == 0)  // wait until TC = 1
		usart_sr = io_read(usart_srAdd);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void usart_putc(uint8_t charactor) {
	usart_txData(USART1_REGISTER, charactor);
}

void usart_printint(int data, int base, int sgn)
{
  static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int count, neg;
  int abs_data;

  neg = 0;
  if(sgn && data < 0){
    neg = 1;
    abs_data = -data;
  } else {
	abs_data = data;
  }

  /*count = 0;
  do{
    buf[count++] = digits[abs_data % base];
  }while((abs_data /= base) != 0);*/
  count = 1;
  for (int i=0; i < count; i++){
	  buf[i] = digits[abs_data % base];
	  if ((abs_data /= base) != 0)
		  count++;
  }

  if(neg)
    buf[count++] = '-';

  while(--count >= 0)
    usart_putc(buf[count]);
}

void usart_printfloat(float xx) // can't work
{
  int beg=(int)(xx);
  int fin=(int)(xx*100)-beg*100;
  usart_printint(xx, 10, 1);
  usart_putc('.');
  if(fin<10)
	  usart_putc('0');
  usart_printint(fin, 10, 1);
}

// Only understands %d, %x, %p, %s, %f.
void usart_printf(const char *fmt, ...)	// TODO, https://github.com/shreshthtuli/xv6/blob/master/printf.charactor
{
  char *s;
  int charactor, i, state;
  int *argument;

  state = 0;
  argument = (int*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
    charactor = fmt[i] & 0xff;
    if(state == 0){
      if(charactor == '%'){
        state = '%';
      } else {
        usart_putc(charactor);
      }
    } else if(state == '%'){
      if(charactor == 'd'){
        usart_printint(*argument, 10, 1);
        argument++;
      } else if(charactor == 'x' || charactor == 'p'){
        usart_printint(*argument, 16, 0);
        argument++;
      } else if(charactor == 's'){
        s = (char*)*argument;
        argument++;
        if(s == 0)
          s = "(null)";
        while(*s != 0){
          usart_putc(*s);
          s++;
        }
      } else if(charactor == 'c'){
        usart_putc(*argument);
        argument++;
      } else if(charactor == 'f'){ // MOD-2
        usart_printfloat((float)*argument);
        argument++;
      } else if(charactor == '%'){
        usart_putc(charactor);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        usart_putc('%');
        usart_putc(charactor);
      }
      state = 0;
    }
  }
}

