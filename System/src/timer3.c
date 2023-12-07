//
// Created by Jinliang on 11/12/2023.
//

#include "timer3.h"

void
timer3_init(void) {
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
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //基本定时器无，随便设为0
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    //使能中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* TIM3_IRQn interrupt configuration */
    NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(TIM3_IRQn);

    //启动定时器
    TIM_Cmd(TIM3, ENABLE);
}