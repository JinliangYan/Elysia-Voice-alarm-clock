//
// Created by Jinliang on 12/4/2023.
//

#include "nvic.h"

void nvic_init(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     // 抢占优先级为2位, 从优先级为2位

    NVIC_InitTypeDef NVIC_InitStructure;

    /* USART1_IRQn */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                   // 开启串口1的中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     // 使能中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;           // 设置抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                  // 设置从优先级为1
    NVIC_Init(&NVIC_InitStructure);                        // 初始化

    /* DMA1_Channel5_IRQn-USART1_Rx */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;            // 开启DMA1通道5(USART1-RX)的中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     // 使能中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;           // 设置抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                  // 设置从优先级为1
    NVIC_Init(&NVIC_InitStructure);                        // 初始化

    /* TIM3_IRQn */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}