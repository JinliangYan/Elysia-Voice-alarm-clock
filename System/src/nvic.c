/**
* \file            nvic.c
* \date            12/4/2023
* \brief
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

#include "nvic.h"

void
nvic_init(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 抢占优先级为2位, 从优先级为2位

    NVIC_InitTypeDef NVIC_InitStructure;

    /* USART1_IRQn */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         // 开启串口1的中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 设置抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 设置从优先级为1
    NVIC_Init(&NVIC_InitStructure);                           // 初始化

    /* DMA1_Channel5_IRQn-USART1_Rx */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  // 开启DMA1通道5(USART1-RX)的中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 设置抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 设置从优先级为1
    NVIC_Init(&NVIC_InitStructure);                           // 初始化

    /* TIM3_IRQn */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}