//
// Created by Jinliang on 11/12/2023.
//

#include "Timer.h"

void Timer_Init(void) {
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //选择时基单元的时钟
    TIM_InternalClockConfig(TIM3);

    //初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;    //基本定时器无，随便设为0
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    //使能中断
    TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);

    //配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    //启动定时器
    TIM_Cmd(TIM3, ENABLE);
}