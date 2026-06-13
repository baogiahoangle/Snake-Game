#ifndef __UART_H
#define __UART_H
#include <stdint.h>

#define UART_ADD_BASE  0x40013800

#define USART_SR_OFFSET    0x00
#define USART_DR_OFFSET    0x04
#define USART_BRR_OFFSET   0x08
#define USART_CR1_OFFSET   0x0C
#define USART_CR2_OFFSET   0x10
#define USART_CR3_OFFSET   0x14
#define USART_GTPR_OFFSET  0x18

#define USART1_SR   *((volatile uint32_t*)(UART_ADD_BASE + USART_SR_OFFSET))
#define USART1_DR   *((volatile uint32_t*)(UART_ADD_BASE + USART_DR_OFFSET))
#define USART1_BRR  *((volatile uint32_t*)(UART_ADD_BASE + USART_BRR_OFFSET))
#define USART1_CR1  *((volatile uint32_t*)(UART_ADD_BASE + USART_CR1_OFFSET))
#define USART1_CR2  *((volatile uint32_t*)(UART_ADD_BASE + USART_CR2_OFFSET))
#define USART1_CR3  *((volatile uint32_t*)(UART_ADD_BASE + USART_CR3_OFFSET))
#define USART1_GTPR *((volatile uint32_t*)(UART_ADD_BASE + USART_GTPR_OFFSET))

void uart1_init(void);
void uart1_send_char(char c);
void uart1_send_string(const char *str);
void USART1_IRQHandler(void);

#endif
