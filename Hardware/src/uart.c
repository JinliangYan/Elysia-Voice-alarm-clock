/**
* \file            uart.c
* \date            12/4/2023
* \brief           USART1 Driver Implementation
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

#define LOG_TAG "UART"
#include "elog.h"

#include <string.h>
#include "uart.h"

/* Clock configuration */
#define UART_RCC      RCC_APB2Periph_USART1
#define UART_GPIO_RCC RCC_APB2Periph_GPIOA
/* Pin configuration */
#define UART_GPIOx    GPIOA
#define UART_TX_PIN   GPIO_Pin_9
#define UART_RX_PIN   GPIO_Pin_10

void uart_rx_check(void);
void uart_process_data(const void* data, size_t len);

#define PACKET_LEN (10)
extern uint8_t uart_rx_packet[PACKET_LEN];

/**
 * \brief           Calculate length of statically allocated array
 */
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

/**
 * \brief           Current data index processed by \ref uart_process_data()
 */
static int process_idx;

/**
 * \brief           USART RX buffer for DMA to transfer every received byte
 * \note            Contains raw data that are about to be processed by different events
 */
uint8_t uart_rx_dma_buffer[DMA_BUF_SIZE];

/**
 * \brief           Check for new data received with DMA
 *
 * User must select context to call this function from:
 * - Only interrupts (DMA HT, DMA TC, UART IDLE) with same preemption priority level
 * - Only thread context (outside interrupts)
 *
 * If called from both context-es, exclusive access protection must be implemented
 * This mode is not advised as it usually means architecture design problems
 *
 * When IDLE interrupt is not present, application must rely only on thread context,
 * by manually calling function as quickly as possible, to make sure
 * data are read from raw buffer and processed.
 *
 * Not doing reads fast enough may cause DMA to overflow unread received bytes,
 * hence application will lost useful data.
 *
 * Solutions to this are:
 * - Improve architecture design to achieve faster reads
 * - Increase raw buffer size and allow DMA to write more data before this function is called
 */
void
uart_rx_check(void) {
    /*
     * Set old position variable as static.
     *
     * Linker should (with default C configuration) set this variable to `0`.
     * It is used to keep latest read start position,
     * transforming this function to not being reentrant or thread-safe
     */
    static size_t old_pos = 0;
    size_t pos;

    /* Calculate current position in buffer and check for new data available */
    pos = ARRAY_LEN(uart_rx_dma_buffer) - DMA_GetCurrDataCounter(DMA1_Channel5);
    if (pos != old_pos) {                       /* Check change in received data */
        if (pos > old_pos) {                    /* Current position is over previous one */
            /*
             * Processing is done in "linear" mode.
             *
             * Application processing is fast with single data block,
             * length is simply calculated by subtracting pointers
             *
             * [   0   ]
             * [   1   ] <- old_pos |------------------------------------|
             * [   2   ]            |                                    |
             * [   3   ]            | Single block (len = pos - old_pos) |
             * [   4   ]            |                                    |
             * [   5   ]            |------------------------------------|
             * [   6   ] <- pos
             * [   7   ]
             * [ N - 1 ]
             */
            uart_process_data(&uart_rx_dma_buffer[old_pos], pos - old_pos);
        } else {
            /*
             * Processing is done in "overflow" mode..
             *
             * Application must process data twice,
             * since there are 2 linear memory blocks to handle
             *
             * [   0   ]            |---------------------------------|
             * [   1   ]            | Second block (len = pos)        |
             * [   2   ]            |---------------------------------|
             * [   3   ] <- pos
             * [   4   ] <- old_pos |---------------------------------|
             * [   5   ]            |                                 |
             * [   6   ]            | First block (len = N - old_pos) |
             * [   7   ]            |                                 |
             * [ N - 1 ]            |---------------------------------|
             */
            uart_process_data(&uart_rx_dma_buffer[old_pos], ARRAY_LEN(uart_rx_dma_buffer) - old_pos);
            if (pos > 0) {
                uart_process_data(&uart_rx_dma_buffer[0], pos);
            }
        }
        old_pos = pos;                          /* Save current position as old for next transfers */
    }
}

/**
 * \brief Send a byte via UART
 *
 * This function sends a byte via the specified UART peripheral (USART1 in this case).
 * It waits until the transmit buffer is empty before sending the byte.
 *
 * \param byte  Byte to send via UART
 */
