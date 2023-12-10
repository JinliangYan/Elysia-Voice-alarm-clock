/**
* \file            uart.h
* \date            12/4/2023
* \brief           Header file for UART module
*/

/*
* Copyright (c) 2023 JinLiang YAN
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* This file is part of Elysia-Voice-alarm-clock.
*
* Author:          JinLiang YAN <yanmiku0206@outlook.com>
*/

#ifndef STM32F103C8T6_UART_RX_TX_DMA_UART_H
#define STM32F103C8T6_UART_RX_TX_DMA_UART_H

#include <stddef.h>
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------ USER CONFIGURATION --------------------------*/
/* UART configuration */
#define UART_WORD_LEN   (8)
#define UART_STOP_BITS  (1)
#define UART_BOUND_RATE (9600)

/* The size of USART RX buffer for DMA to transfer */
#define DMA_BUF_SIZE    (10)
/*-----------------------------------------------------------------*/

void uart_init(void);
void uart_send_byte(uint8_t byte);
void uart_send_bytes(const uint8_t bytes[], size_t len);
void uart_send_string(const char* str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //STM32F103C8T6_UART_RX_TX_DMA_UART_H
