/*
 * i2c.c
 *
 *  Created on: Oct 22, 2024
 *      Author: Xavier
 */


#include <i2c.h>
#include <gpio.h>
#include <io.h>

void i2c_rcc_enable(int bus, int i2c) {
    int i2c_rcc_addr = RCC_REGISTER + bus;
    io_writeMask(i2c_rcc_addr, i2c, i2c);
}

void i2c_cr2_freq(int bus, int freq){
	int i2c_freq_addr = bus + I2C_CR2;
	io_writeMask(i2c_freq_addr, freq, 0X3F);
}

void i2c_ccr(int bus, int scl_freq){
	int i2c_ccr_addr = bus + I2C_CCR;
	int t_pclk = 1/((io_read(bus + I2C_CR2) & 0X3F)*1000000);
	int t_scl = 1/scl_freq;
	int ccr_value = (t_scl/2)/t_pclk;
	io_writeMask(i2c_ccr_addr, ccr_value, 0x0FFF);
}

void i2c_trise(int bus, int trise){
	int i2c_trise_addr = bus + I2C_TRISE;
	int t_pclk = 1/((io_read(bus + I2C_CR2) & 0X3F)*1000000);
	int trise_status = trise / t_pclk + 1;
	io_writeMask(i2c_trise_addr, trise_status, 0X3F);
}

void i2c_cr1_PE(int bus, int enable){
	int i2c_cr1_addr = bus + I2C_CR1;
	io_writeMask(i2c_cr1_addr, enable, enable);
}

void i2c_init(void) {
    gpio_init(INIT_PORT_B); // GPIO port B RCC enabled
    gpio_init(INIT_PORT_F); // GPIO port F RCC enabled

    gpio_alternate_function(GPIO_PORT_B, GPIO_PIN_6, ALTERNATE_AF4);  // PB6 for I2C1_SCL
    gpio_alternate_function(GPIO_PORT_B, GPIO_PIN_7, ALTERNATE_AF4);  // PB7 for I2C1_SDA
    gpio_alternate_function(GPIO_PORT_F, GPIO_PIN_0, ALTERNATE_AF4);  // PF0 for I2C2_SDA
    gpio_alternate_function(GPIO_PORT_F, GPIO_PIN_1, ALTERNATE_AF4);  // PF7 for I2C2_SCL

    i2c_rcc_enable(RCC_APB1_ENR,INIT_I2C1);
    i2c_rcc_enable(RCC_APB1_ENR,INIT_I2C2);

    //i2c_cr2_freq(I2C1_REGISTER, I2C_CR2_8M);
    i2c_ccr(I2C1_REGISTER, I2C_CCR_100K);
    i2c_trise(I2C1_REGISTER, I2C_TRICE_1000n);
    i2c_cr1_PE(I2C1_REGISTER, I2C_CR1_PE);
}

void i2c_write(int bus, int data){
	int i2c_cr1_addr = bus + I2C_CR1;

	io_writeMask(i2c_cr1_addr, I2C_CR1_START, I2C_CR1_START);
}
