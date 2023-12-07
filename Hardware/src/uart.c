//
// Created by Jinliang on 12/4/2023.
//
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

#define PACKET_LEN 10
extern uint8_t uart_rx_packet[PACKET_LEN];

/**
 * @brief           Calculate length of statically allocated array
 */
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

/**
 * @brief           USART RX buffer for DMA to transfer every received byte
 * @note            Contains raw data that are about to be processed by different events
 */
uint8_t uart_rx_dma_buffer[DMA_BUF_SIZE];

/**
 * @brief           Check for new data received with DMA
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
     * 将旧位置变量设置为静态。
     *
     * 链接器应该（在默认的C配置下）将此变量设置为`0`。
     * 它用于保持最新的读取开始位置，
     * 将此函数转换为不可重入或线程安全的函数
     */
    static size_t old_pos = 0;
    size_t pos;

    /* 计算缓冲区中的当前位置并检查是否有新数据可用 */
    pos = ARRAY_LEN(uart_rx_dma_buffer) - DMA_GetCurrDataCounter(DMA1_Channel5);
    if (pos != old_pos) {    /* 检查接收到的数据是否发生变化 */
        if (pos > old_pos) { /* 当前位置位于先前位置之上 */
            /*
             * 处理以"线性"模式进行。
             *
             * 应用程序处理单个数据块时速度很快，
             * 长度通过减法计算指针来简单地计算
             *
             * [   0   ]
             * [   1   ] <- old_pos |------------------------------------|
             * [   2   ]            |                                    |
             * [   3   ]            | 单块数据 (len = pos - old_pos)      |
             * [   4   ]            |                                    |
             * [   5   ]            |------------------------------------|
             * [   6   ] <- pos
             * [   7   ]
             * [ N - 1 ]
             */

            uart_process_data(&uart_rx_dma_buffer[old_pos], pos - old_pos);
        } else {
            /*
             * 处理以"溢出"模式进行。
             *
             * 应用程序必须处理数据两次，
             * 因为有2个线性内存块要处理
             *
             * [   0   ]            |---------------------------------|
             * [   1   ]            | 第二块数据 (len = pos)         |
             * [   2   ]            |---------------------------------|
             * [   3   ] <- pos
             * [   4   ] <- old_pos |---------------------------------|
             * [   5   ]            |                                 |
             * [   6   ]            | 第一块数据 (len = N - old_pos) |
             * [   7   ]            |                                 |
             * [ N - 1 ]            |---------------------------------|
             */

            uart_process_data(&uart_rx_dma_buffer[old_pos], ARRAY_LEN(uart_rx_dma_buffer) - old_pos);
            if (pos > 0) {
                uart_process_data(&uart_rx_dma_buffer[0], pos);
            }
        }
        old_pos = pos; /* 将当前位置保存为下一次传输的旧位置 */
    }
}

/**
 * @brief       UART send a single byte
 * @param       byte: byte to send
 */
void
uart_send_byte(uint8_t byte) {
    USART_SendData(USART1, byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
}

/**
 * @brief               UART send bytes
 * @param               bytes:Bytes array to send
 * @param               len:Length of bytes array
 */
void
uart_send_bytes(const uint8_t bytes[], size_t len) {
    for (int i = 0; i < len; ++i) {
        uart_send_byte(bytes[i]);
    }
}

/**
 * @brief           Process received data over UART
 * @note            Either process them directly or copy to other bigger buffer
 * @param[in]       data: Data to process
 * @param[in]       len: Length in units of bytes
 */
void
uart_process_data(const void* data, size_t len) {
    const uint8_t* d = data;
    static int i = 0;

    /*
     * This function is called on DMA TC or HT events, and on UART IDLE (if enabled) event.
     *
     * For the sake of this example, function does a loop-back data over UART in polling mode.
     * Check ringbuff RX-based example for implementation with TX & RX DMA transfer.
     */

    for (; len > 0; --len, ++d) {
        if (i >= PACKET_LEN) {
            i = 0;
        }
        uart_rx_packet[i] = *d;
        i++;
    }
}

/**
 * @brief           Send string to USART
 * @param[in]       str: String to send
 */
void
uart_send_string(const char* str) {
    uart_send_bytes((uint8_t*)str, strlen(str));
}

/**
 * DMA of USART1_Rx init
 */
static void
uart_dma_init(void) {
    /* Peripheral clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* DMA-RX */
    DMA_InitTypeDef dma_init_structure;
    dma_init_structure.DMA_BufferSize = DMA_BUF_SIZE;                        //设置DMA的缓冲区大小
    dma_init_structure.DMA_DIR = DMA_DIR_PeripheralSRC;                      //设置DMA为外设到内存方向
    dma_init_structure.DMA_M2M = DMA_M2M_Disable;                            //禁止内存到内存的运输
    dma_init_structure.DMA_MemoryBaseAddr = (uint32_t)uart_rx_dma_buffer;    //设置内存的地址
    dma_init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //每次传输单位为字节
    dma_init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //传输过程中内存地址自增
    dma_init_structure.DMA_Mode = DMA_Mode_Circular;                         //设置为循环模式
    dma_init_structure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);   //设置外设地址
    dma_init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //每次传输单位为字节
    dma_init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //禁止外设内存地址自增
    dma_init_structure.DMA_Priority = DMA_Priority_High;                     //设置DMA通道的优先级
    DMA_Init(DMA1_Channel5, &dma_init_structure);

    /* Enable HT & TC interrupts */
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC | DMA_IT_HT, ENABLE);

    /* DMA1_Channel5_IRQn interrupt configuration */
    NVIC_SetPriority(DMA1_Channel5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);

    /* Enable DMA */
    DMA_Cmd(DMA1_Channel5, ENABLE);

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); //允许串口DMA
}

/**
 * @brief           USART1 Initialization Function
 */
void
uart_init(void) {
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
    /*TX-pin*/
    gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_structure.GPIO_Pin = UART_TX_PIN;
    GPIO_Init(UART_GPIOx, &gpio_init_structure);
    /*RX-pin*/
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

    /* USART interrupt */
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    /*USART1 DMA Init*/
    uart_dma_init();

    USART_Cmd(USART1, ENABLE);
}

/* Interrupt handlers here */

/**
 * @brief DMA1 channel5 interrupt handler for USART1 RX
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
 * @brief           USART1 global interrupt handler
 */
void
USART1_IRQHandler(void) {
    /* Check for IDLE line interrupt */
    if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET) {
//      USART_ClearITPendingBit(USART1, USART_IT_IDLE);
        /* Then read the DR register */
        USART1->DR;
        /* Here the USART_IT_IDLE pending bit cleared */
        uart_rx_check(); /* Check for data to process */
    }

    /* Implement other events when needed */
}

void
uart_test(void) {
    extern void Elog_Init(void);
    Elog_Init();
    log_i("uart_test");
    uart_init();
    uint8_t bytes[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    uart_send_bytes(bytes, 10);
    while (true) {}
}