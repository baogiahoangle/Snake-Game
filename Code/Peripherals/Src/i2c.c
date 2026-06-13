#include "i2c.h"

void i2c_init(void) {
  I2C1_CR1 &= ~(uint32_t)(1 << 0);
  I2C1_CR2 = 8;
  I2C1_CCR = 40;
  I2C1_TRISE = 9;
  I2C1_CR1 |= (1 << 0);
}

void i2c_start(void) {
  I2C1_CR1 &= ~(uint32_t)(1 << 9);
  I2C1_CR1 |= (1 << 8);
  while (!(I2C1_SR1 & 0x01)) {}
}

void i2c_stop(void) {
  I2C1_CR1 |= (1 << 9);
}

void i2c_send_address(uint8_t addr, uint8_t rw) {
  I2C1_DR = (uint32_t)((addr << 1) | (rw & 0x01));
  while (!(I2C1_SR1 & (1 << 1))) {}
  (void)I2C1_SR2;
}

uint8_t i2c_send_data(uint8_t data) {
  I2C1_DR = data;
  while (!(I2C1_SR1 & (1 << 2))) {}
  if (I2C1_SR1 & (1 << 10)) {
    I2C1_SR1 &= ~(uint32_t)(1 << 10);
    i2c_stop();
    return 1;
  }
  return 0;
}

uint8_t i2c_read_data(uint8_t ack) {
  if (ack) {
    I2C1_CR1 |= (1 << 10);
  } else {
    I2C1_CR1 &= ~(uint32_t)(1 << 10);
  }
  while (!(I2C1_SR1 & (1 << 6))) {}
  return (uint8_t)I2C1_DR;
}
