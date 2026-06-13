#include "rcc.h"

void rcc_config_72mhz(void) {
  RCC->CR.BITS.HSEON = 1;
  while (!RCC->CR.BITS.HSERDY) {}

  ACR->BITS.LATENCY = 2;

  RCC->CFGR.BITS.PLLSRC = 1;
  RCC->CFGR.BITS.PLLMUL = 7;
  RCC->CFGR.BITS.USBPRE = 0;
  RCC->CR.BITS.PLLON = 1;
  while (!RCC->CR.BITS.PLLRDY) {}

  RCC->CFGR.BITS.HPRE = 0;
  RCC->CFGR.BITS.PPRE1 = 4;
  RCC->CFGR.BITS.PPRE2 = 0;

  RCC->CFGR.BITS.SW = 2;
  while (RCC->CFGR.BITS.SWS != 2) {}
}

void rcc_enable_port_a(void) {
  RCC->APB2ENR.BITS.IOPAEN = 1;
}

void rcc_enable_port_b(void) {
  RCC->APB2ENR.BITS.IOPBEN = 1;
}

void rcc_enable_port_c(void) {
  RCC->APB2ENR.BITS.IOPCEN = 1;
}

void rcc_enable_afio(void) {
  RCC->APB2ENR.BITS.AFIOEN = 1;
}

void rcc_enable_tim2(void) {
  RCC->APB1ENR.BITS.TIM2EN = 1;
}

void rcc_enable_adc01(void) {
  RCC->APB2ENR.BITS.ADC1EN = 1;
}

void rcc_enable_uart1(void) {
  RCC->APB2ENR.BITS.USART1EN = 1;
}

void rcc_enable_spi1(void) {
  RCC->APB2ENR.BITS.SPI1EN = 1;
}

void rcc_enable_i2c1(void) {
  RCC->APB1ENR.BITS.I2C1EN = 1;
}

void rcc_enable_usb(void) {
  RCC->APB1ENR.BITS.USBEN = 1;
}
