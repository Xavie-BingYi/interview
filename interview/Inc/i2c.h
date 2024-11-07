/*
 * i2c.h
 *
 *  Created on: Oct 22, 2024
 *      Author: Xavier
 */

#ifndef I2C_H_
#define I2C_H_

#define I2C3_REGISTER 0x40005C00
#define I2C2_REGISTER 0x40005800
#define I2C1_REGISTER 0x40005400

#define INIT_I2C3 0x00800000
#define INIT_I2C2 0x00400000
#define INIT_I2C1 0x00200000

#define I2C_CR1   0x00
#define I2C_CR2   0x04
#define I2C_DR    0x10
#define I2C_CCR   0x1C
#define I2C_TRISE 0x20

#define I2C_CR2_2M   0x02
#define I2C_CR2_8M   0x08
#define I2C_CR2_50M  0x32

#define I2C_CCR_100K 100000

#define I2C_TRICE_1000n 1000 * 1e-9

#define I2C_CR1_START   0x0100
#define I2C_CR1_PE      0x0001



void i2c_rcc_enable(int bus, int i2c);
void i2c_init(void);
void i2c_cr2_freq(int bus, int freq);
void i2c_ccr(int bus, int scl_freq);
void i2c_trise(int bus, int trise);
void i2c_cr1_PE(int bus, int enable);

#endif /* I2C_H_ */
