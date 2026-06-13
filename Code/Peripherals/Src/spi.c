#include "spi.h"

void spi_init_master(void) {
  gpio_config(PORT_SCK, PIN_SCK, GPIO_MODE_AF_PP);
  gpio_config(PORT_MOSI, PIN_MOSI, GPIO_MODE_AF_PP);
  gpio_config(PORT_MISO, PIN_MISO, GPIO_MODE_INPUT_FLOATING);
  gpio_config(PORT_CS, PIN_CS, GPIO_MODE_OUTPUT_PP);
  SPI1_CR1 = 0;
  SPI1_CR1 |= (1 << 2);
  SPI1_CR1 |= (0x05 << 3);
  SPI1_CR1 |= (1 << 9);
  SPI1_CR1 |= (1 << 8);
  SPI1_CR1 |= (1 << 6);
  gpio_write_pin(PORT_CS, PIN_CS, 1);
}

void spi1_send(uint8_t data) {
  gpio_write_pin(PORT_CS, PIN_CS, 0);
  while (!(SPI1_SR & (1 << 1))) {}
  SPI1_DR = data;
  while (!(SPI1_SR & (1 << 0))) {}
  uint32_t checkDataR = SPI1_DR;
  checkDataR++;
  checkDataR--;
  while (SPI1_SR & (1 << 7)) {}
  gpio_write_pin(PORT_CS, PIN_CS, 1);
}
