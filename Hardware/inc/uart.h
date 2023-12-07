//
// Created by Jinliang on 12/4/2023.
//

#ifndef STM32F103C8T6_UART_RX_TX_DMA_UART_H
#define STM32F103C8T6_UART_RX_TX_DMA_UART_H

/************** USER CONFIGURATION *******************/
/* UART configuration */
#define UART_WORD_LEN   (8)
#define UART_STOP_BITS  (1)
#define UART_BOUND_RATE (9600)

/* The size of USART RX buffer for DMA to transfer */
#define DMA_BUF_SIZE    (10)
/************************************************/

#include <stddef.h>
#include "stm32f10x.h"

void uart_init(void);
void uart_send_byte(uint8_t byte);
void uart_send_bytes(const uint8_t bytes[], size_t len);
void uart_send_string(const char* str);

#endif //STM32F103C8T6_UART_RX_TX_DMA_UART_H
