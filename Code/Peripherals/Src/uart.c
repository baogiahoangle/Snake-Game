#include "uart.h"
#include "gpio.h"
#include "exti.h"

void USART1_IRQHandler(void) {
  if (USART1_SR & (1 << 5)) {
    char c = (char)(USART1_DR & (0xFF));
    uart1_send_char(c);
  }
}

void uart1_init(void) {
  gpio_config(GPIOA, GPIO_PIN_9, GPIO_MODE_AF_PP);
  gpio_config(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT_FLOATING);

  USART1_BRR = 0x0341;
  USART1_CR1 |= (1 << 13);
  USART1_CR1 |= (1 << 3);
  USART1_CR1 |= (1 << 2);
  USART1_CR1 |= (1 << 5);
  nvic_uart_enable();
}

void uart1_send_char(char c) {
  while (!(USART1_SR & (1 << 7)));
  USART1_DR = c;
}

void uart1_send_string(const char *str) {
  while (*str) {
    uart1_send_char(*str++);
  }
}
