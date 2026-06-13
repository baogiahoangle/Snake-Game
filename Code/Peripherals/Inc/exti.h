#ifndef __EXTI_H
#define __EXTI_H
#include "type.h"
#include "afio.h"
#include "gpio.h"

#define EXTI_RISING_MODE   0x00
#define EXTI_FALLING_MODE  0x01
#define EXTI_BOTH_MODE     0x02

#define NVIC_ISER0 *((uint32_t*)(0xE000E100))
#define NVIC_ISER1 *((uint32_t*)(0xE000E104))

typedef struct {
  uint32_bit_t IMR;
  uint32_bit_t EMR;
  uint32_bit_t RTSR;
  uint32_bit_t FTSR;
  uint32_bit_t SWIER;
  uint32_bit_t PR;
} exti_t;

#define EXTI ((volatile exti_t*)0x40010400UL)

void EXTI0_IRQHandler(void);
uint8_t exti_get_pin_number(uint16_t gpio_pin);
void exti_init(uint16_t gpio_pin, volatile gpio_t *Port, uint8_t type);
void nvic_uart_enable(void);
void nvic_usb_enable(void);

#endif