void uart_send_byte(uint8_t byte) {
    USART_SendData(USART1, byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
}

/**
 * \brief Send an array of bytes via UART
 *
 * This function sends an array of bytes via UART using the uart_send_byte function.
 *
 * \param bytes: Pointer to the array of bytes to send
 * \param len:   Number of bytes to send
 */
void uart_send_bytes(const uint8_t bytes[], size_t len) {
    for (size_t i = 0; i < len; ++i) {
        uart_send_byte(bytes[i]);
    }
}


/**
 * \brief           Process received data over UART
 * \note            Either process them directly or copy to other bigger buffer
 * \param[in]       data: Data to process
 * \param[in]       len: Length in units of bytes
 */
void
uart_process_data(const void* data, size_t len) {
    const uint8_t* d = data;
    /*
     * This function is called on DMA TC or HT events, and on UART IDLE (if enabled) event.
     *
     * For the sake of this example, function does a loop-back data over UART in polling mode.
     * Check ringbuff RX-based example for implementation with TX & RX DMA transfer.
     */

    for (; len > 0; --len, ++d, ++process_idx) {
        uart_rx_packet[process_idx] = *d;
    }
}

/**
 * \brief           Send string to USART
 * \param[in]       str: String to send
 */
void
uart_send_string(const char* str) {
    uart_send_bytes((uint8_t*)str, strlen(str));
}

/**
 * \brief Initializes the UART DMA for receiving data
 *
 * This function initializes the DMA (Direct Memory Access) for UART data reception.
 * It configures the DMA channel to transfer data from the UART receive buffer to a circular memory buffer.
 * Interrupts for Half Transfer (HT) and Transfer Complete (TC) are enabled to handle the received data efficiently.
 *
 * \note This function assumes the UART1 peripheral is used for communication.
 */
static void uart_dma_init(void) {
    /* Peripheral clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* DMA-RX */
    DMA_InitTypeDef dma_init_structure;
    dma_init_structure.DMA_BufferSize = DMA_BUF_SIZE;                        // Set DMA buffer size
    dma_init_structure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // Set DMA direction as peripheral to memory
    dma_init_structure.DMA_M2M = DMA_M2M_Disable;                            // Disable memory to memory transfer
    dma_init_structure.DMA_MemoryBaseAddr = (uint32_t)uart_rx_dma_buffer;    // Set memory address
    dma_init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // Set transfer unit size as byte
    dma_init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // Enable memory address increment during transfer
    dma_init_structure.DMA_Mode = DMA_Mode_Circular;                         // Set circular mode
    dma_init_structure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);   // Set peripheral address
    dma_init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // Set transfer unit size as byte
    dma_init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // Disable peripheral address increment during transfer
    dma_init_structure.DMA_Priority = DMA_Priority_High;                     // Set DMA channel priority
    DMA_Init(DMA1_Channel5, &dma_init_structure);

    /* Enable HT & TC interrupts */
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC | DMA_IT_HT, ENABLE);

    /* DMA1_Channel5_IRQn interrupt configuration */
    NVIC_SetPriority(DMA1_Channel5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);

    /* Enable DMA */
    DMA_Cmd(DMA1_Channel5, ENABLE);

    /* Enable UART DMA */
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
}

/**
 * \brief Initializes the UART communication
 *
 * This function initializes the UART communication on USART1.
 * It configures the GPIO pins for TX and RX, USART settings (baud rate, word length, stop bits, etc.),
 * and enables the USART1 peripheral and its associated DMA for reception.
 * Additionally, it enables the IDLE interrupt for USART1.
 *
 * \note This function assumes the use of USART1 for communication.
 */
void uart_init(void) {
    process_idx = 0;

    /* Peripheral clock enable */
    RCC_APB2PeriphClockCmd(UART_GPIO_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(UART_RCC, ENABLE);

    /*
     * USART1 GPIO Configuration
     *
     * PA9   ------> USART1_TX
     * PA10  ------> USART1_RX
     */
    GPIO_InitTypeDef gpio_init_structure;

    /* TX-pin configuration */
    gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_structure.GPIO_Pin = UART_TX_PIN;
    GPIO_Init(UART_GPIOx, &gpio_init_structure);

    /* RX-pin configuration */
    gpio_init_structure.GPIO_Mode = GPIO_Mode_IPU;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_structure.GPIO_Pin = UART_RX_PIN;
    GPIO_Init(UART_GPIOx, &gpio_init_structure);

    /* USART configuration */
    USART_InitTypeDef usart_init_structure;
    usart_init_structure.USART_WordLength = UART_WORD_LEN;
    usart_init_structure.USART_StopBits = UART_STOP_BITS;
    usart_init_structure.USART_Parity = USART_Parity_No;
    usart_init_structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart_init_structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init_structure.USART_BaudRate = UART_BOUND_RATE;
    USART_Init(USART1, &usart_init_structure);

    /* Enable IDLE interrupt */
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

    /* USART interrupt configuration */
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    /* USART1 DMA Init */
    uart_dma_init();

    USART_Cmd(USART1, ENABLE);
}

/**
 * \brief UART Receive Error Handler
 *
 * This function serves as the error handler for UART receive errors.
 * It is called when an error occurs during UART reception.
 *
 * \note You should customize the implementation of this function according to your error handling requirements.
 */
void uart_receive_err_handler(void) {
    // TODO: Implement error handling steps
}


/* Interrupt handlers here */

/**
 * \brief           DMA1 channel5 interrupt handler for USART1 RX
 */
void
DMA1_Channel5_IRQHandler(void) {
    /* Check half-transfer complete interrupt */
    if (DMA_GetITStatus(DMA1_IT_HT5) == SET) {
        DMA_ClearITPendingBit(DMA1_IT_HT5); /* Clear half-transfer complete flag */
        uart_rx_check();                    /* Check for data to process */
    }

    /* Check transfer-complete interrupt */
    if (DMA_GetITStatus(DMA1_IT_TC5) == SET) {
        DMA_ClearITPendingBit(DMA1_IT_TC5); /* Clear transfer complete flag */
        uart_rx_check();                    /* Check for data to process */
    }

    /* Implement other events when needed */
}

/**
 * \brief           USART1 global interrupt handler
 */
void
USART1_IRQHandler(void) {
    /* Check for IDLE line interrupt */
    if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET) {
        /* Read the DR register to clear USART_IT_IDLE pending flag*/
        USART1->DR;
        /* Check for data to process */
        uart_rx_check();
        /*
        * Upon the occurrence of an IDLE interrupt, the data packet should have been fully sent.
        * Therefore, we reset process_idx to zero.
        */
        if (process_idx >= PACKET_LEN) {
            uart_receive_err_handler();
        }
        process_idx = 0;
    }

    /* Implement other events when needed */
}

/* Debug here */
#if defined(DEBUG)
void
uart_test(void) {
    extern void elog_init_(void);
    elog_init_();
    log_i("uart_test");
    uart_init();
    uint8_t bytes[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    uart_send_bytes(bytes, 10);
    while (true) {}
}
#endif  /* DEBUG */