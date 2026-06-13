#include "gpio.h"

void gpio_config(volatile gpio_t *GPIOx, uint16_t pin, uint32_t mode) {
  uint32_t position = 0;
  uint32_t config = 0;
  for (position = 0; position < 16; position++) {
    if (pin & (1 << position)) {
      if (position < 8) {
        GPIOx->CRL.REG &= ~(uint32_t)(0xF << (position * 4));
      } else {
        GPIOx->CRH.REG &= ~(uint32_t)(0xF << (position - 8) * 4);
      }
      switch (mode) {
        case GPIO_MODE_OUTPUT_PP:
          config = (0x03 << 0) | (0x00 << 2);
          break;
        case GPIO_MODE_OUTPUT_OD:
          config = (0x03 << 0) | (0x01 << 2);
          break;
        case GPIO_MODE_AF_PP:
          config = (0x03 << 0) | (0x02 << 2);
          break;
        case GPIO_MODE_AF_OD:
          config = (0x03 << 0) | (0x03 << 2);
          break;
        case GPIO_MODE_INPUT_ANALOG:
          config = (0x00 << 2);
          break;
        case GPIO_MODE_INPUT_FLOATING:
          config = (0x01 << 2);
          break;
        case GPIO_MODE_INPUT_PU:
          config = (0x02 << 2);
          GPIOx->ODR.REG |= (1 << position);
          break;
        case GPIO_MODE_INPUT_PD:
          config = (0x02 << 2);
          GPIOx->ODR.REG &= ~(1 << position);
          break;
      }
      if (mode == GPIO_MODE_INPUT_FLOATING ||
          mode == GPIO_MODE_INPUT_PD || mode == GPIO_MODE_INPUT_PU) {
        config |= 0x00;
      }
      if (position < 8) {
        GPIOx->CRL.REG |= (config << (position * 4));
      } else {
        GPIOx->CRH.REG |= (config << (position - 8) * 4);
      }
    }
  }
}

void gpio_write_pin(volatile gpio_t *GPIOx, uint16_t pin, uint8_t state) {
  if (state) {
    GPIOx->BSRR.REG = pin;
  } else {
    GPIOx->BRR.REG = pin;
  }
}

uint8_t gpio_read_pin(volatile gpio_t *GPIOx, uint16_t pin) {
  return ((GPIOx->IDR.REG & pin) ? 1 : 0);
}
